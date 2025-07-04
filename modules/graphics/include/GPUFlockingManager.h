#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include "../../../include/glew_compat.h"

namespace FlockingGraphics {

//----------------------------------------------------------------------------------------------------------------------
/// @brief GPU-accelerated flocking simulation using OpenGL compute shaders
/// @details Runs flocking behavior calculations on GPU (CUDA cores) for massive performance improvement
//----------------------------------------------------------------------------------------------------------------------

struct GPUBoidData {
    glm::vec3 position;
    float padding1;
    glm::vec3 velocity;
    float padding2;
    glm::vec3 acceleration;
    float padding3;
    glm::vec4 color;
    glm::vec3 lastPosition;
    float padding4;
    
    GPUBoidData() : padding1(0.0f), padding2(0.0f), padding3(0.0f), padding4(0.0f) {}
};

struct FlockingParameters {
    float separationDistance = 2.0f;
    float alignmentDistance = 4.0f;
    float cohesionDistance = 4.0f;
    float separationForce = 1.5f;
    float alignmentForce = 1.0f;
    float cohesionForce = 1.0f;
    float maxSpeed = 2.0f;
    float maxForce = 0.5f;
    int numBoids = 200;
    float deltaTime = 0.016f; // ~60 FPS
    float randomSeed = 0.0f;
    float speedMultiplier = 1.0f;
    glm::vec3 boundingBoxMin = glm::vec3(-20.0f, -20.0f, -20.0f);
    float padding1 = 0.0f;
    glm::vec3 boundingBoxMax = glm::vec3(20.0f, 20.0f, 20.0f);
    float padding2 = 0.0f;
    glm::vec3 obstaclePosition = glm::vec3(0.0f, 0.0f, 0.0f);
    float obstacleRadius = 4.0f;
};

class GPUFlockingManager {
public:
    GPUFlockingManager();
    ~GPUFlockingManager();
    
    // Initialize GPU resources
    bool initialize();
    
    // Update flocking parameters
    void updateParameters(const FlockingParameters& params);
    
    // Upload boid data to GPU
    void uploadBoidData(const std::vector<GPUBoidData>& boidData);
    
    // Run compute shader simulation
    void computeFlocking();
    
    // Download results from GPU
    std::vector<GPUBoidData> downloadBoidData();
    
    // Get performance metrics
    float getLastComputeTime() const { return m_lastComputeTime; }
    
    // Enable/disable GPU acceleration
    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }
    
    // Toggle between GPU and CPU modes
    void toggleGPUMode() { 
        if (m_initialized) {
            m_enabled = !m_enabled; 
            std::cout << "GPU Flocking: " << (m_enabled ? "ENABLED" : "DISABLED") << std::endl;
        }
    }
    
private:
    // OpenGL resources
    GLuint m_computeProgram = 0;
    GLuint m_boidSSBO = 0;        // Input buffer
    GLuint m_boidOutputSSBO = 0;  // Output buffer
    GLuint m_paramUBO = 0;        // Parameters uniform buffer
    
    // GPU state
    std::vector<GPUBoidData> m_gpuBoidData;
    FlockingParameters m_parameters;
    
    // Performance tracking
    float m_lastComputeTime = 0.0f;
    bool m_enabled = true;
    bool m_initialized = false;
    
    // Helper methods
    bool createComputeShader();
    bool createBuffers();
    void updateRandomSeed();
    void cleanup();
};

} // namespace FlockingGraphics
