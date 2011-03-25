#include "create.h"
#include "interface.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Interface w;
       w.resize(480, 480);
       w.show();
    return a.exec();
}
