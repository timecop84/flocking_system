#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // make an instance to the application
    QApplication a(argc, argv);

    MainWindow w;

    // create a new main window
    //MainWindow w;

    // show the main window
    w.show();

    // start the Qt application
    return a.exec();
}
