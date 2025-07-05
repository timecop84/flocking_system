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

// Local work group size - optimized for GPU architecture
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

// Boid data structure matching C++ BoidData
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

// Shader Storage Buffer Objects for boid data
layout(std140, binding = 0) restrict buffer BoidDataBuffer {
    BoidData boids[];
};

layout(std140, binding = 1) restrict writeonly buffer BoidDataOutBuffer {
    BoidData boidsOut[];
};

// Flocking parameters uniform block
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
};

// Simple random function
float random(float seed, float coord) {
    return fract(sin(dot(vec2(seed, coord), vec2(12.9898, 78.233))) * 43758.5453);
}

// Get random jitter - match CPU exactly with reduced strength for stability
vec3 getRandomJitter(float seed, float index) {
    // Match CPU random generation: ((rand() % 1000) / 1000.0f - 0.5f) * 2.0f
    vec3 jitter;
    jitter.x = (random(seed + 1.0, index * 3.0 + 1.0) - 0.5) * 2.0;
    jitter.y = (random(seed + 2.0, index * 3.0 + 2.0) - 0.5) * 2.0;
    jitter.z = (random(seed + 3.0, index * 3.0 + 3.0) - 0.5) * 2.0;
    return jitter * 1.5; // FIXED: Match CPU jitter strength (was 0.5, now 1.5)
}

// Boundary constraint - keep boids within bounds
vec3 applyBoundaryConstraint(vec3 position, vec3 velocity) {
    vec3 force = vec3(0.0);
    float margin = 10.0;
    
    // X boundaries
    if (position.x < boundingBoxMin.x + margin) {
        force.x += (boundingBoxMin.x + margin - position.x) * 0.1;
    } else if (position.x > boundingBoxMax.x - margin) {
        force.x += (boundingBoxMax.x - margin - position.x) * 0.1;
    }
    
    // Y boundaries
    if (position.y < boundingBoxMin.y + margin) {
        force.y += (boundingBoxMin.y + margin - position.y) * 0.1;
    } else if (position.y > boundingBoxMax.y - margin) {
        force.y += (boundingBoxMax.y - margin - position.y) * 0.1;
    }
    
    // Z boundaries
    if (position.z < boundingBoxMin.z + margin) {
        force.z += (boundingBoxMin.z + margin - position.z) * 0.1;
    } else if (position.z > boundingBoxMax.z - margin) {
        force.z += (boundingBoxMax.z - margin - position.z) * 0.1;
    }
    
    return force;
}

