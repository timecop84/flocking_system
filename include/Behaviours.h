// Behaviours.h - Modernized, documented, and cleaned up for maintainability
#pragma once
#include <vector>
#include "FlockTypes.h"
#include "boid.h"
#include "Vector.h"

/**
 * @file Behaviours.h
 * @brief The Behaviours class encapsulates flocking behavior logic (cohesion, alignment, separation, etc).
 *
 * Modernized for clarity, maintainability, and efficiency. All type aliases and math helpers are now in FlockTypes.h.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */
class Behaviours {
public:
    Behaviours();
    void Cohesion(int& boidNumber, std::vector<Boid*>& boidList);
    void Alignment(int& boidNumber, std::vector<Boid*>& boidList);
    void Seperation(int& boidNumber, std::vector<Boid*>& boidList);
    Vector m_behaviourSet();
    void Destination(int& boidNumber, std::vector<Boid*>& boidList);
    void applyModernFlockingForce(int& boidNumber, std::vector<Boid*>& boidList);
    Vector BehaviourSetup();
    void setBehaviourDistance(double distance) { m_BehaviourDistance = distance; }
    void setFlockDistance(double distance) { m_flockDistance = distance; }
    void setCohesionForce(double cohesion) { m_cohesionForce = cohesion; }
    void setSeparationForce(double separation) { m_seperationForce = separation; }
    void setAlignment(double alignment) { m_alignment = alignment; }
    
    // Getters for modern system
    double getFlockDistance() const {return m_flockDistance;}
    double getBehaviourDistance() const {return m_BehaviourDistance;}
    double getCohesionForce() const {return m_cohesionForce;}
    double getSeparationForce() const {return m_seperationForce;}
    double getAlignment() const {return m_alignment;}
    
    // Getters for validation - access calculated forces
    Vector getCoherence() const {return m_coherence;}
    Vector getAlignmentForce() const {return m_alignmentForce;}
    Vector getSeparation() const {return m_separation;}
    Vector getBehaviourSetup() const {return m_behaviourSetup;}
    Vector getCohesionSet() const {return m_cohesionSet;}
    Vector getSeparationSet() const {return m_seperationSet;}
    Vector getAlignmentSet() const {return m_alighmentSet;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our ctor
    ~Behaviours();
    
    //----------------------------------------------------------------------------------------------------------------------
    // Modern GLM-based methods for gradual migration
    //----------------------------------------------------------------------------------------------------------------------
    
    /// @brief Calculate cohesion force using modern GLM types
    /// @param [in] boidPos current boid position
    /// @param [in] neighborPositions positions of neighboring boids
    /// @return cohesion force as GLM vector
    flock::Vec3 calculateCohesionModern(const flock::Vec3& boidPos, 
                                       const std::vector<flock::Vec3>& neighborPositions) const;
    
    /// @brief Calculate separation force using modern GLM types
    /// @param [in] boidPos current boid position
    /// @param [in] neighborPositions positions of neighboring boids
    /// @return separation force as GLM vector
    flock::Vec3 calculateSeparationModern(const flock::Vec3& boidPos, 
                                         const std::vector<flock::Vec3>& neighborPositions) const;
    
    /// @brief Calculate alignment force using modern GLM types
    /// @param [in] boidVel current boid velocity
    /// @param [in] neighborVelocities velocities of neighboring boids
    /// @return alignment force as GLM vector
    flock::Vec3 calculateAlignmentModern(const flock::Vec3& boidVel, 
                                        const std::vector<flock::Vec3>& neighborVelocities) const;
    
    //----------------------------------------------------------------------------------------------------------------------

private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our boid pointer
    Boid *m_boid;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our boidlist pointer
    std::vector <Boid*> *_boidList;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the value of cohesion the check.
    Vector m_coherence;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to check the distance between the behaviours.
    double m_BehaviourDistance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to contribute additional cohesion force to the flock.
    double  m_cohesionForce;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to contribute additional seperation force to the flock.
    double m_seperationForce;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief contribution of the Alignment force in overall behaviour Flock
    double m_alignment;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the amount of Separation generated.
    Vector m_separation;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to contribute additional alignment force to the flock.
    Vector m_alignmentForce;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the diffence in positions between the current boid to the local boids.
    Vector m_boidDistance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the positions between the current boid to the local boids.
    double m_flockDistance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the behaviour movement.
    Vector m_behaviourSetup;
    /// @brief variable to store the final seperation velocity.
    //----------------------------------------------------------------------------------------------------------------------
    Vector m_seperationSet;
    /// @brief variable to store the final cohesion velocity.
    //----------------------------------------------------------------------------------------------------------------------
    Vector m_cohesionSet;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the final alignment velocity.
    Vector m_alighmentSet;
    //----------------------------------------------------------------------------------------------------------------------
};
