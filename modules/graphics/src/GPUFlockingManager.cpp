#include "../include/GPUFlockingManager.h"
#include "../include/ShaderLib.h"
#include "../../../include/glew_compat.h"  // Include proper OpenGL function loader
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <vector>
#include <cstdio>  // For FILE operations

namespace FlockingGraphics {

// Helper function to load shader source from a file
std::string loadShaderSource(const std::string& filepath) {
    std::cout << "Attempting to load shader file: " << filepath << std::endl;
    
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cout << "Primary path failed, trying alternative path..." << std::endl;
            // Try alternative path for build directory
            std::ifstream alt_file("../../" + filepath);
            if (alt_file.is_open()) {
                std::stringstream buffer;
                buffer << alt_file.rdbuf();
                std::cout << "Successfully loaded shader file: " << "../../" + filepath << " (" << buffer.str().length() << " bytes)" << std::endl;
                return buffer.str();
            }
            std::cerr << "ERROR: Could not open shader file: " << filepath << " or " << "../../" + filepath << std::endl;
            return "";
        }
        
        std::cout << "Reading shader file content..." << std::endl;
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::cout << "Successfully loaded shader file: " << filepath << " (" << content.length() << " bytes)" << std::endl;
        
        // Check for empty or invalid content
        if (content.empty()) {
            std::cerr << "ERROR: Shader file is empty!" << std::endl;
            return "";
        }
        
        return content;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception while loading shader: " << e.what() << std::endl;
        return "";
    } catch (...) {
        std::cerr << "Unknown exception while loading shader" << std::endl;
        return "";
    }
}


GPUFlockingManager::GPUFlockingManager() {
    m_parameters = FlockingParameters(); // Initialize with defaults
}

GPUFlockingManager::~GPUFlockingManager() {
    cleanup();
}

bool GPUFlockingManager::initialize() {
    std::cout << "=== INITIALIZING GPU FLOCKING ACCELERATION ===" << std::endl;
    
    // Check compute shader support
    GLint maxComputeWorkGroupSizeX;
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxComputeWorkGroupSizeX);
    std::cout << "Max compute work group size X: " << maxComputeWorkGroupSizeX << std::endl;
    
    if (!createComputeShader()) {
        std::cerr << "Failed to create compute shader! Falling back to CPU flocking." << std::endl;
        m_initialized = false;
        m_enabled = false;
        std::cout << "===============================================" << std::endl;
        return false;
    }
    
    if (!createBuffers()) {
        std::cerr << "Failed to create GPU buffers! Falling back to CPU flocking." << std::endl;
        return false;
    }
    
    m_initialized = true;
    m_enabled = false; // Start disabled for safety
    std::cout << "GPU Flocking Manager initialized successfully!" << std::endl;
    std::cout << "Note: GPU flocking starts disabled. Press 'G' to toggle GPU/CPU modes." << std::endl;
    std::cout << "===============================================" << std::endl;
    return true;
}

bool GPUFlockingManager::createComputeShader() {
    std::cout << "Using embedded compute shader source" << std::endl;
    
    // Embed shader source directly to bypass file loading issues
    std::string shaderSource = R"(
#version 450 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct BoidData {
    vec3 position;
    float padding1;
    vec3 velocity;
    float padding2;
    vec3 acceleration;
    float padding3;
    vec4 color;
    vec3 lastPosition;
    float padding4;
};

layout(std140, binding = 0) restrict buffer BoidDataBuffer {
    BoidData boids[];
};

layout(std140, binding = 1) restrict writeonly buffer BoidDataOutBuffer {
    BoidData boidsOut[];
};

layout(std140, binding = 2) uniform FlockingParameters {
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
    vec3 obstaclePosition;
    float obstacleRadius;
};

