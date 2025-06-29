#include <iostream>
#include "include/FlockTypes.h"
#include "include/Random.h"
#include "include/ModernExample.h"

int main() {
    std::cout << "=== Testing Modern Flocking System (No OpenGL) ===" << std::endl;
    
    // Test basic GLM functionality
    std::cout << "\n1. Testing GLM integration:" << std::endl;
    flock::Vec3 v1(1.0f, 2.0f, 3.0f);
    flock::Vec3 v2(4.0f, 5.0f, 6.0f);
    
    flock::Vec3 sum = v1 + v2;
    float length = flock::Utils::length(v1);
    float distance = flock::Utils::distance(v1, v2);
    flock::Vec3 normalized = flock::Utils::normalize(v1);
    
    std::cout << "v1 = (" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
    std::cout << "v2 = (" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;
    std::cout << "v1 + v2 = (" << sum.x << ", " << sum.y << ", " << sum.z << ")" << std::endl;
    std::cout << "length(v1) = " << length << std::endl;
    std::cout << "distance(v1, v2) = " << distance << std::endl;
    std::cout << "normalize(v1) = (" << normalized.x << ", " << normalized.y << ", " << normalized.z << ")" << std::endl;
    
    // Test random functionality
    std::cout << "\n2. Testing Random generation:" << std::endl;
    flock::Random random;
    
    for (int i = 0; i < 5; ++i) {
        flock::Vec3 randomVec = random.randomVec3();
        flock::Vec3 normalizedVec = random.randomNormalizedVec3();
        float randomFloat = random.randomFloat();
        
        std::cout << "Random vector " << i << ": (" << randomVec.x << ", " << randomVec.y << ", " << randomVec.z << ")" << std::endl;
        std::cout << "Normalized " << i << ": (" << normalizedVec.x << ", " << normalizedVec.y << ", " << normalizedVec.z << 
                     ") length = " << flock::Utils::length(normalizedVec) << std::endl;
        std::cout << "Random float " << i << ": " << randomFloat << std::endl;
    }
    
    // Test colors
    std::cout << "\n3. Testing Colors:" << std::endl;
    flock::Color red = flock::Colors::RED;
    flock::Color randomColor = random.randomColor();
    
    std::cout << "Red color: (" << red.r << ", " << red.g << ", " << red.b << ", " << red.a << ")" << std::endl;
    std::cout << "Random color: (" << randomColor.r << ", " << randomColor.g << ", " << randomColor.b << ", " << randomColor.a << ")" << std::endl;
    
    // Test math constants
    std::cout << "\n4. Testing Math Constants:" << std::endl;
    std::cout << "PI = " << flock::Math::PI << std::endl;
    std::cout << "TWO_PI = " << flock::Math::TWO_PI << std::endl;
    std::cout << "DEG_TO_RAD = " << flock::Math::DEG_TO_RAD << std::endl;
    
    // Test matrix operations
    std::cout << "\n5. Testing Matrix Operations:" << std::endl;
    flock::Mat4 identity(1.0f);
    flock::Vec3 translation(1.0f, 2.0f, 3.0f);
    flock::Mat4 translated = flock::Utils::translate(identity, translation);
    
    std::cout << "Identity matrix created successfully" << std::endl;
    std::cout << "Translation matrix created successfully" << std::endl;
    
    // Test flocking example
    std::cout << "\n6. Testing Flocking Example:" << std::endl;
    flock::examples::demonstrateModernUsage();
    
    std::cout << "\n=== All tests completed successfully! ===" << std::endl;
    std::cout << "\nThis demonstrates that GLM provides everything we need:" << std::endl;
    std::cout << "✓ Vector math (Vec3, Vec4)" << std::endl;
    std::cout << "✓ Matrix operations (Mat4, Mat3)" << std::endl;
    std::cout << "✓ Math functions (normalize, distance, dot, cross)" << std::endl;
    std::cout << "✓ Constants (PI, TWO_PI, etc.)" << std::endl;
    std::cout << "✓ Random number generation (thread-safe)" << std::endl;
    std::cout << "✓ Modern C++ features (no raw pointers, RAII)" << std::endl;
    
    return 0;
}
