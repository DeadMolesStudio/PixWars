#ifndef CPP_SERVER_PIXEL_SERVER_HPP
#define CPP_SERVER_PIXEL_SERVER_HPP

#include <boost/property_tree/ptree.hpp>
#include <string>

#include "pixel.h"


class PixelServer : public Pixel {
 public:
    PixelServer(unsigned int id, unsigned short color = 0, bool blocked = false,
                time_t edit_time = 0);

    bool isBlocked() const;
    void setBlockedStatus(bool status = true);

    time_t getEditTime() const;
    void setEditTime(time_t edit_time);

    std::string toJson() const override;
    void toBoostJson(boost::property_tree::ptree& root) const;
    void printPixel() const override;

 protected:
    bool blocked;
    time_t edit_time;
};


#endif //CPP_SERVER_PIXEL_SERVER_HPP
