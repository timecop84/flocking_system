#include <QApplication>
#include <iostream>

// Include gradually to isolate the problem
#include "modules/math/include/MathUtils.h"
#include "modules/graphics/include/ShaderManager.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    std::cout << "Testing math + graphics modules..." << std::endl;
    math::Vector v(1.0f, 2.0f, 3.0f);
    std::cout << "Math module works: " << v.m_x << ", " << v.m_y << ", " << v.m_z << std::endl;
    std::cout << "Graphics module header included successfully" << std::endl;
    
    return 0;
}
