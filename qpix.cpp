#include "qpix.h"

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
    PixelClient::setUnlockTime();
    PixelClient::setStrColor(new_color.toStdString());
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

