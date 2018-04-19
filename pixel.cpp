#include "pixel.h"

std::vector<std::string> list_color
        {
                "white", "red", "blue", "green", "black", "pink", "yellow", "cyan"
        };



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

std::string PixelServer::toJson()
{
    return " ";
}

void PixelServer::printPixel()
{
    std::cout << "SERVER pixel" << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << "color: " << color << std::endl;
    std::cout << "blocked: " << blocked << std::endl;
    std::cout << "edit_time: " << get_time(edit_time) << std::endl;
    std::cout << "------------" << std::endl;
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


std::string PixelClient::getStrColor() const
{
    return list_color[color];
}

bool PixelClient::pixBlocked() const
{
    time_t now;
    time(&now);

    //std:: cout << "PixelClient::pixBlocked()   ";
    time_t rest = get_difference(unlock_time);
    //std::cout << std::endl << "REST IS " << rest << std::endl;
    if ( rest == time(NULL) )
        return false;
    return true;
}

void PixelClient::setUnlockTime(const time_t time)
{
    if ( time == unlock_time )
        return;
     unlock_time = time;
}

void PixelClient::setUnlockTime()
{
    time_t now;
    time(&now);
    unlock_time = get_block_time(now);
}

void PixelClient::setStrColor(const std::string new_color)
{
    if ( new_color == list_color[color] )
        return;
    std::vector<std::string>::iterator it = std::find(list_color.begin(),list_color.end(), new_color);
    color = std::distance(list_color.begin(), it);
}

std::string PixelClient::leftTime() const
{
    time_t rest = get_difference(unlock_time);
    if ( rest == time(NULL) )
        return "почему 0";
    std::cout << "  PixelClient::leftTime()     " << rest << std::endl;
    std::string min = std::to_string(rest / 60);
    std::string sec = std::to_string(rest % 60);

    return min + ":" + sec;
}

std::string PixelClient::toJson()
{
    return " ";
}

void PixelClient::printPixel()
{
    std::cout << "CLIENT pixel" << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << "color: " << color << std::endl;
    std::cout << "unlock_time: " << get_time(unlock_time) << std::endl;
    std::cout << "------------" << std::endl;
}





