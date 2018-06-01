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
#include <QEventLoop>
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
{}

PixelTable::~PixelTable()
{}

void PixelTable::getField(QString url)
{
    time_t now;
    last_update = time(&now);
    qDebug() << "getField()";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray("application/json"));

    QJsonObject jsonRequest;
    jsonRequest.insert("command", "get_field");

    QNetworkReply* reply = manager->post(request, QJsonDocument(jsonRequest).toJson());

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    qDebug() << "after loop\n";
    if(reply->error() == QNetworkReply::NoError)
    {
        QString strReply = QString(reply->readAll()).remove("\n");
        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();
        if(jsonReply["status"].toString().toStdString() == "OK")
        {
            qDebug() << "ok";
            for(size_t i = 0; i < jsonReply["field"].toArray().size(); ++i)
            {
                int color = jsonReply["field"].toArray().at(i)["color"].toString().toInt();
                time_t time = time_t(jsonReply["field"].toArray().at(i)["unlock_time"].toString().toInt());
                add(i,color,time);
            }
            emit countChanged();
            emit dataChanged();
        }
        else
        {
            qDebug() << "not ok\n";
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" << reply->errorString();
        delete reply;
    }
    manager->deleteLater();
}

QQmlListProperty<Qpix> PixelTable::data()
{
    return QQmlListProperty<Qpix>(static_cast<QObject *>(this),
                                     static_cast<void *>(&listPixels),
                                     &PixelTable::appendData, &PixelTable::countData,
                                     &PixelTable::atData, &PixelTable::clearData);
}

void PixelTable::add(unsigned int id, const unsigned new_color, time_t unlock_time)
{
    Qpix *pix = new Qpix(id,this);
    pix->setProperty("color", new_color);
    bool block = false;
    if ( get_difference(unlock_time) >= time(NULL) )
        block = true;
    pix->setProperty("blocked", block);
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
bool PixelTable::setQpixById(QString url, unsigned int id, const unsigned new_color)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray("application/json"));

    QJsonObject jsonRequest;
    jsonRequest.insert("command", "paint_pixel");
    QJsonObject pixel;
    pixel.insert("id", patch::to_string(id).c_str());
    pixel.insert("color", patch::to_string(new_color).c_str());
    jsonRequest.insert("pixel_for_paint", pixel);
//    qDebug() << QJsonDocument(jsonRequest).toJson();
    QNetworkReply* reply = manager->post(request, QJsonDocument(jsonRequest).toJson());

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    bool res = false;
    if(reply->error() == QNetworkReply::NoError)
    {
        QString strReply = QString(reply->readAll());
        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();
        if(jsonReply["status"] == "OK")
        {
            setQpixById(id, new_color, time_t(jsonReply["unlock_time"].toString().toUInt()));
            res = true;
        }
        else
        {
            setQpixById(id,
                       jsonReply["failed_pixel"].toObject()["color"].toString().toInt(),
                       time_t(jsonReply["failed_pixel"].toObject()["unlock_time"].toString().toUInt()));
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" << reply->errorString();
        delete reply;
    }
    delete manager;
    return res;
}

void PixelTable::checkPixels(QString url)
{
    qDebug() << "checkPixels()\n";

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArray("application/json"));

    QJsonObject jsonRequest;
    jsonRequest.insert("command", "update_since_last_update");
    jsonRequest.insert("last_update", QJsonValue::fromVariant(QVariant(unsigned(last_update+35))));
    time_t now;
    last_update = time(&now);

    QNetworkReply* reply = manager->post(request, QJsonDocument(jsonRequest).toJson());
    connect(reply, SIGNAL(finished()), this, SLOT(checkPixelsSlot()));

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QString strReply = QString(reply->readAll());
        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();

        if(jsonReply["status"] == "OK")
        {
            size_t size = jsonReply["updated_pixels"].toArray().size();
            if(size > 0)
            {
                qDebug() << "chekpixels size" << size;
                for(size_t i = 0; i < size; ++i)
                {
                    QJsonValue current = jsonReply["updated_pixels"].toArray().at(i);
                    unsigned id = current["id"].toString().toInt();
                    int color = current["color"].toString().toInt();
                    time_t time = current["unlock_time"].toString().toUInt();
                    setQpixById(id, color, time);
                }
                emit dataChanged();
            }
        }
        else
        {
            qDebug() << "not ok";
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" << reply->errorString();
        delete reply;
    }
}


void PixelTable::setQpixById(unsigned int id, const unsigned new_color, time_t unlock_time)
{
    Qpix *pix = listPixels.at(id);
    pix->setProperty("color", new_color);
    bool block = false;
    if ( get_difference(unlock_time) >= time(NULL) )
        block = true;
    pix->setProperty("blocked", block);
    pix->setUnlockTime(unlock_time);
    emit pix->blockedChanged(pix->pixBlocked());
}


QString PixelTable::unblockQpixIn(unsigned int id)
{
    return QString(listPixels.at(id)->leftTime().c_str());
}

void PixelTable::checkPixelsSlot()
{
    qDebug() << "checkPixelsSlot()";
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
    if(reply->error() == QNetworkReply::NoError)
    {
        QString strReply = QString(reply->readAll());
        QJsonObject jsonReply = QJsonDocument::fromJson(strReply.toUtf8()).object();

        if(jsonReply["status"] == "OK")
        {
            size_t size = jsonReply["updated_pixels"].toArray().size();
            if(size > 0)
            {
                qDebug() << "chekpixels size" << size;
                for(size_t i = 0; i < size; ++i)
                {
                    QJsonValue current = jsonReply["updated_pixels"].toArray().at(i);
                    unsigned id = current["id"].toString().toInt();
                    int color = current["color"].toString().toInt();
                    time_t time = current["unlock_time"].toString().toUInt();
                    setQpixById(id, color, time);
                }
                emit dataChanged();
            }
        }
        else
        {
            qDebug() << "not ok";
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" << reply->errorString();
        delete reply;
    }
}

