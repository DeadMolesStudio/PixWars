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
    Pixel(unsigned int id);
    virtual ~Pixel();

    unsigned int getId() const;
    unsigned short getColor() const;

    void setColor(const unsigned new_color);

    virtual std::string toJson() = 0;
    virtual void printPixel() = 0;

protected:
    unsigned int id;
    unsigned short color;
};


class PixelServer : public Pixel
{
public:
    PixelServer(unsigned int id);
    ~PixelServer();

    bool isBlocked() const;
    time_t getEditTime() const;

    std::string toJson();
    void printPixel();

protected:
    bool blocked;
    time_t edit_time;
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

    std::string toJson();
    void printPixel();

protected:
    time_t unlock_time;
};

#endif //PIXEL_H
