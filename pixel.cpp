#include "pixel.h"

Pixel::Pixel(unsigned int id) :
    id(id), color(0)
{
}

Pixel::~Pixel()
{
}

unsigned int Pixel::getId() const
{
    return id;
}

unsigned short Pixel::getColor() const
{
    return color;
}


void Pixel::setColor(const unsigned new_color)
{
    if ( new_color == color )
        return;
    color = new_color;
}

PixelServer::PixelServer(unsigned int id) :
    Pixel(id), blocked(0), edit_time(NULL)
{
}

PixelServer::~PixelServer()
{
}

bool PixelServer::isBlocked() const
{
    return blocked;
}

time_t PixelServer::getEditTime() const
{
    return edit_time;
}


PixelClient::PixelClient(unsigned int id) :
    Pixel(id), unlock_time(NULL)
{
}

PixelClient::~PixelClient()
{
}

time_t PixelClient::getUnlockTime() const
{
    return unlock_time;
}

bool PixelClient::pixBlocked() const
{
    time_t rest = get_difference(unlock_time);
    if ( rest == time(NULL) )
        return false;
    return true;
}

void PixelClient::setUnlockTime(const time_t time)
{
    unlock_time = time;
}

void PixelClient::setUnlockTime()
{
    time_t now;
    time(&now);
    unlock_time = get_block_time(now);
}

#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

std::string PixelClient::leftTime() const
{
    time_t rest = get_difference(unlock_time);
    if ( rest == time(NULL) )
        return "мгновение";
    std::cout << "  PixelClient::leftTime()     " << rest << std::endl;
    std::string min = patch::to_string(rest / 60);
    std::string sec = patch::to_string(rest % 60);

    return min + ":" + sec;
}






