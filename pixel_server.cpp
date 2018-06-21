#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "pixel_server.hpp"


PixelServer::PixelServer(unsigned int id, unsigned short color, bool blocked,
                         time_t edit_time)
        : Pixel(id, color),
          blocked(blocked),
          edit_time(edit_time) {}

bool PixelServer::isBlocked() const {
    return blocked;
}

void PixelServer::setBlockedStatus(bool status) {
    blocked = status;
}

time_t PixelServer::getEditTime() const {
    return edit_time;
}

void PixelServer::setEditTime(time_t edit_time) {
    this->edit_time = edit_time;
}

std::string PixelServer::toJson() const {
    boost::property_tree::ptree root;
    toBoostJson(root);

    std::stringstream ss;
    boost::property_tree::write_json(ss, root);

    return ss.str();
}

void PixelServer::toBoostJson(boost::property_tree::ptree& root) const {
    root.put("id", id);
    root.put("color", color);
    root.put("unlock_time", edit_time);
}

void PixelServer::printPixel() const {
    std::cout << "SERVER pixel" << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << "color: " << color << std::endl;
    std::cout << "blocked: " << blocked << std::endl;
    std::cout << "edit_time: " << get_time(edit_time) << std::endl;
    std::cout << "------------" << std::endl;
}
