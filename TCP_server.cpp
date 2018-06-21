#include "TCP_server.hpp"


#include <boost/bind.hpp>
#include <iostream>
#include <functional>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

#include "HTTP_connection.hpp"
#include "logger.hpp"


/// Asynchronous server with threads
class TCP_server_impl {
 public:
    TCP_server_impl(unsigned short port = 80);

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

enum TCPConsts {
    max_connections = 1024,
    max_threads = 4,
};

TCP_server_impl::TCP_server_impl(unsigned short port)
        : port(port),
          endpoint(boost::asio::ip::tcp::v4(), port),
        // initialises an acceptor to listen on TCP port port
          acceptor(io_context, endpoint, /* reuse_addr = */ true) {}

void TCP_server_impl::run_server() {
    acceptor.listen(max_connections);

    start_accept();

    write_log("Run server");
    std::cout << "Run server on " << port << " port with "
              << max_connections << " max connections and "
              << max_threads << " max threads." << std::endl;

    std::vector<std::thread> threads;
    for (int i = 0; i < max_threads; i++)
        threads.emplace_back(std::thread(std::bind(&TCP_server_impl::run, this)));
    for (auto &thread : threads)
        thread.join();
}

void TCP_server_impl::handle_accept(std::shared_ptr<HTTP_connection> connection,
                                    const boost::system::error_code& error) {
    if (!error) {
        connection->read_request();
    }

    start_accept();
}

void TCP_server_impl::start_accept() {
    std::shared_ptr<HTTP_connection> connection
            (new HTTP_connection(acceptor.get_executor().context()));
    acceptor.async_accept(connection->get_socket(),
            boost::bind(&TCP_server_impl::handle_accept,
                    this, connection,
                    boost::asio::placeholders::error));
}

void TCP_server_impl::run() {
    io_context.run();
}

TCP_server::TCP_server(unsigned short port) {
    p = new TCP_server_impl(port);
}

TCP_server::~TCP_server() {
    delete p;
}

void TCP_server::run_server() {
    p->run_server();
}