void main() {
    uint index = gl_GlobalInvocationID.x;
    if (index >= numBoids) return;
    
    vec3 position = boids[index].position;
    vec3 velocity = boids[index].velocity;
    vec3 lastPosition = boids[index].lastPosition;
    
    // Initialize forces
    vec3 cohesionForceVec = vec3(0.0);
    vec3 alignmentForceVec = vec3(0.0);
    vec3 separationForceVec = vec3(0.0);
    
    int cohesionCount = 0;
    int alignmentCount = 0;
    int separationCount = 0;
    
    // Check neighbors - match CPU exactly
    for (int i = 0; i < numBoids; i++) {
        if (i == index) continue;
        
        vec3 neighborPos = boids[i].position;
        vec3 neighborVel = boids[i].velocity;
        vec3 diff = position - neighborPos;
        float distanceSq = dot(diff, diff);
        
        // SEPARATION: Match CPU flock.cpp exactly (squared distance approach)
        if (distanceSq < separationDistance * separationDistance && distanceSq > 0.001) {
            // Apply distance-based separation force - EXACT CPU match
            float separationStrength = (separationDistance * separationDistance - distanceSq) / (separationDistance * separationDistance);
            vec3 separationForce = diff * separationStrength;
            separationForceVec += separationForce;
            separationCount++;
        }
        
        // COHESION & ALIGNMENT: Check if within behavior distance (squared)
        if (distanceSq < cohesionDistance * cohesionDistance && distanceSq > 0.001) {
            cohesionForceVec += neighborPos;
            cohesionCount++;
            alignmentForceVec += neighborVel;
            alignmentCount++;
        }
    }
    
    // Finalize cohesion - match CPU exactly
    if (cohesionCount > 0) {
        cohesionForceVec /= float(cohesionCount);  // Get centroid
        cohesionForceVec = cohesionForceVec - position;  // Direction toward centroid
        if (length(cohesionForceVec) > 0.0001) {
            cohesionForceVec = normalize(cohesionForceVec);
        } else {
            cohesionForceVec = vec3(0.0);
        }
    }
    
    // Finalize alignment - match CPU exactly  
    if (alignmentCount > 0) {
        alignmentForceVec /= float(alignmentCount);  // Get average velocity
        if (length(alignmentForceVec) > 0.0001) {
            alignmentForceVec = normalize(alignmentForceVec);
            alignmentForceVec *= 0.5;  // Reduce alignment strength by 50% like CPU
        } else {
            alignmentForceVec = vec3(0.0);
        }
    }
    
    // Finalize separation - CPU doesn't divide by count or multiply by separationForce
    // The CPU applies separationForce after all forces are combined
    // separationForceVec is already accumulated correctly above
    
    // Combine forces like CPU: separationSet + cohesionSet + alignmentSet
    vec3 separationSet = separationForceVec * separationForce;
    vec3 cohesionSet = cohesionForceVec * cohesionForce;
    vec3 alignmentSet = alignmentForceVec * alignmentForce;
    
    vec3 totalForce = separationSet + cohesionSet + alignmentSet;
    
    // Add random jitter for fly-like chaotic movement (match CPU)
    float randX = fract(sin(dot(position.xy + randomSeed, vec2(12.9898, 78.233))) * 43758.5453);
    float randY = fract(sin(dot(position.yz + randomSeed, vec2(12.9898, 78.233))) * 43758.5453);
    float randZ = fract(sin(dot(position.xz + randomSeed, vec2(12.9898, 78.233))) * 43758.5453);
    
    vec3 randomJitter = vec3(
        (randX - 0.5) * 2.0,  // Random -1 to 1
        (randY - 0.5) * 2.0,
        (randZ - 0.5) * 2.0
    );
    randomJitter *= 1.5;  // Match CPU jitter strength
    
    totalForce += randomJitter;
    
    // Apply speed multiplier to forces (match CPU)
    totalForce *= speedMultiplier;
    
    // Apply force limit like CPU (increased from 0.5f to 1.5f for more dynamic movement)
    if (length(totalForce) > 1.5) {
        totalForce = normalize(totalForce) * 1.5;
    }
    
    // Update velocity (match CPU: updateVelocity)
    velocity += totalForce;
    
    // Apply velocity constraints (match CPU: velocityConstraint)
    float velocityLength = length(velocity);
    float maxVelocity = 10.0;  // Match CPU max velocity
    float minVelocity = 0.1;   // Match CPU min velocity
    
    // Check for extreme values that might cause instability
    if (velocityLength > maxVelocity * 10.0) {
        // If velocity is extremely high, clamp it more aggressively
        if (velocityLength > 0.0001) {
            velocity = normalize(velocity) * maxVelocity;
        }
    } else if (velocityLength > maxVelocity) {
        if (velocityLength > 0.0001) {
            velocity = normalize(velocity) * maxVelocity;
        }
    } else if (velocityLength < minVelocity) {
        if (velocityLength > 0.0001) {
            velocity = normalize(velocity) * minVelocity;
        } else {
            // If velocity is essentially zero, give it a small default velocity
            velocity = vec3(0.1, 0.1, 0.0);
        }
    }
    
    // Calculate new direction and update position (match CPU: boidDirection)
    vec3 newDirection = position - lastPosition;
    
    // Safety checks for NaN values
    if (any(isnan(velocity))) {
        velocity = vec3(0.1, 0.1, 0.0);
    }
    if (any(isnan(newDirection))) {
        newDirection = vec3(0.0, 0.0, 0.0);
    }
    
    // CPU-style position update: blend velocity with previous movement
    vec3 nextMovement = (velocity + newDirection) * 0.5;
    
    // Safety check for NaN values in the final movement calculation
    if (any(isnan(nextMovement))) {
        nextMovement = vec3(0.1, 0.1, 0.0);
    }
    
    // Update position and lastPosition (match CPU exactly)
    vec3 newLastPosition = position;
    position += nextMovement;
    
    // Final safety check for position
    if (any(isnan(position))) {
        position = vec3(0.0, 0.0, 0.0);
        velocity = vec3(0.1, 0.1, 0.0);
    }
    
    // Write results
    boidsOut[index].position = position;
    boidsOut[index].velocity = velocity;
    boidsOut[index].acceleration = totalForce;
    boidsOut[index].color = boids[index].color;
    boidsOut[index].lastPosition = newLastPosition;
}
)";

    std::cout << "Embedded shader loaded (" << shaderSource.length() << " bytes)" << std::endl;

    if (shaderSource.empty()) {
        std::cerr << "ERROR: Failed to load compute shader source." << std::endl;
        return false;
    }

    // Create and compile compute shader
    std::cout << "Creating and compiling compute shader..." << std::endl;
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);

    if (computeShader == 0) {
        std::cerr << "ERROR: Failed to create compute shader object!" << std::endl;
        return false;
    }

    const char* source = shaderSource.c_str();
    glShaderSource(computeShader, 1, &source, nullptr);
    glCompileShader(computeShader);

    // Check compilation
    std::cout << "Checking compute shader compilation status..." << std::endl;
    GLint success;
    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(computeShader, 1024, nullptr, infoLog);
        std::cerr << "ERROR: Compute shader compilation failed!" << std::endl;
        std::cerr << "Compilation log: " << infoLog << std::endl;
        glDeleteShader(computeShader);
        return false;
    }
    std::cout << "Compute shader compiled successfully!" << std::endl;

    // Create and link program
    std::cout << "Creating and linking compute program..." << std::endl;
    m_computeProgram = glCreateProgram();

    if (m_computeProgram == 0) {
        std::cerr << "ERROR: Failed to create compute program!" << std::endl;
        glDeleteShader(computeShader);
        return false;
    }

    glAttachShader(m_computeProgram, computeShader);
    glLinkProgram(m_computeProgram);

    // Check linking
    std::cout << "Checking compute program linking status..." << std::endl;
    glGetProgramiv(m_computeProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(m_computeProgram, 1024, nullptr, infoLog);
        std::cerr << "ERROR: Compute program linking failed!" << std::endl;
        std::cerr << "Linking log: " << infoLog << std::endl;
        glDeleteShader(computeShader);
        glDeleteProgram(m_computeProgram);
        m_computeProgram = 0; // Reset program handle
        return false;
    }
    std::cout << "Compute program linked successfully!" << std::endl;

    glDeleteShader(computeShader);
    std::cout << "Compute shader compiled and linked successfully" << std::endl;
    return true;
}

