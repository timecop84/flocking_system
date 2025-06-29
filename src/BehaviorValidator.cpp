#include "BehaviorValidator.h"
#include "Behaviours.h"
#include <cmath>

bool BehaviorValidator::validateBoidCalculation(int boidIndex, 
                                              const std::vector<Boid*>& boidList,
                                              Behaviours* legacyBehaviours)
{
    if (boidIndex < 0 || boidIndex >= static_cast<int>(boidList.size())) {
        return false;
    }
    
    // Calculate legacy forces
    legacyBehaviours->Cohesion(boidIndex, const_cast<std::vector<Boid*>&>(boidList));
    legacyBehaviours->Alignment(boidIndex, const_cast<std::vector<Boid*>&>(boidList));
    legacyBehaviours->Seperation(boidIndex, const_cast<std::vector<Boid*>&>(boidList));
    FlockingForces legacyForces = extractLegacyForces(legacyBehaviours);
    
    // Calculate modern forces
    FlockingForces modernForces = calculateModernForces(boidIndex, boidList,
                                                       legacyBehaviours->getBehaviourDistance(),
                                                       legacyBehaviours->getFlockDistance(),
                                                       legacyBehaviours);
    
    return compareForcesWithTolerance(legacyForces, modernForces);
}

void BehaviorValidator::logDetailedComparison(int boidIndex,
                                            const std::vector<Boid*>& boidList,
                                            Behaviours* legacyBehaviours)
{
    std::cout << "\n=== BEHAVIOR VALIDATION REPORT ===" << std::endl;
    std::cout << "Boid Index: " << boidIndex << "/" << boidList.size() << std::endl;
    
    if (boidIndex < 0 || boidIndex >= static_cast<int>(boidList.size())) {
        std::cout << "ERROR: Invalid boid index!" << std::endl;
        return;
    }
    
    Boid* currentBoid = boidList[boidIndex];
    ngl::Vector pos = currentBoid->getPosition();
    ngl::Vector vel = currentBoid->getVelocity();
    
    std::cout << "Boid Position: (" << pos.m_x << ", " << pos.m_y << ", " << pos.m_z << ")" << std::endl;
    std::cout << "Boid Velocity: (" << vel.m_x << ", " << vel.m_y << ", " << vel.m_z << ")" << std::endl;
    std::cout << "Behaviour Distance: " << legacyBehaviours->getBehaviourDistance() << std::endl;
    std::cout << "Flock Distance: " << legacyBehaviours->getFlockDistance() << std::endl;
    
    // Calculate legacy forces
    legacyBehaviours->Cohesion(boidIndex, const_cast<std::vector<Boid*>&>(boidList));
    legacyBehaviours->Alignment(boidIndex, const_cast<std::vector<Boid*>&>(boidList));
    legacyBehaviours->Seperation(boidIndex, const_cast<std::vector<Boid*>&>(boidList));
    FlockingForces legacyForces = extractLegacyForces(legacyBehaviours);
    
    // Calculate modern forces
    FlockingForces modernForces = calculateModernForces(boidIndex, boidList,
                                                       legacyBehaviours->getBehaviourDistance(),
                                                       legacyBehaviours->getFlockDistance(),
                                                       legacyBehaviours);
    
    printForceComparison(legacyForces, modernForces);
    
    bool matches = compareForcesWithTolerance(legacyForces, modernForces);
    std::cout << "Result: " << (matches ? "MATCH" : "MISMATCH") << std::endl;
    std::cout << "================================\n" << std::endl;
}

