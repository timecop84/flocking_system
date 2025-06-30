/**
 * Example: Using Modular FlockLib Components
 * 
 * This example demonstrates how to use the simplified modular
 * components without the wrapper layers.
 */

// Include individual modules directly (recommended approach)
#include "modules/math/include/MathUtils.h"
#include "modules/graphics/include/Graphics3D.h"
#include "modules/utils/include/FlockUtils.h"

#include <iostream>
#include <vector>

int main() {
    std::cout << "=== Modular FlockLib Demo ===" << std::endl;
    
    // === Math Module Demo ===
    std::cout << "\n=== Math Module Demo ===" << std::endl;
    
    // Using the simplified math namespace
    math::Vec3 pos1(1.0f, 2.0f, 3.0f);
    math::Vec3 pos2(4.0f, 5.0f, 6.0f);
    
    std::cout << "Vector 1: (" << pos1.m_x << ", " << pos1.m_y << ", " << pos1.m_z << ")" << std::endl;
    std::cout << "Vector 2: (" << pos2.m_x << ", " << pos2.m_y << ", " << pos2.m_z << ")" << std::endl;
    
    // Vector operations
    math::Vec3 sum = pos1 + pos2;
    std::cout << "Sum: (" << sum.m_x << ", " << sum.m_y << ", " << sum.m_z << ")" << std::endl;
    
    float distance = pos1.distance(pos2);
    std::cout << "Distance between vectors: " << distance << std::endl;
    
    // Math utilities
    float clamped = math::utils::clamp(15.0f, 0.0f, 10.0f);
    std::cout << "Clamped 15 to range [0,10]: " << clamped << std::endl;
    
    // Math constants
    std::cout << "PI constant: " << math::PI << std::endl;
    
    // === Graphics Module Demo ===
    std::cout << "\n=== Graphics Module Demo ===" << std::endl;
    
    // Using the Color class
    graphics3d::Color red(1.0f, 0.0f, 0.0f, 1.0f);
    graphics3d::Color blue(0.0f, 0.0f, 1.0f, 1.0f);
    
    std::cout << "Red color: R=" << red.m_r << " G=" << red.m_g << " B=" << red.m_b << std::endl;
    std::cout << "Blue color: R=" << blue.m_r << " G=" << blue.m_g << " B=" << blue.m_b << std::endl;
    
    // Using the Camera class (basic setup)
    math::Vec3 from(0, 0, 10);
    math::Vec3 to(0, 0, 0);
    math::Vec3 up(0, 1, 0);
    
    Camera camera(from, to, up, flock::graphics::PERSPECTIVE);
    camera.setShape(45.0f, 16.0f/9.0f, 0.1f, 100.0f, flock::graphics::PERSPECTIVE);
    
    std::cout << "Camera configured with perspective projection" << std::endl;
    std::cout << "Camera eye position: (" << camera.getEye().m_x << ", " 
              << camera.getEye().m_y << ", " << camera.getEye().m_z << ")" << std::endl;
    
    // === Using TransformStack ===
    TransformStack transforms;
    transforms.identity();
    transforms.translate(5.0f, 0.0f, 0.0f);
    transforms.rotate(45.0f, 0.0f, 1.0f, 0.0f);
    
    std::cout << "Transform stack configured with translation and rotation" << std::endl;
    
    // === Using Light System ===
    Light mainLight(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    mainLight.setAttenuation(1.0f, 0.1f, 0.01f);
    
    std::cout << "Light configured at position (10, 10, 10) with white color" << std::endl;
    std::cout << "Light attenuation: constant=1.0, linear=0.1, quadratic=0.01" << std::endl;
    
    // === Using ShaderLib ===
    ShaderLib* shaderLib = ShaderLib::instance();
    shaderLib->createShaderProgram("basic");
    
    std::cout << "ShaderLib initialized with basic shader program" << std::endl;
    
    // === Utils Module Demo ===
    std::cout << "\n=== Utils Module Demo ===" << std::endl;
    
    // Note: PerformanceMonitor would need an actual OpenGL context to work properly
    std::cout << "PerformanceMonitor is available for timing operations" << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "FlockLib modules are ready for use in your projects!" << std::endl;
    
    return 0;
}
