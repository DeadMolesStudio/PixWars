#include <QCoreApplication>
#include <iostream>
#include "pixel.h"
#include <qpix.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PixelClient p(3);
    p.setStrColor("blue");
    p.printPixel();
    std::cout << "poka" << std::endl;

    Qpix pp(8);

    QString s = "red";
    pp.setQpix(s);
    pp.printPixel();

    return a.exec();
}
