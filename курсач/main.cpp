#include "mainwindow.h"
#include <QApplication>
//#include "header.h"
#include <QDebug>
#include <QtGlobal>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //qInstallMessageHandler(myMessageOutput);
    MainWindow w;
    w.show();

    return a.exec();
}
