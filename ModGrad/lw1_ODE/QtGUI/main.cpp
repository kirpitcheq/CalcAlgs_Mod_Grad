#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    ODE_KPeQ::some((ODE_KPeQ::Array)10);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
