#include "Behaviours.h"
#include "FlockTypes.h"  // Add modern types

Behaviours::Behaviours()
{
    m_BehaviourDistance = 20;
    m_flockDistance = 4;
    m_seperationForce = 9;
    m_alignment = 10;
    m_cohesionForce = 2;
}
//----------------------------------------------------------------------------------------------------------------------
void Behaviours::Cohesion(int &_boidNumber, std::vector <Boid*> & _boidList)
{
    m_coherence.set(0, 0, 0);
    m_boidDistance.set(0, 0, 0);
    int count = 1;

    for(int i=0;i<_boidList.size();i++)
    {
        if(i!=_boidNumber)
        {
            m_boidDistance = _boidList.at(_boidNumber)->getPosition() - _boidList.at(i)->getPosition();

            if(m_boidDistance.length() < m_BehaviourDistance)
            {
                m_coherence += _boidList.at(i)->getPosition();
                count++;
            }
        }
    }
    m_coherence /= count;
    m_coherence = (m_coherence - _boidList.at(_boidNumber)->getPosition());
    m_coherence.normalize();
}


//----------------------------------------------------------------------------------------------------------------------
void Behaviours::Alignment(int &_boidNumber, std::vector<Boid*> &_boidList)
{
    int count = 1;
    m_boidDistance.set(0, 0, 0);

    for(int i=0;i<_boidList.size();i++)
    {
        if(i!=_boidNumber)
        {
            m_boidDistance = _boidList.at(_boidNumber)->getPosition() - _boidList.at(i)->getPosition();

            if(m_boidDistance.length() < m_BehaviourDistance)
            {
                m_alignmentForce += _boidList.at(i)->getVelocity();
                count++;
            }
        }
    }

    if (m_alignmentForce.length() > m_BehaviourDistance)
    {
        m_alignmentForce.normalize();
    }

    m_alignmentForce /= count;
    m_alignmentForce = (m_alignmentForce - _boidList.at(_boidNumber)->getVelocity());


}
//----------------------------------------------------------------------------------------------------------------------
void Behaviours::Seperation(int &_boidNumber, std::vector<Boid*> &_boidList)
{
    m_separation.set(0, 0, 0);
    m_boidDistance.set(0, 0, 0);

    for(int i=0;i<_boidList.size();i++)
    {
        if(i!=_boidNumber)
        {
            m_boidDistance = _boidList.at(_boidNumber)->getPosition() - _boidList.at(i)->getPosition();

            if(m_boidDistance.length() < m_flockDistance)
            {
                m_separation -= (_boidList.at(_boidNumber)->getPosition() - _boidList.at(i)->getPosition());
            }
        }
    }
    if (m_boidDistance.length() > m_flockDistance)
    {
        m_boidDistance.normalize();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Behaviours::Destination(int & _boidNumber, std::vector <Boid*> & _boidList)
{
    Vector targeting(1,0,1);

    targeting = ((targeting   - _boidList.at(_boidNumber)->getNextPosition()) * - 100);
}
//----------------------------------------------------------------------------------------------------------------------

Vector Behaviours::BehaviourSetup()
{

    Vector m_seperationCorrection(-1, -1, -1);
    m_behaviourSetup.set(0, 0, 0);


    m_seperationSet = (m_seperationForce * m_separation) * m_seperationCorrection;
    m_cohesionSet = m_cohesionForce * m_coherence;
    m_alighmentSet = m_alignmentForce * m_alignment;
    m_behaviourSetup = m_seperationSet + m_cohesionSet + m_alighmentSet;

    if (m_behaviourSetup.length() > 0.5)
    {
        m_behaviourSetup.normalize();
        m_behaviourSetup *= 0.5;
    }


    return m_behaviourSetup;





}

//----------------------------------------------------------------------------------------------------------------------
// Modern flocking helper using GLM-based calculations
//----------------------------------------------------------------------------------------------------------------------
void Behaviours::applyModernFlockingForce(int &_boidNumber, std::vector<Boid*> &_boidList)
{
    if (_boidList.empty() || _boidNumber < 0 || _boidNumber >= static_cast<int>(_boidList.size())) {
        return;
    }
    
    // Collect neighbor data using modern accessors
    std::vector<flock::Vec3> neighborPositions;
    std::vector<flock::Vec3> neighborVelocities;
    
    Boid* currentBoid = _boidList[_boidNumber];
    flock::Vec3 currentPos = currentBoid->getPositionModern();
    
    for (size_t i = 0; i < _boidList.size(); ++i) {
        if (static_cast<int>(i) != _boidNumber) {
            flock::Vec3 neighborPos = _boidList[i]->getPositionModern();
            float distance = flock::Utils::distance(currentPos, neighborPos);
            
            if (distance < m_BehaviourDistance) {
                neighborPositions.push_back(neighborPos);
                neighborVelocities.push_back(_boidList[i]->getVelocityModern());
            }
        }
    }
    
    // Use modern flocking calculation
    if (!neighborPositions.empty()) {
        flock::Vec3 modernForce = currentBoid->calculateModernFlocking(neighborPositions, neighborVelocities);
        
        // Apply the modern force back to legacy velocity
        // Scale down the force to prevent overly dramatic changes
        modernForce *= 0.1f;  
        
        Vector legacyForce(modernForce.x, modernForce.y, modernForce.z);
        currentBoid->addVelocity(legacyForce);
    }
}

//----------------------------------------------------------------------------------------------------------------------
// Modern GLM-based method implementations
//----------------------------------------------------------------------------------------------------------------------

flock::Vec3 Behaviours::calculateCohesionModern(const flock::Vec3& boidPos, 
                                               const std::vector<flock::Vec3>& neighborPositions) const
{
    if (neighborPositions.empty()) {
        return flock::Vec3(0.0f);
    }
    
    // Calculate center of mass of neighbors
    flock::Vec3 centerOfMass(0.0f);
    for (const auto& pos : neighborPositions) {
        centerOfMass += pos;
    }
    centerOfMass /= static_cast<float>(neighborPositions.size());
    
    // Return force towards center of mass, scaled by cohesion strength
    return (centerOfMass - boidPos) * static_cast<float>(m_cohesionForce);
}

//----------------------------------------------------------------------------------------------------------------------
flock::Vec3 Behaviours::calculateSeparationModern(const flock::Vec3& boidPos, 
                                                 const std::vector<flock::Vec3>& neighborPositions) const
{
    flock::Vec3 separationForce(0.0f);
    int count = 0;
    
    for (const auto& neighborPos : neighborPositions) {
        float distance = glm::length(neighborPos - boidPos);
        if (distance > 0.0f && distance < m_flockDistance) {
            // Calculate repulsion force (inversely proportional to distance)
            flock::Vec3 diff = boidPos - neighborPos;
            diff = glm::normalize(diff);
            diff /= distance; // Weight by distance
            separationForce += diff;
            count++;
        }
    }
    
    if (count > 0) {
        separationForce /= static_cast<float>(count);
        separationForce *= static_cast<float>(m_seperationForce);
    }
    
    return separationForce;
}

//----------------------------------------------------------------------------------------------------------------------
flock::Vec3 Behaviours::calculateAlignmentModern(const flock::Vec3& boidVel, 
                                                const std::vector<flock::Vec3>& neighborVelocities) const
{
    if (neighborVelocities.empty()) {
        return flock::Vec3(0.0f);
    }
    
    // Calculate average velocity of neighbors
    flock::Vec3 averageVelocity(0.0f);
    for (const auto& vel : neighborVelocities) {
        averageVelocity += vel;
    }
    averageVelocity /= static_cast<float>(neighborVelocities.size());
    
    // Return force to align with average velocity
    return (averageVelocity - boidVel) * static_cast<float>(m_alignment);
}

//----------------------------------------------------------------------------------------------------------------------
