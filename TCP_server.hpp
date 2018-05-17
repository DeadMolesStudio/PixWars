#ifndef CPP_SERVER_SERVER_HPP
#define CPP_SERVER_SERVER_HPP

#include <boost/asio.hpp>

#include "HTTP_connection.hpp"


/// Asynchronous server with threads
class TCP_server {
 public:
    TCP_server(unsigned short port = 80);

    void run_server();

 private:
    const unsigned short port;

    /// The io_context object provides I/O services, such as sockets, that the server object will use.
    boost::asio::io_context io_context;
    /// The type of a TCP endpoint.
    boost::asio::ip::tcp::endpoint endpoint;
    /// Provides the ability to accept new connections (TCP).
    boost::asio::ip::tcp::acceptor acceptor;

    /// Creates a socket and initiates an asynchronous accept operation to wait for a new connection.
    void start_accept();
    /// Run the io_context object so that it will perform asynchronous operations on your behalf.
    void run();
    /// Is called when the asynchronous accept operation initiated by start_accept() finishes.
    /// It services the client request, and then calls start_accept() to initiate the next accept operation.
    void handle_accept(std::shared_ptr<HTTP_connection> connection,
                       const boost::system::error_code& e);
};

#endif //CPP_SERVER_SERVER_HPP