bool GPUFlockingManager::createBuffers() {
    // Create Shader Storage Buffer Objects for boid data
    glGenBuffers(1, &m_boidSSBO);
    glGenBuffers(1, &m_boidOutputSSBO);
    glGenBuffers(1, &m_paramUBO);
    
    // Initialize with default size (will be resized as needed)
    const size_t initialSize = 2000 * sizeof(GPUBoidData);
    
    // Input buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_boidSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, initialSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_boidSSBO);
    
    // Output buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_boidOutputSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, initialSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_boidOutputSSBO);
    
    // Parameters uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, m_paramUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(FlockingParameters), &m_parameters, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_paramUBO);
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    std::cout << "GPU buffers created: Input SSBO, Output SSBO, Parameters UBO" << std::endl;
    return true;
}

void GPUFlockingManager::updateParameters(const FlockingParameters& params) {
    m_parameters = params;
    
    if (m_initialized) {
        // Update random seed for variety
        updateRandomSeed();
        
        // Upload to GPU
        glBindBuffer(GL_UNIFORM_BUFFER, m_paramUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(FlockingParameters), &m_parameters);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void GPUFlockingManager::uploadBoidData(const std::vector<GPUBoidData>& boidData) {
    if (!m_initialized || boidData.empty()) return;
    
    m_gpuBoidData = boidData;
    const size_t dataSize = boidData.size() * sizeof(GPUBoidData);
    
    // Upload to input buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_boidSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, boidData.data(), GL_DYNAMIC_DRAW);
    
    // Ensure output buffer is the same size
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_boidOutputSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GPUFlockingManager::computeFlocking() {
    if (!m_initialized) {
        return; // Silently skip if not initialized
    }
    if (!m_enabled) {
        return; // Silently skip if disabled
    }
    if (m_gpuBoidData.empty()) {
        return; // Silently skip if no data
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Check for OpenGL errors before starting
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error before compute: " << error << std::endl;
        return;
    }
    
    // Use the compute program
    glUseProgram(m_computeProgram);
    
    // Check program is valid
    GLint status;
    glGetProgramiv(m_computeProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        std::cerr << "Compute program not properly linked!" << std::endl;
        return;
    }
    
    // Dispatch compute shader with error checking
    const GLuint numBoids = static_cast<GLuint>(m_gpuBoidData.size());
    const GLuint workGroupSize = 64; // Must match shader local_size_x
    const GLuint numWorkGroups = (numBoids + workGroupSize - 1) / workGroupSize;
    
    // Debug output for first few frames
    static int frameCount = 0;
    if (frameCount < 3) {
        std::cout << "GPU Compute: Processing " << numBoids << " boids using " << numWorkGroups << " work groups" << std::endl;
        frameCount++;
    }
    
    // Dispatch with error checking
    glDispatchCompute(numWorkGroups, 1, 1);
    
    // Check for dispatch errors
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error during compute dispatch: " << error << std::endl;
        m_enabled = false; // Disable GPU flocking to prevent further issues
        return;
    }
    
    // Wait for compute to finish with timeout protection
    // Use glFinish instead of just memory barrier for stronger synchronization
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    
    // Check for barrier errors
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error during memory barrier: " << error << std::endl;
        m_enabled = false; // Disable GPU flocking to prevent further issues
        return;
    }
    
    // Optional: Add a fence sync for timeout protection
    GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    if (fence) {
        // Wait for completion with timeout (16ms - one frame at 60fps)
        GLenum waitResult = glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, 16000000); // 16ms in nanoseconds
        if (waitResult == GL_TIMEOUT_EXPIRED) {
            std::cerr << "GPU compute timeout! Disabling GPU flocking." << std::endl;
            m_enabled = false;
        } else if (waitResult == GL_WAIT_FAILED) {
            std::cerr << "GPU compute sync failed! Disabling GPU flocking." << std::endl;
            m_enabled = false;
        }
        glDeleteSync(fence);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    m_lastComputeTime = std::chrono::duration<float, std::milli>(endTime - startTime).count();
    
    // Print performance every 120 frames (every 2 seconds at 60fps)
    static int perfFrameCount = 0;
    perfFrameCount++;
    if (perfFrameCount >= 120) {
        std::cout << "GPU Flocking: " << m_lastComputeTime << "ms for " << numBoids << " boids" << std::endl;
        perfFrameCount = 0;
    }
}

std::vector<GPUBoidData> GPUFlockingManager::downloadBoidData() {
    if (!m_initialized || m_gpuBoidData.empty()) {
        return m_gpuBoidData;
    }
    
    const size_t dataSize = m_gpuBoidData.size() * sizeof(GPUBoidData);
    
    // Map output buffer and read data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_boidOutputSSBO);
    void* mappedData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    
    if (mappedData) {
        std::memcpy(m_gpuBoidData.data(), mappedData, dataSize);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    return m_gpuBoidData;
}

void GPUFlockingManager::updateRandomSeed() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1000.0f);
    
    m_parameters.randomSeed = dis(gen);
}

void GPUFlockingManager::cleanup() {
    if (m_computeProgram != 0) {
        glDeleteProgram(m_computeProgram);
        m_computeProgram = 0;
    }
    
    if (m_boidSSBO != 0) {
        glDeleteBuffers(1, &m_boidSSBO);
        m_boidSSBO = 0;
    }
    
    if (m_boidOutputSSBO != 0) {
        glDeleteBuffers(1, &m_boidOutputSSBO);
        m_boidOutputSSBO = 0;
    }
    
    if (m_paramUBO != 0) {
        glDeleteBuffers(1, &m_paramUBO);
        m_paramUBO = 0;
    }
    
    m_initialized = false;
}

} // namespace FlockingGraphics
