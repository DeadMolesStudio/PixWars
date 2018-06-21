#include <boost/bind.hpp>
#include <iostream>
#include <functional>
#include <thread>
#include <vector>

#include "TCP_server.hpp"
#include "HTTP_connection.hpp"
#include "logger.hpp"


enum TCPConsts {
    max_connections = 1024,
    max_threads = 4,
};

TCP_server::TCP_server(unsigned short port)
    : port(port),
      endpoint(boost::asio::ip::tcp::v4(), port),
      // initialises an acceptor to listen on TCP port port
      acceptor(io_context, endpoint, /* reuse_addr = */ true) {}

void TCP_server::run_server() {
    acceptor.listen(max_connections);

    start_accept();

    write_log("Run server");
    std::cout << "Run server on " << port << " port with "
              << max_connections << " max connections and "
              << max_threads << " max threads." << std::endl;

    // running io_contexts in threads?
    std::vector<std::thread> threads;
    for (int i = 0; i < max_threads; i++)
        threads.emplace_back(std::thread(std::bind(&TCP_server::run, this)));
    for (auto &thread : threads)
        thread.join();
}

void TCP_server::handle_accept(std::shared_ptr<HTTP_connection> connection,
                               const boost::system::error_code& error) {
    if (!error) {
        connection->read_request();
    }

    start_accept();
}

void TCP_server::start_accept() {
    std::shared_ptr<HTTP_connection> connection
            (new HTTP_connection(acceptor.get_executor().context()));
    acceptor.async_accept(connection->get_socket(),
                          boost::bind(&TCP_server::handle_accept,
                                      this, connection,
                                      boost::asio::placeholders::error));
}

void TCP_server::run() {
    io_context.run();
}
