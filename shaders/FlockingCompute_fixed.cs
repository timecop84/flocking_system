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
    vec3 obstaclePosition;   // Added obstacle position
    float obstacleRadius;    // Added obstacle radius
};

void main() {
    uint index = gl_GlobalInvocationID.x;
    
    // Check bounds
    if (index >= numBoids) return;
    
    // Current boid data
    vec3 position = boids[index].position;
    vec3 velocity = boids[index].velocity;

    // --- DEBUG: Pass-through shader ---
    // All flocking logic is temporarily disabled to ensure the shader compiles and runs.
    
    // Write original data directly to output buffer
    boidsOut[index].position = position;
    boidsOut[index].velocity = velocity;
    boidsOut[index].acceleration = vec3(0.0); // No acceleration
    boidsOut[index].color = boids[index].color; // Preserve color
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

// Enhanced boundary constraint with anti-center-collapse measures
vec3 applyBoundaryConstraint(vec3 position, vec3 velocity) {
    vec3 force = vec3(0.0);
    float margin = 10.0;
    float boxCenterX = (boundingBoxMin.x + boundingBoxMax.x) * 0.5;
    float boxCenterY = (boundingBoxMin.y + boundingBoxMax.y) * 0.5;
    float boxCenterZ = (boundingBoxMin.z + boundingBoxMax.z) * 0.5;
    
    // X boundaries with stronger force
    if (position.x < boundingBoxMin.x + margin) {
        force.x += (boundingBoxMin.x + margin - position.x) * 0.3; // Stronger than before
    } else if (position.x > boundingBoxMax.x - margin) {
        force.x += (boundingBoxMax.x - margin - position.x) * 0.3;
    }
    
    // Y boundaries with stronger force
    if (position.y < boundingBoxMin.y + margin) {
        force.y += (boundingBoxMin.y + margin - position.y) * 0.3;
    } else if (position.y > boundingBoxMax.y - margin) {
        force.y += (boundingBoxMax.y - margin - position.y) * 0.3;
    }
    
    // Z boundaries with stronger force
    if (position.z < boundingBoxMin.z + margin) {
        force.z += (boundingBoxMin.z + margin - position.z) * 0.3;
    } else if (position.z > boundingBoxMax.z - margin) {
        force.z += (boundingBoxMax.z - margin - position.z) * 0.3;
    }
    
    // Anti-center-collapse: Add slight outward force from center
    vec3 toCenter = vec3(boxCenterX, boxCenterY, boxCenterZ) - position;
    float distToCenter = length(toCenter);
    
    // If too close to center, add outward force
    if (distToCenter < 8.0 && distToCenter > 0.001) {
        vec3 outwardDir = normalize(-toCenter); // Direction away from center
        float strength = (8.0 - distToCenter) / 8.0; // Stronger when closer to center
        force += outwardDir * strength * 0.5;
    }
    
    return force;
}

// Obstacle avoidance - keep boids away from obstacle (matching CPU implementation)
vec3 applyObstacleAvoidance(vec3 position) {
    // Vector from boid to obstacle
    vec3 toObstacle = obstaclePosition - position;
    float distance = length(toObstacle);
    
    // Define avoidance zones (matching CPU values)
    float avoidanceRadius = obstacleRadius * 2.5;  // Start avoiding at this distance
    float collisionRadius = obstacleRadius * 1.2;  // Hard collision boundary
    
    if (distance < avoidanceRadius && distance > 0.0001) {
        // Calculate repulsion direction (away from obstacle)
        vec3 repulsionDir = position - obstaclePosition;
        if (length(repulsionDir) > 0.0001) {
            repulsionDir = normalize(repulsionDir);
            
            // Force strength inversely proportional to distance (stronger when closer)
            float forceStrength = (avoidanceRadius - distance) / avoidanceRadius;
            forceStrength = forceStrength * forceStrength; // Quadratic falloff for smoother transition
            
            // Return repulsion force vector
            return repulsionDir * (forceStrength * 0.3);
        }
    }
    
    return vec3(0.0); // No avoidance needed
}

void main() {
    uint index = gl_GlobalInvocationID.x;
    
    // Check bounds
    if (index >= numBoids) return;
    
    // Current boid data
    vec3 position = boids[index].position;
    vec3 velocity = boids[index].velocity;

    // --- DEBUG: Pass-through shader ---
    // All flocking logic is temporarily disabled to ensure the shader compiles and runs.
    
    // Write original data directly to output buffer
    boidsOut[index].position = position;
    boidsOut[index].velocity = velocity;
    boidsOut[index].acceleration = vec3(0.0); // No acceleration
    boidsOut[index].color = boids[index].color; // Preserve color
}
