#include "pixeltable.h"

PixelTable::PixelTable(QObject *parent) :
    QObject(parent)
{
//    Qpix *pix = new Qpix(id,this);
//    pix->setProperty("color", "white");
//    listPixels << pix;
}

QQmlListProperty<Qpix> PixelTable::data()
{
    return QQmlListProperty<Qpix>(static_cast<QObject *>(this),
                                     static_cast<void *>(&listPixels),
                                     &PixelTable::appendData, &PixelTable::countData,
                                     &PixelTable::atData, &PixelTable::clearData);
}

void PixelTable::add(unsigned int id)
{
    Qpix *pix = new Qpix(id,this);
    pix->setProperty("color", "white");
    listPixels.append(pix);

    emit dataChanged();
}


void PixelTable::appendData(QQmlListProperty<Qpix> *list, Qpix *value)
{
    QList<Qpix*> *data = static_cast<QList<Qpix*> *>(list->data);
    data->append(value);
}

int PixelTable::countData(QQmlListProperty<Qpix> *list)
{
    QList<Qpix*> *data = static_cast<QList<Qpix*> *>(list->data);
    return data->size();
}

void PixelTable::clearData(QQmlListProperty<Qpix> *list)
{
    QList<Qpix*> *data = static_cast<QList<Qpix*> *>(list->data);
    qDeleteAll(data->begin(), data->end());
    data->clear();
}

Qpix *PixelTable::atData(QQmlListProperty<Qpix> *list, int index)
{
    QList<Qpix*> *data = static_cast<QList<Qpix*> *>(list->data);
    return data->at(index);
}







