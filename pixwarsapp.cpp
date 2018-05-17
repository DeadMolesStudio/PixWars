#include "pixwarsapp.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "pixeltable.h"


PixWarsApp::PixWarsApp(int argc, char *argv[]) : app(argc, argv)
{
//    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    qmlRegisterType<PixelTable>("pixel_model", 1, 0, "QPixelTable");
    qmlRegisterType<Qpix>("pixel_model", 1, 0, "QPixel");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
}

int PixWarsApp::execute()
{
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

PixWarsApp::~PixWarsApp()
{

}

