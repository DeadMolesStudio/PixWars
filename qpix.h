#ifndef QPIX_H
#define QPIX_H

#include <QObject>
#include <QString>
#include <pixel.h>

class Qpix : public QObject, public PixelClient
{
    Q_OBJECT
    Q_PROPERTY(unsigned color READ getQColor WRITE setQColor NOTIFY colorChanged)
    Q_PROPERTY(bool blocked READ pixBlocked WRITE setBlock NOTIFY blockedChanged)
public:
    Qpix(QObject *parent = nullptr);
    explicit Qpix(unsigned int id, QObject *parent = nullptr);

    unsigned getQColor();
    QString getTime();

    void setQpix(const unsigned new_color);
    void setQColor(const unsigned new_color);
    void setBlock(const bool block);
    void setTime(const QString t);

    QString UnblockIn();
    void blockQpix();

signals:
    void colorChanged(unsigned color);
    void blockedChanged(bool blocked);
    void timeChanged(QString time);

public slots:

};

#endif // QPIX_H
