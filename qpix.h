#ifndef QPIX_H
#define QPIX_H

#include <QObject>
#include <QString>
#include <pixel.h>

class Qpix : public QObject, public PixelClient
{
    Q_OBJECT
    Q_PROPERTY(QString color READ getQColor WRITE setQColor NOTIFY colorChanged)
    Q_PROPERTY(bool blocked READ pixBlocked WRITE setBlock NOTIFY blockedChanged)
    //Q_PROPERTY(unsigned int id READ id WRITE setId NOTIFY idChanged)
public:
    Qpix(QObject *parent = nullptr);
    explicit Qpix(unsigned int id, QObject *parent = nullptr);

    QString getQColor();

    void setQpix(const QString new_color);
    void setQColor(const QString new_color);
    void setBlock(const bool block);

    QString UnblockIn();
    void blockQpix();

signals:
    void colorChanged(QString color);
    void blockedChanged(bool blocked);

public slots:

};

#endif // QPIX_H