BehaviorValidator::FlockingForces BehaviorValidator::calculateModernForces(int boidIndex,
                                                                         const std::vector<Boid*>& boidList,
                                                                         float behaviourDistance,
                                                                         float flockDistance,
                                                                         Behaviours* legacyBehaviours)
{
    FlockingForces forces;
    
    if (boidIndex < 0 || boidIndex >= static_cast<int>(boidList.size())) {
        return forces; // Return zero forces
    }
    
    Boid* currentBoid = boidList[boidIndex];
    ngl::Vector currentPos = currentBoid->getPosition();
    ngl::Vector currentVel = currentBoid->getVelocity();
    glm::vec3 glmCurrentPos(currentPos.m_x, currentPos.m_y, currentPos.m_z);
    glm::vec3 glmCurrentVel(currentVel.m_x, currentVel.m_y, currentVel.m_z);
    
    // COHESION calculation (modern GLM version)
    glm::vec3 coherence(0.0f);
    int cohesionCount = 1; // Start with 1 like legacy
    
    for(int i = 0; i < static_cast<int>(boidList.size()); i++) {
        if(i != boidIndex) {
            ngl::Vector neighborPos = boidList[i]->getPosition();
            glm::vec3 glmNeighborPos(neighborPos.m_x, neighborPos.m_y, neighborPos.m_z);
            
            glm::vec3 boidDistance = glmCurrentPos - glmNeighborPos;
            
            if(glm::length(boidDistance) < behaviourDistance) {
                coherence += glmNeighborPos;
                cohesionCount++;
            }
        }
    }
    
    coherence /= static_cast<float>(cohesionCount);
    coherence = coherence - glmCurrentPos;
    if (glm::length(coherence) > 0.0001f) {
        coherence = glm::normalize(coherence);
    }
    forces.cohesion = coherence;
    
    // ALIGNMENT calculation (modern GLM version)
    // NOTE: Legacy system does NOT reset m_alignmentForce, so we start with (0,0,0)
    glm::vec3 alignmentForce(0.0f);
    int alignmentCount = 1; // Start with 1 like legacy
    
    for(int i = 0; i < static_cast<int>(boidList.size()); i++) {
        if(i != boidIndex) {
            ngl::Vector neighborPos = boidList[i]->getPosition();
            glm::vec3 glmNeighborPos(neighborPos.m_x, neighborPos.m_y, neighborPos.m_z);
            
            glm::vec3 boidDistance = glmCurrentPos - glmNeighborPos;
            
            if(glm::length(boidDistance) < behaviourDistance) {
                ngl::Vector neighborVel = boidList[i]->getVelocity();
                glm::vec3 glmNeighborVel(neighborVel.m_x, neighborVel.m_y, neighborVel.m_z);
                alignmentForce += glmNeighborVel;
                alignmentCount++;
            }
        }
    }
    
    // Match exact legacy sequence: normalize if length > behaviourDistance, then divide by count, then subtract current velocity
    if (glm::length(alignmentForce) > behaviourDistance) {
        alignmentForce = glm::normalize(alignmentForce);
    }
    alignmentForce /= static_cast<float>(alignmentCount);
    alignmentForce = alignmentForce - glmCurrentVel;
    forces.alignment = alignmentForce;
    
    // SEPARATION calculation (modern GLM version)
    glm::vec3 separation(0.0f);
    
    for(int i = 0; i < static_cast<int>(boidList.size()); i++) {
        if(i != boidIndex) {
            ngl::Vector neighborPos = boidList[i]->getPosition();
            glm::vec3 glmNeighborPos(neighborPos.m_x, neighborPos.m_y, neighborPos.m_z);
            
            glm::vec3 boidDistance = glmCurrentPos - glmNeighborPos;
            
            if(glm::length(boidDistance) < flockDistance) {
                separation -= (glmCurrentPos - glmNeighborPos);
            }
        }
    }
    forces.separation = separation;
    
    // Calculate combined force - get actual parameters from the legacy behaviour instance
    float separationForce = legacyBehaviours->getSeparationForce();
    float cohesionForce = legacyBehaviours->getCohesionForce();  
    float alignmentParam = legacyBehaviours->getAlignment();
    
    // Use exact legacy combination logic from BehaviourSetup()
    glm::vec3 separationCorrection(-1.0f, -1.0f, -1.0f);
    glm::vec3 separationSet = (separationForce * separation) * separationCorrection;
    glm::vec3 cohesionSet = cohesionForce * coherence;
    glm::vec3 alignmentSet = alignmentForce * alignmentParam;  // Note: alignmentForce * m_alignment
    
    glm::vec3 behaviourSetup = separationSet + cohesionSet + alignmentSet;
    
    if (glm::length(behaviourSetup) > 0.5f) {
        behaviourSetup = glm::normalize(behaviourSetup);
        behaviourSetup *= 0.5f;
    }
    
    forces.combined = behaviourSetup;
    
    return forces;
}

