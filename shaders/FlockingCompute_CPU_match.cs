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
    vec3 obstaclePosition;
    float obstacleRadius;
};

// Simple random function
float random(float seed, float coord) {
    return fract(sin(dot(vec2(seed, coord), vec2(12.9898, 78.233))) * 43758.5453);
}

// Boundary constraint - keep boids within bounds
vec3 applyBoundaryConstraint(vec3 position) {
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
    
    // Current boid data
    vec3 position = boids[index].position;
    vec3 velocity = boids[index].velocity;
    
    // Initialize behavior forces
    vec3 cohesionForceVec = vec3(0.0);
    vec3 alignmentForceVec = vec3(0.0);
    vec3 separationForceVec = vec3(0.0);
    
    int cohesionCount = 0;
    int alignmentCount = 0;
    int separationCount = 0;
    
    // Check against all other boids
    for (int i = 0; i < numBoids; i++) {
        if (i == index) continue; // Skip self
        
        vec3 neighborPos = boids[i].position;
        vec3 neighborVel = boids[i].velocity;
        
        vec3 diff = position - neighborPos;
        float distance = length(diff);
        
        // SEPARATION: Match CPU calculateSeparationModern exactly
        if (distance > 0.0 && distance < separationDistance) {
            vec3 separationDir = normalize(diff);
            separationDir /= distance; // Weight by distance - CRITICAL for stability
            separationForceVec += separationDir;
            separationCount++;
        }
        
        // COHESION & ALIGNMENT: Within behavior distance
        if (distance < cohesionDistance) {
            cohesionForceVec += neighborPos;
            cohesionCount++;
            
            alignmentForceVec += neighborVel;
            alignmentCount++;
        }
    }
    
    // COHESION: Match CPU exactly - direction toward center of mass
    if (cohesionCount > 0) {
        cohesionForceVec /= float(cohesionCount);
        cohesionForceVec = (cohesionForceVec - position) * cohesionForce;
    }
    
    // ALIGNMENT: Match CPU exactly
    if (alignmentCount > 0) {
        alignmentForceVec /= float(alignmentCount);
        alignmentForceVec = (alignmentForceVec - velocity) * alignmentForce;
    }
    
    // SEPARATION: Match CPU exactly
    if (separationCount > 0) {
        separationForceVec /= float(separationCount);
        separationForceVec *= separationForce;
    }
    
    // Combine forces like CPU BehaviourSetup()
    vec3 totalForce = separationForceVec + cohesionForceVec + alignmentForceVec;
    
    // Apply CPU force limit exactly
    if (length(totalForce) > 0.5) {
        totalForce = normalize(totalForce) * 0.5;
    }
    
    // Apply boundary constraints
    totalForce += applyBoundaryConstraint(position);
    
    // Update velocity like CPU
    velocity += totalForce;
    
    // Apply speed constraint like CPU
    float speed = length(velocity);
    if (speed > maxSpeed) {
        velocity = normalize(velocity) * maxSpeed;
    }
    
    // Update position
    position += velocity * deltaTime;
    
    // Write results
    boidsOut[index].position = position;
    boidsOut[index].velocity = velocity;
    boidsOut[index].acceleration = totalForce;
    boidsOut[index].color = boids[index].color;
}
