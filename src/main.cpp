#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // make an instance to the application
    QApplication a(argc, argv);

    try {
        MainWindow w;
        w.show();

        // start the Qt application
        return a.exec();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Error", QString("Exception: %1").arg(e.what()));
        return -1;
    }
    catch (...) {
        QMessageBox::critical(nullptr, "Error", "Unknown exception occurred");
        return -1;
    }
}
