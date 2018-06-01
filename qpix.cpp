#include "qpix.h"
#include "QDebug"

Qpix::Qpix(QObject *parent) :
    QObject(parent), PixelClient(0)
{}

Qpix::Qpix(unsigned int id, QObject *parent) :
    QObject(parent), PixelClient(id)
{}

void Qpix::setQpix(const unsigned new_color)
{
    setQColor(new_color);
}

unsigned Qpix::getQColor()
{
    return color;
}

void Qpix::setQColor(const unsigned new_color)
{
        PixelClient::setColor(new_color);
        emit colorChanged(new_color);
}

QString Qpix::UnblockIn()
{
    std::string str = PixelClient::leftTime();
    QString qstr = QString::fromStdString(str);
    return qstr;
}

void Qpix::blockQpix()
{
    PixelClient::setUnlockTime();
    setBlock(true);
}

void Qpix::setBlock(const bool block)
{
    emit blockedChanged(block);
}

void Qpix::setTime(const QString t)
{
    emit timeChanged(t);
}

QString Qpix::getTime()
{
    return UnblockIn();
}






