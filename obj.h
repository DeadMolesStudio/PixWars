#ifndef OBJ_H
#define OBJ_H

#include <QObject>
#include "pixel.h"

class obj : public QObject, PixelClient
{
    Q_OBJECT
public:
    explicit obj(unsigned int id, QObject *parent = nullptr);

    void setPixel(const std::string new_color);

signals:

public slots:
};

#endif // OBJ_H
