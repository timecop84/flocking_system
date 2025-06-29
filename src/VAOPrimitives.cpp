#include "VAOPrimitives.h"
#include <iostream>

VAOPrimitives* VAOPrimitives::s_instance = nullptr;

VAOPrimitives* VAOPrimitives::instance() {
    if (!s_instance) {
        s_instance = new VAOPrimitives();
    }
    return s_instance;
}

void VAOPrimitives::createSphere(const std::string& name, float radius, int precision) {
    std::cout << "VAOPrimitives: Creating sphere '" << name << "' radius=" << radius << " precision=" << precision << std::endl;
}

void VAOPrimitives::draw(const std::string& name) {
    std::cout << "VAOPrimitives: Drawing '" << name << "'" << std::endl;
}
