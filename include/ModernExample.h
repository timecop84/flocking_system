#pragma once

/**
 * Example of how to use modern types alongside legacy code
 * This shows the migration path from ngl_compat to modern GLM-based code
 */

#include "FlockTypes.h"
#include "Random.h"
#include <iostream>

namespace flock::examples {

// Example: Modern flocking behavior calculation
class ModernFlocking {
public:
    struct FlockingResult {
        flock::Vec3 cohesion;
        flock::Vec3 alignment; 
        flock::Vec3 separation;
        flock::Vec3 combined;
    };
    
    static FlockingResult calculateFlocking(
        const flock::Vec3& boidPosition,
        const flock::Vec3& boidVelocity,
        const std::vector<flock::Vec3>& neighborPositions,
        const std::vector<flock::Vec3>& neighborVelocities,
        float separationDistance = 2.0f,
        float alignmentDistance = 4.0f,
        float cohesionDistance = 6.0f
    ) {
        FlockingResult result{};
        
        if (neighborPositions.empty()) return result;
        
        size_t separationCount = 0, alignmentCount = 0, cohesionCount = 0;
        
        for (size_t i = 0; i < neighborPositions.size(); ++i) {
            float distance = flock::Utils::distance(boidPosition, neighborPositions[i]);
            
            // Separation: steer away from nearby boids
            if (distance > 0.0f && distance < separationDistance) {
                flock::Vec3 diff = boidPosition - neighborPositions[i];
                diff = flock::Utils::normalize(diff) / distance; // Weight by distance
                result.separation += diff;
                separationCount++;
            }
            
            // Alignment: match velocity of nearby boids
            if (distance > 0.0f && distance < alignmentDistance) {
                result.alignment += neighborVelocities[i];
                alignmentCount++;
            }
            
            // Cohesion: move toward center of nearby boids
            if (distance > 0.0f && distance < cohesionDistance) {
                result.cohesion += neighborPositions[i];
                cohesionCount++;
            }
        }
        
        // Average and normalize the forces
        if (separationCount > 0) {
            result.separation /= static_cast<float>(separationCount);
            if (flock::Utils::length(result.separation) > 0.0f) {
                result.separation = flock::Utils::normalize(result.separation) * 1.5f; // Separation strength
            }
        }
        
        if (alignmentCount > 0) {
            result.alignment /= static_cast<float>(alignmentCount);
            if (flock::Utils::length(result.alignment) > 0.0f) {
                result.alignment = flock::Utils::normalize(result.alignment) * 1.0f; // Alignment strength
            }
        }
        
        if (cohesionCount > 0) {
            result.cohesion /= static_cast<float>(cohesionCount);
            result.cohesion = result.cohesion - boidPosition; // Vector toward center
            if (flock::Utils::length(result.cohesion) > 0.0f) {
                result.cohesion = flock::Utils::normalize(result.cohesion) * 1.0f; // Cohesion strength
            }
        }
        
        // Combine forces
        result.combined = result.separation + result.alignment + result.cohesion;
        
        return result;
    }
};

// Example: Usage demonstration
inline void demonstrateModernUsage() {
    std::cout << "=== Modern Flocking System Demo ===" << std::endl;
    
    // Create random positions using modern Random
    flock::Random random;
    
    std::vector<flock::Vec3> boidPositions;
    std::vector<flock::Vec3> boidVelocities;
    
    // Generate 10 random boids
    for (int i = 0; i < 10; ++i) {
        boidPositions.push_back(random.randomVec3() * 10.0f);
        boidVelocities.push_back(random.randomNormalizedVec3() * 2.0f);
    }
    
    // Calculate flocking for first boid
    auto flocking = ModernFlocking::calculateFlocking(
        boidPositions[0], 
        boidVelocities[0],
        std::vector<flock::Vec3>(boidPositions.begin() + 1, boidPositions.end()),
        std::vector<flock::Vec3>(boidVelocities.begin() + 1, boidVelocities.end())
    );
    
    std::cout << "Boid 0 Position: (" << boidPositions[0].x << ", " << boidPositions[0].y << ", " << boidPositions[0].z << ")" << std::endl;
    std::cout << "Combined Force: (" << flocking.combined.x << ", " << flocking.combined.y << ", " << flocking.combined.z << ")" << std::endl;
    
    // Example of using modern math functions
    float distance = flock::Utils::distance(boidPositions[0], boidPositions[1]);
    flock::Vec3 normalized = flock::Utils::normalize(boidVelocities[0]);
    
    std::cout << "Distance to nearest boid: " << distance << std::endl;
    std::cout << "Normalized velocity: (" << normalized.x << ", " << normalized.y << ", " << normalized.z << ")" << std::endl;
    
    // Random colors
    flock::Color randomColor = random.randomColor();
    std::cout << "Random color: (" << randomColor.r << ", " << randomColor.g << ", " << randomColor.b << ", " << randomColor.a << ")" << std::endl;
}

}
