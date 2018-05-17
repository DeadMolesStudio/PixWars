#ifndef PIXWARSAPP_H
#define PIXWARSAPP_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>

class PixWarsApp
{
public:
    PixWarsApp(int argc, char *argv[]);
    ~PixWarsApp();

    int execute();
private:
    QGuiApplication app;
    QQmlApplicationEngine engine;
};

#endif // PIXWARSAPP_H
