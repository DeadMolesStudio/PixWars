#ifndef CPP_SERVER_SERVER_HPP
#define CPP_SERVER_SERVER_HPP


/// Asynchronous server with threads
class TCP_server {
 public:
    TCP_server(unsigned short port = 80);
    ~TCP_server();

    void run_server();

 private:
    class TCP_server_impl;
    TCP_server_impl* p;
};

#endif //CPP_SERVER_SERVER_HPP
