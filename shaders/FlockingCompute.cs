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
}
