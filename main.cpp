#include "convolution.h"

#include <QApplication>
#include <QtPlugin>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Convolution w;
    w.show();
    return a.exec();
}
