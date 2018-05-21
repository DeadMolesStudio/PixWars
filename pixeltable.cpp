#include "pixeltable.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <string>
#include <fstream>

#include <QVariant>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>

#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

PixelTable::PixelTable(QObject *parent) :
    QObject(parent)
{
//    for( int i = 0; i < 729; i++)
//    {
//        add(i);
//    }
}

void PixelTable::getField(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished()), this, SLOT(getPixelsSlot()));

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QJsonObject jsonRequest;
    jsonRequest.insert("command", "get_field");
    manager->post(request, QJsonDocument(jsonRequest).toJson());
    manager->deleteLater();

//    if(reply->error() == QNetworkReply::NoError)
//    {
//        QString strReply = QString(reply->readAll());
//        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();

//        if(jsonReply["status"] == "OK")
//        {
//            for(size_t i = 0; i < jsonReply["field"].toArray().size(); ++i)
//            {
//                add(i);
//                int color = jsonReply["field"].toArray().at(i)["color"].toInt();
//                time_t time = time_t(jsonReply["field"].toArray().at(i)["unlock_time"].toVariant().toUInt());
//                setQpixById(i, color, time);
//            }
//        }
//        delete reply;
//    }
//    else
//    {
//        qDebug() << "Failure" << reply->errorString();
//        delete reply;
//    }
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
    pix->setProperty("color", pix->getQColor());
    pix->setProperty("blocked", false);
    //pix->setProperty("time","разблокировано");
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


int PixelTable::count() const
{
    return listPixels.size();
}

//этот метод теперь для операции paint_pixel
void PixelTable::setQpixById(QString url, unsigned int id, const unsigned new_color)
{
    std::cout << "void PixelTable::setQpixById" << std::endl;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished()), this, SLOT(paintPixelSlot()));

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QJsonObject jsonRequest;
    jsonRequest.insert("command", "paint_pixel");
    QJsonObject pixel;
    pixel.insert("id", patch::to_string(id).c_str());
    pixel.insert("color", patch::to_string(new_color).c_str());
    jsonRequest.insert("pixel_for_paint", pixel);
    manager->post(request, QJsonDocument(jsonRequest).toJson());
    manager->deleteLater();

//    if(reply->error() == QNetworkReply::NoError)
//    {
//        QString strReply = QString(reply->readAll());
//        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();

//        if(jsonReply["status"] == "OK")
//        {
//            listPixels.at(id)->setQpix(new_color);
//            listPixels.at(id)->setUnlockTime(time_t(jsonReply["unlock_time"].toVariant().toUInt()));
//        }
//        else
//        {
//            listPixels.at(id)->setQpix(jsonReply["failed_pixel"].toObject()["color"].toInt());
//            listPixels.at(id)->setUnlockTime(time_t(jsonReply["failed_pixel"].toObject()["unlock_time"].toVariant().toUInt()));
//            //TODO:: сделал типо сигнал чтоб показывать что тебя опередили и это фиаско
//        }
//        delete reply;
//    }
//    else
//    {
//        qDebug() << "Failure" << reply->errorString();
//        delete reply;
//    }
}

void PixelTable::checkPixels(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished()), this, SLOT(updatePixelsSlot()));

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QJsonObject jsonRequest;
    jsonRequest.insert("command", "update_field");
    QJsonArray pixels;
    for(size_t i = 0; i < this->count(); i++)
    {
        if(!listPixels[i]->pixBlocked())
        {
            pixels.append(patch::to_string(i).c_str());
        }
    }
    jsonRequest.insert("pixels_for_update", pixels);
    manager->post(request, QJsonDocument(jsonRequest).toJson());
    manager->deleteLater();
//    if(reply->error() == QNetworkReply::NoError)
//    {
//        QString strReply = QString(reply->readAll());
//        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();

//        if(jsonReply["status"] == "OK")
//        {
//            for(int i = 0; i < jsonReply["field"].toArray().size(); ++i)
//            {
//                setQpixById(jsonReply["updated_pixels"].toArray().at(i)["id"].toInt(),
//                            jsonReply["updated_pixels"].toArray().at(i)["color"].toInt(),
//                            time_t(jsonReply["updated_pixels"].toArray().at(i)["unlock_time"].toVariant().toUInt()));
//            }
//        }
//        delete reply;
//    }
//    else
//    {
//        qDebug() << "Failure" << reply->errorString();
//        delete reply;
//    }
}


void PixelTable::setQpixById(unsigned int id, const unsigned new_color, time_t unlock_time)
{
    listPixels.at(id)->setQpix(new_color);
    listPixels.at(id)->setUnlockTime(unlock_time);
}


QString PixelTable::unblockQpixIn(unsigned int id)
{
    return listPixels.at(id)->UnblockIn();
}

void PixelTable::getPixelsSlot()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if(reply->error() == QNetworkReply::NoError)
    {
        QString strReply = QString(reply->readAll());
        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();

        if(jsonReply["status"] == "OK")
        {
            for(size_t i = 0; i < jsonReply["field"].toArray().size(); ++i)
            {
                add(i);
                int color = jsonReply["field"].toArray().at(i)["color"].toInt();
                time_t time = time_t(jsonReply["field"].toArray().at(i)["unlock_time"].toVariant().toUInt());
                setQpixById(i, color, time);
            }
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" << reply->errorString();
        delete reply;
    }
}

void PixelTable::paintPixelSlot()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QNetworkRequest *request = qobject_cast<QNetworkRequest *>(sender());

    if(reply->error() == QNetworkReply::NoError)
    {
        QString strReply = QString(reply->readAll());
        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();

        if(jsonReply["status"] == "OK")
        {
            listPixels.at(id)->setQpix(new_color);
            listPixels.at(id)->setUnlockTime(time_t(jsonReply["unlock_time"].toVariant().toUInt()));
        }
        else
        {
            listPixels.at(id)->setQpix(jsonReply["failed_pixel"].toObject()["color"].toInt());
            listPixels.at(id)->setUnlockTime(time_t(jsonReply["failed_pixel"].toObject()["unlock_time"].toVariant().toUInt()));
            //TODO:: сделал типо сигнал чтоб показывать что тебя опередили и это фиаско
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" << reply->errorString();
        delete reply;
    }

}

void PixelTable::updatePixelsSlot()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if(reply->error() == QNetworkReply::NoError)
    {
        QString strReply = QString(reply->readAll());
        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();

        if(jsonReply["status"] == "OK")
        {
            for(int i = 0; i < jsonReply["field"].toArray().size(); ++i)
            {
                setQpixById(jsonReply["updated_pixels"].toArray().at(i)["id"].toInt(),
                            jsonReply["updated_pixels"].toArray().at(i)["color"].toInt(),
                            time_t(jsonReply["updated_pixels"].toArray().at(i)["unlock_time"].toVariant().toUInt()));
            }
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" << reply->errorString();
        delete reply;
    }
}



