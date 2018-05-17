#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <string.h>
#include "time_date.h"
//#include "color.h"

#include <iterator>
#include <vector>

class Pixel
{
 public:
    Pixel();
    Pixel(unsigned int id, unsigned short color = 0);
    virtual ~Pixel();

    unsigned int getId() const;
    unsigned short getColor() const;

    void setColor(unsigned short new_color);

    virtual std::string toJson() const = 0;
    virtual void printPixel() const = 0;

 protected:
    unsigned int id;
    unsigned short color;
};

class PixelClient : public Pixel
{
 public:
    PixelClient(unsigned int id);
    ~PixelClient();

    time_t getUnlockTime() const;
    std::string getStrColor() const;
    bool pixBlocked() const;

    void setUnlockTime();
    void setUnlockTime(const time_t time);
    void setStrColor(const std::string new_color);

    std::string leftTime() const;

    std::string toJson() const;
    void printPixel() const;

 protected:
    time_t unlock_time;
};

#endif //PIXEL_H
