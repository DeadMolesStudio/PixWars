#ifndef CPP_SERVER_LOGGER_HPP
#define CPP_SERVER_LOGGER_HPP

#include <string>


void write_log(const std::string& message,
               const std::string& filename = "server_log.txt");

#endif //CPP_SERVER_LOGGER_HPP
