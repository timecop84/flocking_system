#include "NGLInit.h"
#include <iostream>

NGLInit* NGLInit::s_instance = nullptr;

NGLInit* NGLInit::instance() {
    if (!s_instance) {
        s_instance = new NGLInit();
    }
    return s_instance;
}

void NGLInit::initGlew() {
    std::cout << "NGLInit: initGlew() called (stub)" << std::endl;
}

void NGLInit::NGLQuit() {
    std::cout << "NGLInit: NGLQuit() called (stub)" << std::endl;
}
