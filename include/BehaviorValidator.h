#ifndef BEHAVIOR_VALIDATOR_H
#define BEHAVIOR_VALIDATOR_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include "ngl_compat/Vector.h"
#include "boid.h"

/**
 * @brief Utility class to validate that modern GLM implementations match legacy behavior
 * 
 * This class helps debug differences between legacy and modern flocking implementations
 * by providing side-by-side comparison tools.
 */
class BehaviorValidator
{
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
    
    /**
     * @brief Compare legacy and modern flocking calculations for a specific boid
     * @param boidIndex Index of the boid to analyze
     * @param boidList List of all boids
     * @param legacyBehaviours Legacy behavior calculator
     * @return True if calculations match within tolerance
     */
    static bool validateBoidCalculation(int boidIndex, 
                                      const std::vector<Boid*>& boidList,
                                      class Behaviours* legacyBehaviours);
    
    /**
     * @brief Log detailed comparison of legacy vs modern calculations
     * @param boidIndex Index of the boid to analyze
     * @param boidList List of all boids  
     * @param legacyBehaviours Legacy behavior calculator
     */
    static void logDetailedComparison(int boidIndex,
                                    const std::vector<Boid*>& boidList,
                                    class Behaviours* legacyBehaviours);
    
    /**
     * @brief Calculate modern flocking forces for a specific boid
     * @param boidIndex Index of the boid
     * @param boidList List of all boids
     * @param behaviourDistance Cohesion/alignment range
     * @param flockDistance Separation range
     * @param legacyBehaviours Legacy behavior instance for parameter extraction
     * @return Calculated forces
     */
    static FlockingForces calculateModernForces(int boidIndex,
                                              const std::vector<Boid*>& boidList,
                                              float behaviourDistance,
                                              float flockDistance,
                                              class Behaviours* legacyBehaviours);
    
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

#endif // BEHAVIOR_VALIDATOR_H
