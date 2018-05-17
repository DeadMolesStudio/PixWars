#include <boost/beast.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "HTTP_connection.hpp"
#include "pixel_server.hpp"
#include "server_models.hpp"


HTTP_connection::HTTP_connection(boost::asio::io_context& io_context)
    : socket(io_context) {}

boost::asio::ip::tcp::socket& HTTP_connection::get_socket() {
    return socket;
}

void HTTP_connection::read_request() {
    /// read the request
    boost::beast::http::async_read(
        socket,
        buffer,
        request,
        boost::bind(&HTTP_connection::handle_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void HTTP_connection::handle_read(const boost::system::error_code& error,
                                 size_t bytes_transferred) {
    // handle the error
    if (error)
        return;

    if (request.method() != boost::beast::http::verb::post) {
        send_response("Bad method.\n",
                boost::beast::http::status::method_not_allowed);
        return;
    }
    if (!request.has_content_length()) {
        send_response("No content-length given.\n",
                boost::beast::http::status::length_required);
        return;
    } // more errors...

    // parsing the request
    std::stringstream ss;
    std::string body = request.body();
    ss << body;
    boost::property_tree::ptree root;
    try {
        boost::property_tree::read_json(ss, root);
        auto command = root.get<std::string>("command");
        if (command == "update_field") {
            std::vector<unsigned int> pixels;
            for (boost::property_tree::ptree::value_type& value
                    : root.get_child("pixels_for_update")) {
                if (!value.first.empty())
                    throw std::invalid_argument
                            ("Wrong format of update_field command got.\n");
                pixels.push_back(value.second.get_value<unsigned int>());
            }

            update_field(pixels,
                         boost::bind(&HTTP_connection::serialize_updated_pixels,
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
                        boost::bind(&HTTP_connection::serialize_painted_pixel,
                                    shared_from_this(),
                                    boost::placeholders::_1,
                                    boost::placeholders::_2));
        }
        else if (command == "get_field") { // TODO: список команд? map?
            get_field(boost::bind(&HTTP_connection::serialize_all_field,
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

void HTTP_connection::handle_write (const boost::system::error_code& error) {
    // handle error
    if (error)
        return;
}

void HTTP_connection::send_response(std::string message,
                                    boost::beast::http::status code) {
    /// HTTP/1.1 400 Bad Request ({ request.version() } { code })
    /// Content-Type: application/json
    /// Content-Length: 0 ({ strlen(message) })
    /// Connection: close
    ///
    /// { message }
    // response = {};
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
            boost::bind(&HTTP_connection::handle_write,
                        shared_from_this(),
                        boost::asio::placeholders::error));
}

void HTTP_connection::serialize_updated_pixels(
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

void HTTP_connection::serialize_painted_pixel(const PixelServer& pixel,
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

void HTTP_connection::serialize_all_field(
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
