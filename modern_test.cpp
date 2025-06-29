#include <iostream>
#include "include/FlockTypes.h"
#include "include/Random.h"
#include "include/ModernBoid.h"
#include "include/ModernExample.h"

int main() {
    std::cout << "=== Testing Modern Flocking System ===" << std::endl;
    
    // Test basic GLM functionality
    std::cout << "\n1. Testing GLM integration:" << std::endl;
    flock::Vec3 v1(1.0f, 2.0f, 3.0f);
    flock::Vec3 v2(4.0f, 5.0f, 6.0f);
    
    flock::Vec3 sum = v1 + v2;
    float length = flock::Utils::length(v1);
    float distance = flock::Utils::distance(v1, v2);
    
    std::cout << "v1 + v2 = (" << sum.x << ", " << sum.y << ", " << sum.z << ")" << std::endl;
    std::cout << "length(v1) = " << length << std::endl;
    std::cout << "distance(v1, v2) = " << distance << std::endl;
    
    // Test random functionality
    std::cout << "\n2. Testing Random generation:" << std::endl;
    flock::Random random;
    
    for (int i = 0; i < 3; ++i) {
        flock::Vec3 randomVec = random.randomVec3();
        std::cout << "Random vector " << i << ": (" << randomVec.x << ", " << randomVec.y << ", " << randomVec.z << ")" << std::endl;
    }
    
    // Test modern boid
    std::cout << "\n3. Testing Modern Boid:" << std::endl;
    Boid boid(flock::Vec3(0.0f, 0.0f, 0.0f), flock::Vec3(1.0f, 0.0f, 0.0f));
    
    std::cout << "Initial position: (" << boid.getPosition().x << ", " << boid.getPosition().y << ", " << boid.getPosition().z << ")" << std::endl;
    std::cout << "Initial velocity: (" << boid.getVelocity().x << ", " << boid.getVelocity().y << ", " << boid.getVelocity().z << ")" << std::endl;
    
    // Update boid
    boid.update();
    std::cout << "After update: (" << boid.getPosition().x << ", " << boid.getPosition().y << ", " << boid.getPosition().z << ")" << std::endl;
    
    // Test flocking example
    std::cout << "\n4. Testing Flocking Example:" << std::endl;
    flock::examples::demonstrateModernUsage();
    
    std::cout << "\n=== All tests completed successfully! ===" << std::endl;
    return 0;
}
