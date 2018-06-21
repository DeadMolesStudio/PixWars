#include <iostream>
#include <vector>

#include "TCP_server.hpp"
#include "pixel_server.hpp"

std::vector<PixelServer*> game_field; // to db

const unsigned short port = 8080;
const unsigned int field_size = 100; // to config

void initialize_field(std::vector<PixelServer*>& field);

int main(int argc, char** argv) {
    initialize_field(game_field);

    TCP_server server(port);
    server.run_server();

    return 0;
}

void initialize_field(std::vector<PixelServer*>& field) {
    for (unsigned int i = 0; i < field_size; i++) {
        field.push_back(new PixelServer(i));
    }
    std::cout << "Successfully initialized game field with " << field_size
              << " size." << std::endl;
}
