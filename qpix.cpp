#include "qpix.h"
#include "QDebug"

Qpix::Qpix(QObject *parent) :
    QObject(parent), PixelClient(0)
{
}


Qpix::Qpix(unsigned int id, QObject *parent) :
    QObject(parent), PixelClient(id)
{
}


void Qpix::setQpix(const QString new_color)
{
    setQColor(new_color);
    blockQpix();
}


QString Qpix::getQColor()
{
    std::string col = PixelClient::getStrColor();
    QString qcol = QString::fromStdString(col);
    return qcol;
}

void Qpix::setQColor(const QString new_color)
{
        PixelClient::setStrColor(new_color.toStdString());
        emit colorChanged(new_color);
}

QString Qpix::UnblockIn()
{
    std::cout << "Qpix::UnblockIn()" << std::endl;
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
    std::cout << "void Qpix::setTime(const QString t)     " << std::endl;
    emit timeChanged(t);
}

QString Qpix::getTime()
{
    std::cout << "Qpix::getTime(" << std::endl;
    return UnblockIn();
}






