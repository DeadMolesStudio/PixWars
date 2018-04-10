#include "obj.h"

obj::obj(unsigned int id, QObject *parent) :
    QObject(parent), PixelClient(id)
{
}


void obj::setPixel(const std::string new_color)
{
    PixelClient::setUnlockTime();
    PixelClient::setStrColor(new_color);
}
