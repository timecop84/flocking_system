#include <iostream>
#include <cstddef>

// Simple vec3 and vec4 for testing
struct vec3 {
    float x, y, z;
};

struct vec4 {
    float x, y, z, w;
};

// Copy of the GPU struct from your code
struct GPUBoidData {
    vec3 position;
    float padding1;
    vec3 velocity;
    float padding2;
    vec3 acceleration;
    float padding3;
    vec4 color;
    vec3 lastPosition;
    float padding4;
    
    GPUBoidData() : padding1(0.0f), padding2(0.0f), padding3(0.0f), padding4(0.0f) {}
};

struct FlockingParameters {
    float separationDistance;
    float alignmentDistance;
    float cohesionDistance;
    float separationForce;
    float alignmentForce;
    float cohesionForce;
    float maxSpeed;
    float maxForce;
    float deltaTime;
    float speedMultiplier;
    int numBoids;
    float randomSeed;
    vec3 boundingBoxMin;
    vec3 boundingBoxMax;
};

int main() {
    std::cout << "=== STRUCT ALIGNMENT DEBUG ===" << std::endl;
    std::cout << "GPUBoidData struct size: " << sizeof(GPUBoidData) << " bytes" << std::endl;
    std::cout << "FlockingParameters struct size: " << sizeof(FlockingParameters) << " bytes" << std::endl;
    std::cout << "Expected by shader: 80 bytes for both structures" << std::endl;
    std::cout << std::endl;
    
    std::cout << "FlockingParameters field offsets:" << std::endl;
    std::cout << "  separationDistance: " << offsetof(FlockingParameters, separationDistance) << std::endl;
    std::cout << "  alignmentDistance:  " << offsetof(FlockingParameters, alignmentDistance) << std::endl;
    std::cout << "  cohesionDistance:   " << offsetof(FlockingParameters, cohesionDistance) << std::endl;
    std::cout << "  separationForce:    " << offsetof(FlockingParameters, separationForce) << std::endl;
    std::cout << "  alignmentForce:     " << offsetof(FlockingParameters, alignmentForce) << std::endl;
    std::cout << "  cohesionForce:      " << offsetof(FlockingParameters, cohesionForce) << std::endl;
    std::cout << "  maxSpeed:           " << offsetof(FlockingParameters, maxSpeed) << std::endl;
    std::cout << "  maxForce:           " << offsetof(FlockingParameters, maxForce) << std::endl;
    std::cout << "  numBoids:           " << offsetof(FlockingParameters, numBoids) << std::endl;
    std::cout << "  deltaTime:          " << offsetof(FlockingParameters, deltaTime) << std::endl;
    std::cout << "  randomSeed:         " << offsetof(FlockingParameters, randomSeed) << std::endl;
    std::cout << "  speedMultiplier:    " << offsetof(FlockingParameters, speedMultiplier) << std::endl;
    std::cout << "  boundingBoxMin:     " << offsetof(FlockingParameters, boundingBoxMin) << std::endl;
    std::cout << "  boundingBoxMax:     " << offsetof(FlockingParameters, boundingBoxMax) << std::endl;
    
    std::cout << std::endl;
    std::cout << "Field sizes:" << std::endl;
    std::cout << "  sizeof(vec3): " << sizeof(vec3) << " bytes" << std::endl;
    std::cout << "  sizeof(vec4): " << sizeof(vec4) << " bytes" << std::endl;
    std::cout << "  sizeof(float): " << sizeof(float) << " bytes" << std::endl;
    std::cout << "  sizeof(int): " << sizeof(int) << " bytes" << std::endl;
    std::cout << std::endl;
    
    // Check if the structs have the expected alignment
    bool boidCorrectSize = sizeof(GPUBoidData) == 80;
    bool paramsCorrectSize = sizeof(FlockingParameters) == 80;
    std::cout << "GPUBoidData size correct for std140: " << (boidCorrectSize ? "YES" : "NO") << std::endl;
    std::cout << "FlockingParameters size correct for std140: " << (paramsCorrectSize ? "YES" : "NO") << std::endl;
    
    // Test with actual values
    GPUBoidData test;
    test.position = {1.0f, 2.0f, 3.0f};
    test.velocity = {4.0f, 5.0f, 6.0f};
    test.acceleration = {7.0f, 8.0f, 9.0f};
    test.color = {0.5f, 0.5f, 0.5f, 1.0f};
    test.lastPosition = {10.0f, 11.0f, 12.0f};
    
    std::cout << std::endl;
    std::cout << "Test data:" << std::endl;
    std::cout << "  position: " << test.position.x << ", " << test.position.y << ", " << test.position.z << std::endl;
    std::cout << "  velocity: " << test.velocity.x << ", " << test.velocity.y << ", " << test.velocity.z << std::endl;
    
    return 0;
}
