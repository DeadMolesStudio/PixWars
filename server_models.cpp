#include <ctime>
#include <exception>
#include <functional>
#include <vector>

#include "pixel_server.hpp"


extern std::vector<PixelServer*> game_field;

void update_field(std::vector<unsigned int>& pixels,
                  const std::function<void(const std::vector<const PixelServer*>&)>&
                      serialize_updated_pixels) {
    // get pixels for update
    std::vector<const PixelServer*> updated_pixels;
    for (auto i : pixels) {
        if (i >= game_field.size()) // fixme: never works
            throw std::invalid_argument("Wrong pixel id for update_field");
        updated_pixels.push_back(game_field[i]);
    }
    // callback
    serialize_updated_pixels(updated_pixels);
}

void paint_pixel(unsigned int id,
                 unsigned short color,
                 const std::function<void(const PixelServer&, bool)>&
                     serialize_painted_pixel) {
    PixelServer* painted_pixel = game_field[id]; // todo: database request
    time_t now;
    time(&now); // get real server time

    bool was_painted = false;
    if (!painted_pixel->isBlocked()) {
        painted_pixel->setBlockedStatus(); // pseudo-transaction
        if (painted_pixel->getEditTime() < now) {
            painted_pixel->setColor(color);
            painted_pixel->setEditTime(now + 10); // to config
            was_painted = true;
        }
        painted_pixel->setBlockedStatus(false);
    }

    // callback
    serialize_painted_pixel(*painted_pixel, was_painted);
}

void get_field(const std::function<void(const std::vector<const PixelServer*>&)>&
                       serialize_all_field) {
    // get all field
    std::vector<const PixelServer*> field;
    for (auto i : game_field) {
        field.push_back(i);
    }

    // callback
    serialize_all_field(field);
}
