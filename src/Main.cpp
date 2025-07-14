/**
 * @file main.cpp
 * @brief Entry point for the Flocking System application (Qt-based GUI).
 *
 * Initializes the Qt application and main window, with robust exception handling.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */
#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"

#ifdef _WIN32
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        MainWindow w;
        w.show();

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
