#include "pixwarsapp.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    PixWarsApp app(argc, argv);
    return app.execute();
}
