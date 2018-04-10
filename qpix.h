#ifndef QPIX_H
#define QPIX_H

#include <QObject>
#include <QString>
#include <pixel.h>

class Qpix : public QObject, public PixelClient
{
    Q_OBJECT
    Q_PROPERTY(QString color READ getQColor WRITE setQpix NOTIFY colorChanged)
public:
    explicit Qpix(unsigned int id, QObject *parent = nullptr);

    void setQpix(const QString new_color);
    QString getQColor();

signals:
    void colorChanged(QString color);

public slots:

};

#endif // QPIX_H
