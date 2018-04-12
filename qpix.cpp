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
    std::cout << "  22  Qpix::setQpix(" << std::endl;
    PixelClient::setStrColor(new_color.toStdString());
    blockQpix();

    emit colorChanged(new_color);
}

void Qpix::blockQpix()
{
    PixelClient::setUnlockTime();
    emit blockedChanged(PixelClient::pixBlocked());
}

QString Qpix::getQColor()
{
    std::string col = PixelClient::getStrColor();
    QString qcol = QString::fromStdString(col);
    return qcol;
}

QString Qpix::UnblockIn()
{
    std::string str = PixelClient::leftTime();
    QString qstr = QString::fromStdString(str);
    return qstr;
}

void Qpix::setBlock(bool block)
{

    emit blockedChanged(block);
}


void Qpix::setQCol(QString new_color)
{
        PixelClient::setStrColor(new_color.toStdString());
        emit colorChanged(new_color);
}


