#ifndef QPIX_H
#define QPIX_H

#include <QObject>
#include <QString>
#include <pixel.h>

class Qpix : public QObject, public PixelClient
{
    Q_OBJECT
    Q_PROPERTY(QString color READ getQColor WRITE setQpix NOTIFY colorChanged)
    Q_PROPERTY(bool blocked READ pixBlocked NOTIFY blockedChanged) //какое notify
    //Q_PROPERTY(unsigned int id READ id WRITE setId NOTIFY idChanged)
public:
    Qpix(QObject *parent = nullptr);
    explicit Qpix(unsigned int id, QObject *parent = nullptr);

    void blockQpix();
    QString getQColor();
    QString UnblockIn();
    void setQpix(const QString new_color);


signals:
    void colorChanged(QString color);
    void blockedChanged(bool blocked);

public slots:

};

#endif // QPIX_H