BehaviorValidator::FlockingForces BehaviorValidator::extractLegacyForces(Behaviours* legacyBehaviours)
{
    FlockingForces forces;
    
    // Extract forces from legacy Behaviours class using the getter methods
    ngl::Vector coherence = legacyBehaviours->getCoherence();
    ngl::Vector alignment = legacyBehaviours->getAlignmentForce();
    ngl::Vector separation = legacyBehaviours->getSeparation();
    
    // Call BehaviourSetup() to get the properly combined force
    ngl::Vector combined = legacyBehaviours->BehaviourSetup();
    
    // Convert to GLM for comparison
    forces.cohesion = glm::vec3(coherence.m_x, coherence.m_y, coherence.m_z);
    forces.alignment = glm::vec3(alignment.m_x, alignment.m_y, alignment.m_z);
    forces.separation = glm::vec3(separation.m_x, separation.m_y, separation.m_z);
    forces.combined = glm::vec3(combined.m_x, combined.m_y, combined.m_z);
    
    return forces;
}

bool BehaviorValidator::compareForcesWithTolerance(const FlockingForces& legacy,
                                                 const FlockingForces& modern,
                                                 float tolerance)
{
    auto compareVec3 = [tolerance](const glm::vec3& a, const glm::vec3& b) -> bool {
        return (std::abs(a.x - b.x) < tolerance &&
                std::abs(a.y - b.y) < tolerance &&
                std::abs(a.z - b.z) < tolerance);
    };
    
    return compareVec3(legacy.cohesion, modern.cohesion) &&
           compareVec3(legacy.alignment, modern.alignment) &&
           compareVec3(legacy.separation, modern.separation) &&
           compareVec3(legacy.combined, modern.combined);
}

void BehaviorValidator::printForceComparison(const FlockingForces& legacy,
                                           const FlockingForces& modern)
{
    std::cout << std::fixed << std::setprecision(4);
    
    std::cout << "\nForce Comparison:" << std::endl;
    std::cout << "                Legacy              Modern              Difference" << std::endl;
    std::cout << "Cohesion:   (" << std::setw(6) << legacy.cohesion.x << "," << std::setw(6) << legacy.cohesion.y << "," << std::setw(6) << legacy.cohesion.z << ") "
              << " (" << std::setw(6) << modern.cohesion.x << "," << std::setw(6) << modern.cohesion.y << "," << std::setw(6) << modern.cohesion.z << ") "
              << " (" << std::setw(6) << (legacy.cohesion.x - modern.cohesion.x) << "," << std::setw(6) << (legacy.cohesion.y - modern.cohesion.y) << "," << std::setw(6) << (legacy.cohesion.z - modern.cohesion.z) << ")" << std::endl;
              
    std::cout << "Alignment:  (" << std::setw(6) << legacy.alignment.x << "," << std::setw(6) << legacy.alignment.y << "," << std::setw(6) << legacy.alignment.z << ") "
              << " (" << std::setw(6) << modern.alignment.x << "," << std::setw(6) << modern.alignment.y << "," << std::setw(6) << modern.alignment.z << ") "
              << " (" << std::setw(6) << (legacy.alignment.x - modern.alignment.x) << "," << std::setw(6) << (legacy.alignment.y - modern.alignment.y) << "," << std::setw(6) << (legacy.alignment.z - modern.alignment.z) << ")" << std::endl;
              
    std::cout << "Separation: (" << std::setw(6) << legacy.separation.x << "," << std::setw(6) << legacy.separation.y << "," << std::setw(6) << legacy.separation.z << ") "
              << " (" << std::setw(6) << modern.separation.x << "," << std::setw(6) << modern.separation.y << "," << std::setw(6) << modern.separation.z << ") "
              << " (" << std::setw(6) << (legacy.separation.x - modern.separation.x) << "," << std::setw(6) << (legacy.separation.y - modern.separation.y) << "," << std::setw(6) << (legacy.separation.z - modern.separation.z) << ")" << std::endl;
              
    std::cout << "Combined:   (" << std::setw(6) << legacy.combined.x << "," << std::setw(6) << legacy.combined.y << "," << std::setw(6) << legacy.combined.z << ") "
              << " (" << std::setw(6) << modern.combined.x << "," << std::setw(6) << modern.combined.y << "," << std::setw(6) << modern.combined.z << ") "
              << " (" << std::setw(6) << (legacy.combined.x - modern.combined.x) << "," << std::setw(6) << (legacy.combined.y - modern.combined.y) << "," << std::setw(6) << (legacy.combined.z - modern.combined.z) << ")" << std::endl;
}