void main() {
    uint index = gl_GlobalInvocationID.x;
    
    // Check bounds
    if (index >= numBoids) return;
    
    // Current boid data with safety checks
    vec3 position = boids[index].position;
    vec3 velocity = boids[index].velocity;
    
    // Safety checks for NaN and infinity
    if (any(isnan(position)) || any(isinf(position))) {
        position = vec3(0.0, 0.0, 0.0);
    }
    if (any(isnan(velocity)) || any(isinf(velocity))) {
        velocity = vec3(0.1, 0.1, 0.0);
    }
    
    // Behavior accumulators - match CPU algorithm exactly
    vec3 coherence = vec3(0.0);
    vec3 alignmentSum = vec3(0.0);
    vec3 separation = vec3(0.0);
    
    int cohesionCount = 0;
    int alignmentCount = 0;
    
    // Squared distances for optimization - match CPU exactly
    float behaviorDistanceSq = cohesionDistance * cohesionDistance;  // CPU uses cohesionDistance for both
    float flockDistanceSq = separationDistance * separationDistance;
    
    // Check against all other boids - exact CPU algorithm
    for (int i = 0; i < numBoids; i++) {
        if (i == index) continue; // Skip self
        
        vec3 neighborPos = boids[i].position;
        vec3 neighborVel = boids[i].velocity;
        
        // Calculate distance vector
        vec3 boidDistance = position - neighborPos;
        float distanceSq = dot(boidDistance, boidDistance);
        
        // COHESION & ALIGNMENT: Check if within behavior distance
        if (distanceSq < behaviorDistanceSq && distanceSq > 0.001) {
            coherence += neighborPos;
            cohesionCount++;
            
            // ALIGNMENT: Same distance check
            alignmentSum += neighborVel;
            alignmentCount++;
        }
        
        // SEPARATION: Check if within flock distance
        if (distanceSq < flockDistanceSq && distanceSq > 0.001) {
            // Match CPU calculateSeparationModern exactly:
            float distance = sqrt(distanceSq);
            vec3 diff = position - neighborPos;
            if (length(diff) > 0.0001) {
                diff = normalize(diff);
                diff /= distance; // CRITICAL: Weight by distance for stability
                separation += diff;
            }
        }
    }
    
    // COHESION: Finalize calculation - match CPU exactly
    if (cohesionCount > 0) {
        coherence /= float(cohesionCount);
        coherence = coherence - position; // Direction toward centroid
        if (length(coherence) > 0.0001) {
            coherence = normalize(coherence);
        } else {
            coherence = vec3(0.0);
        }
    } else {
        coherence = vec3(0.0);
    }
    
    // ALIGNMENT: Finalize calculation - match CPU exactly
    if (alignmentCount > 0) {
        alignmentSum /= float(alignmentCount);
        if (length(alignmentSum) > 0.0001) {
            alignmentSum = normalize(alignmentSum);
            // Reduce alignment strength by 50% for chaotic movement (CPU does this)
            alignmentSum *= 0.5;
        } else {
            alignmentSum = vec3(0.0);
        }
    } else {
        alignmentSum = vec3(0.0);
    }
    
    // SEPARATION: Finalize calculation - match CPU exactly
    int separationCount = 0; // Count separation neighbors
    if (length(separation) > 0.0001) {
        // Calculate approximate count based on separation magnitude
        separationCount = max(1, int(length(separation) * 10.0)); 
        separation /= float(separationCount);
        separation *= separationForce;
    }
    
    // BEHAVIOR SETUP: Combine forces exactly like CPU
    vec3 separationSet = separation; // Don't negate - modern CPU doesn't use correction
    vec3 cohesionSet = coherence * cohesionForce;
    vec3 alignmentSet = alignmentSum * alignmentForce;
    
    // Add random jitter for fly-like chaotic movement
    vec3 randomJitter = getRandomJitter(randomSeed, float(index));
    
    vec3 behaviourSetup = separationSet + cohesionSet + alignmentSet + randomJitter;
    
    // Force limit - match CPU exactly
    if (length(behaviourSetup) > 0.5) {  // FIXED: CPU uses 0.5, not 1.0
        if (length(behaviourSetup) > 0.0001) {
            behaviourSetup = normalize(behaviourSetup) * 0.5;  // FIXED: CPU uses 0.5
        } else {
            behaviourSetup = vec3(0.0);
        }
    }
    
    // Apply speed multiplier
    behaviourSetup *= speedMultiplier;
    
    // Apply boundary constraints
    behaviourSetup += applyBoundaryConstraint(position, velocity);
    
    // DEBUGGING: Clamp behavior forces to prevent explosive movement
    float behaviorMagnitude = length(behaviourSetup);
    if (behaviorMagnitude > 5.0) {
        behaviourSetup = normalize(behaviourSetup) * 5.0;
    }
    
    // Update velocity - match CPU exactly
    velocity += behaviourSetup;
    
    // Velocity constraint (max speed) - match CPU but be more conservative
    float velocityLength = length(velocity);
    if (velocityLength > maxSpeed * 2.0) {  // REDUCED: was 10.0, now 2.0
        // If velocity is high, clamp it more aggressively
        if (velocityLength > 0.0001) {
            velocity = normalize(velocity) * maxSpeed;
        }
    } else if (velocityLength > maxSpeed) {
        if (velocityLength > 0.0001) {
            velocity = normalize(velocity) * maxSpeed;
        }
    } else if (velocityLength < 0.1) { // REDUCED: was 0.3, now 0.1
        if (velocityLength > 0.0001) {
            velocity = normalize(velocity) * 0.1;
        }
    }
    
    // Position update - FIXED: match CPU exactly with safe deltaTime
    // CPU uses simple integration but with proper time scaling
    float safeDeltaTime = min(deltaTime, 0.016); // Cap at ~60fps for stability
    position += velocity * safeDeltaTime;
    
    // Final safety checks before writing results
    if (any(isnan(position)) || any(isinf(position))) {
        position = vec3(0.0, 0.0, 0.0);
        velocity = vec3(0.1, 0.1, 0.0);
    }
    if (any(isnan(velocity)) || any(isinf(velocity))) {
        velocity = vec3(0.1, 0.1, 0.0);
    }
    if (any(isnan(behaviourSetup)) || any(isinf(behaviourSetup))) {
        behaviourSetup = vec3(0.0, 0.0, 0.0);
    }
    
    // Write results to output buffer
    boidsOut[index].position = position;
    boidsOut[index].velocity = velocity;
    boidsOut[index].acceleration = behaviourSetup; // Store the applied force
    boidsOut[index].color = boids[index].color; // Preserve color
    boidsOut[index].lastPosition = boids[index].position; // Store previous position
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
    
    // Set up buffer bindings based on current swap state
    GLuint inputBuffer = m_useSwappedBuffers ? m_boidOutputSSBO : m_boidSSBO;
    GLuint outputBuffer = m_useSwappedBuffers ? m_boidSSBO : m_boidOutputSSBO;
    
    // Bind buffers with proper roles
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer);  // Input buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBuffer); // Output buffer
    
    // CRITICAL: Bind the parameters uniform buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_paramUBO);  // Parameters UBO
    
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
    if (frameCount < 1) {
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
    
    // CRITICAL: Swap buffers for next frame
    // This ensures next frame reads from the output of this frame
    m_useSwappedBuffers = !m_useSwappedBuffers;
    
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
    
    // Read from the current output buffer (the one that was written to last)
    GLuint currentOutputBuffer = m_useSwappedBuffers ? m_boidSSBO : m_boidOutputSSBO;
    
    // Map output buffer and read data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, currentOutputBuffer);
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
