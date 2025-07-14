// BehaviorValidator.h - Modernized, documented, and cleaned up for maintainability
#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include "FlockTypes.h"
#include "Vector.h"
#include "Boid.h"

/**
 * @file BehaviorValidator.h
 * @brief Utility class to validate that modern GLM implementations match legacy behavior.
 *
 * Modernized for clarity, maintainability, and efficiency. All type aliases and math helpers are now in FlockTypes.h.
 *
 * This class helps debug differences between legacy and modern flocking implementations by providing side-by-side comparison tools.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
class BehaviorValidator {
public:
    struct BoidState {
        glm::vec3 position;
        glm::vec3 velocity;
        int id;
    };
    struct FlockingForces {
        glm::vec3 cohesion;
        glm::vec3 alignment;
        glm::vec3 separation;
        glm::vec3 combined;
    };
    static bool validateBoidCalculation(int boidIndex, const std::vector<Boid*>& boidList, class Behaviours* legacyBehaviours);
    static void logDetailedComparison(int boidIndex, const std::vector<Boid*>& boidList, class Behaviours* legacyBehaviours);
    static FlockingForces calculateModernFlockingForces(int boidIndex, const std::vector<Boid*>& boidList, float behaviourDistance, float flockDistance, class Behaviours* legacyBehaviours);
    /**
     * @brief Extract legacy forces from Behaviours class after calculation
     * @param legacyBehaviours Legacy behavior calculator (after calling methods)
     * @return Extracted forces
     */
    static FlockingForces extractLegacyForces(class Behaviours* legacyBehaviours);
    
    /**
     * @brief Compare two force structures within tolerance
     * @param legacy Legacy forces
     * @param modern Modern forces
     * @param tolerance Floating point tolerance
     * @return True if forces match within tolerance
     */
    static bool compareForcesWithTolerance(const FlockingForces& legacy,
                                         const FlockingForces& modern,
                                         float tolerance = 0.001f);
    
    /**
     * @brief Print formatted force comparison
     * @param legacy Legacy forces
     * @param modern Modern forces
     */
    static void printForceComparison(const FlockingForces& legacy,
                                   const FlockingForces& modern);
    
private:
    static constexpr float FLOAT_TOLERANCE = 0.001f;
};
