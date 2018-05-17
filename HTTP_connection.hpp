#ifndef CPP_SERVER_CONNECTION_HPP
#define CPP_SERVER_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <string>

#include "pixel_server.hpp"


/// Represents client connection
class HTTP_connection : public std::enable_shared_from_this<HTTP_connection> {
 public:
    HTTP_connection(boost::asio::io_context& io_context);

    boost::asio::ip::tcp::socket& get_socket();

    /// Read the data from the client
    void read_request();

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

    /// Serialize JSON for response
    void serialize_updated_pixels(
            const std::vector<const PixelServer*>& updated_pixels);
    void serialize_painted_pixel(const PixelServer& pixel, bool success);
    void serialize_all_field(const std::vector<const PixelServer*>& field);
};

#endif //CPP_SERVER_CONNECTION_HPP
