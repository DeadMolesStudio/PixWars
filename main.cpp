#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "pixeltable.h"
#include "testpix.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<PixelTable>("pixel_model", 1, 0, "QPixelTable");
    qmlRegisterType<Qpix>("pixel_model", 1, 0, "QPixel");
    qmlRegisterType<testPix>("pixel_model", 1, 0, "QTestPix");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
