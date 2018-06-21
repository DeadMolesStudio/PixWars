#include <boost/asio.hpp>

#include "HTTP_connection.hpp"
#include "pixel_server.hpp"
#include "server_models.hpp"
#include "logger.hpp"

// implementation:

#include <boost/beast.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <sstream>
#include <string>


class HTTP_connection::HTTP_connection_impl : public std::enable_shared_from_this<HTTP_connection_impl> {
 public:
    HTTP_connection_impl(boost::asio::io_context& io_context);

    boost::asio::ip::tcp::socket& get_socket();

    /// Read the data from the client
    void read_request();
    /// Serialize JSON for response
    void serialize_updated_pixels(
            const std::vector<const PixelServer*>& updated_pixels);
    void serialize_painted_pixel(const PixelServer& pixel, bool success);
    void serialize_all_field(const std::vector<const PixelServer*>& field);
 private:
    boost::asio::ip::tcp::socket socket; // socket for connection
    boost::beast::flat_buffer buffer;
    boost::beast::http::request<boost::beast::http::string_body> request;
    boost::beast::http::response<boost::beast::http::string_body> response;

    /// Parse the client's response
    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred);
    void handle_write(const boost::system::error_code& error);

    /// Send the response to the client
    void send_response(std::string message,
                       boost::beast::http::status code
                       = boost::beast::http::status::ok);

};

HTTP_connection::HTTP_connection_impl::HTTP_connection_impl(boost::asio::io_context& io_context)
        : socket(io_context) {}

boost::asio::ip::tcp::socket& HTTP_connection::HTTP_connection_impl::get_socket() {
    return socket;
}

void HTTP_connection::HTTP_connection_impl::read_request() {
    boost::beast::http::async_read(
            socket,
            buffer,
            request,
            boost::bind(&HTTP_connection_impl::handle_read,
                    shared_from_this(), // error
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void HTTP_connection::HTTP_connection_impl::handle_read(const boost::system::error_code& error,
                                       size_t bytes_transferred) {
    if (error) {
        write_log(error.message());
        return;
    }

    if (request.method() != boost::beast::http::verb::post) {
        send_response("Bad method.\n",
                boost::beast::http::status::method_not_allowed);
        return;
    }
    if (!request.has_content_length()) {
        send_response("No content-length given.\n",
                boost::beast::http::status::length_required);
        return;
    }

    // parsing the request
    std::stringstream ss;
    std::string body = request.body();
    ss << body;
    boost::property_tree::ptree root;
    try {
        boost::property_tree::read_json(ss, root);
        auto command = root.get<std::string>("command");
        if (command == "update_since_last_update") {
            auto last_update = root.get<time_t>("last_update");
            update_since_last_update(
                    last_update,
                    boost::bind(&HTTP_connection_impl::serialize_updated_pixels,
                            shared_from_this(),
                            boost::placeholders::_1));
        }
        else if (command == "paint_pixel") {
            unsigned int id = 0;
            unsigned short color = 0;

            boost::property_tree::ptree pixel_tree = root.get_child(
                    "pixel_for_paint");
            id = pixel_tree.get<unsigned int>("id");
            color = pixel_tree.get<unsigned short>("color");

            paint_pixel(id, color,
                    boost::bind(&HTTP_connection_impl::serialize_painted_pixel,
                            shared_from_this(),
                            boost::placeholders::_1,
                            boost::placeholders::_2));
        }
        else if (command == "get_field") { // TODO: список команд? map?
            get_field(boost::bind(&HTTP_connection_impl::serialize_all_field,
                    shared_from_this(),
                    boost::placeholders::_1));
        }
        else
            throw std::invalid_argument("Wrong command got.\n");
    }
    catch (const boost::property_tree::json_parser_error &e) {
        send_response("Expected json file.\n",
                boost::beast::http::status::bad_request);
    }
    catch (const boost::property_tree::ptree_bad_path &e) {
        send_response("Wrong json format.\n",
                boost::beast::http::status::bad_request);
    }
    catch (const std::invalid_argument &e) {
        send_response(e.what(), boost::beast::http::status::bad_request);
    }
}

void HTTP_connection::HTTP_connection_impl::handle_write (const boost::system::error_code& error) {
    if (error) {
        write_log(error.message());
        return;
    }
}

/// HTTP/1.1 400 Bad Request ({ request.version() } { code })
/// Content-Type: application/json
/// Content-Length: 0 ({ strlen(message) })
/// Connection: close
///
/// { message }
void HTTP_connection::HTTP_connection_impl::send_response(std::string message,
                                         boost::beast::http::status code) {
    response.version(request.version());
    response.result(code);
    if (code == boost::beast::http::status::ok)
        response.set(boost::beast::http::field::content_type,
                "application/json");
    response.content_length(message.length());
    response.set(boost::beast::http::field::connection, "close");
    response.body() = message;

    boost::beast::http::async_write(
            socket,
            response,
            boost::bind(&HTTP_connection_impl::handle_write,
                    shared_from_this(),
                    boost::asio::placeholders::error));
}

void HTTP_connection::HTTP_connection_impl::serialize_updated_pixels(
        const std::vector<const PixelServer*>& updated_pixels) {
    std::stringstream ss;                       //
    boost::property_tree::ptree root;           // to serializer class?

    root.put("status", "OK");
    boost::property_tree::ptree field_root;
    for (auto i : updated_pixels) {
        boost::property_tree::ptree json;
        i->toBoostJson(json);

        field_root.push_back(std::make_pair("", json));
    }
    root.add_child("updated_pixels", field_root);

    boost::property_tree::write_json(ss, root);
    send_response(ss.str());
}

void HTTP_connection::HTTP_connection_impl::serialize_painted_pixel(const PixelServer& pixel,
                                                   bool success) {
    std::stringstream ss;
    boost::property_tree::ptree root;

    if (!success) {
        boost::property_tree::ptree json;
        pixel.toBoostJson(json);

        boost::property_tree::ptree pixel_root;
        root.put("status", "FAIL");
        pixel_root.put("color", json.get<unsigned short>("color"));
        pixel_root.put("unlock_time", json.get<time_t>("unlock_time"));
        root.add_child("failed_pixel", pixel_root);
    }
    else {
        root.put("status", "OK");
        root.put("unlock_time", pixel.getEditTime());
    }

    boost::property_tree::write_json(ss, root);
    send_response(ss.str());
}

void HTTP_connection::HTTP_connection_impl::serialize_all_field(
        const std::vector<const PixelServer*>& field) {
    std::stringstream ss;
    boost::property_tree::ptree root;

    root.put("status", "OK");
    boost::property_tree::ptree field_root;
    for (auto i : field) {
        boost::property_tree::ptree json;
        i->toBoostJson(json);

        boost::property_tree::ptree pixel_root;
        pixel_root.put("color", json.get<unsigned short>("color"));
        pixel_root.put("unlock_time", json.get<time_t>("unlock_time"));
        field_root.push_back(std::make_pair("", pixel_root));
    }
    root.add_child("field", field_root);

    boost::property_tree::write_json(ss, root);
    send_response(ss.str());
}


HTTP_connection::HTTP_connection(boost::asio::io_context& io_context) {
    p = new HTTP_connection_impl(io_context);
}

HTTP_connection::~HTTP_connection() {
    delete p;
}

void HTTP_connection::read_request() {
    p->read_request();
}

boost::asio::ip::tcp::socket& HTTP_connection::get_socket() {
    return p->get_socket();
}


