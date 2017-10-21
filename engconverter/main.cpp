#include <QtGui/QApplication>
#include "engconverter.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    EngConverter engConverter;
    engConverter.show();
    return app.exec();
}
