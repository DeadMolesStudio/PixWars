#ifndef CPP_SERVER_CONNECTION_HPP
#define CPP_SERVER_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>

#include "pixel_server.hpp"


/// Represents client connection
class HTTP_connection : public std::enable_shared_from_this<HTTP_connection> {
 public:
    HTTP_connection(boost::asio::io_context& io_context);
    ~HTTP_connection();

    boost::asio::ip::tcp::socket& get_socket();

    /// Read the data from the client
    void read_request();

 private:
    class HTTP_connection_impl;
    HTTP_connection_impl* p;

    /// Serialize JSON for response
    void serialize_updated_pixels(
            const std::vector<const PixelServer*>& updated_pixels);
    void serialize_painted_pixel(const PixelServer& pixel, bool success);
    void serialize_all_field(const std::vector<const PixelServer*>& field);
};

#endif //CPP_SERVER_CONNECTION_HPP
