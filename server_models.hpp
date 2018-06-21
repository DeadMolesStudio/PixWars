#ifndef CPP_SERVER_SERVER_MODELS_HPP
#define CPP_SERVER_SERVER_MODELS_HPP

#include <functional>
#include <vector>

#include "pixel_server.hpp"


/// Sends only pixels mentioned in the list
void update_field(std::vector<unsigned int>& pixels,
                  const std::function<void(const std::vector<const PixelServer*>&)>&
                          serialize_updated_pixels);
/// Sends only pixels with unlocking time greater than the last update time
void update_since_last_update(time_t last_update,
                              const std::function<void(const std::vector<const PixelServer*>&)>&
                              serialize_updated_pixels);
/// Paints pixel if it's unlocked
void paint_pixel(unsigned int id,
                 unsigned short color,
                 const std::function<void(const PixelServer&, bool)>&
                         serialize_painted_pixel);
/// Sends all field pixels to user
void get_field(
        const std::function<void(const std::vector<const PixelServer*>&)>&
                       serialize_all_field);

#endif //CPP_SERVER_SERVER_MODELS_HPP
