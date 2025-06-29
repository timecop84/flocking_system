#include "flock.h"
#include "Matrix.h"
#include "ShaderLib.h"
#include "LegacyRandom.h"
#include "QDebug"
#include <iostream>



//----------------------------------------------------------------------------------------------------------------------
const static int s_extents=5;  // Original legacy value - spawn in small central area
//----------------------------------------------------------------------------------------------------------------------
Flock::Flock(BBox *bbox, Obstacle *obstacle)
{
    m_behaviours = new Behaviours();
    m_bbox = bbox;
    m_numberOfBoids = 200;
    m_checkSphereSphere=true;
    m_obstacle = obstacle;
    m_speedMultiplier = 1.0f;  // Default speed multiplier

    resetBoids();
}
//----------------------------------------------------------------------------------------------------------------------

void Flock::draw(const std::string &_shaderName, TransformStack &_transformStack, Camera *_cam)const
{
    ShaderLib *shader=ShaderLib::instance();
    shader->use(_shaderName);
    _transformStack.pushTransform();

    loadMatricesToShader(_transformStack, _cam);


    for(Boid *b : m_boidList)
    {
        b->draw(_shaderName,_transformStack,_cam);
    }

    _transformStack.popTransform();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

}
//----------------------------------------------------------------------------------------------------------------------
void Flock::loadMatricesToShader(TransformStack &_tx, Camera *_cam) const

{
    ShaderLib *shader = ShaderLib::instance();
    Matrix MV;
    Matrix MVP;
    Mat3x3 normalMatrix;
    Matrix M;

    M = _tx.getCurrentTransform();
    MV = _tx.getCurrAndGlobal().getMatrix() *_cam->getViewMatrix();
    MVP = MV * _cam->getProjectionMatrix();
    normalMatrix = MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMatrix("MV", MV);
    shader->setShaderParamFromMatrix("MVP", MVP);
    shader->setShaderParamFromMat3x3("normalMatrix", normalMatrix);
    shader->setShaderParamFromMatrix("M", M);

}
//-----------------------------------------------------------------------------------------------------------------------
void Flock::addBoids()
{
    if (m_numberOfBoids <= 1990)
    {
        Random *rng=Random::instance();
        Vector dir;
        dir=rng->getRandomVector();
        // add the spheres to the end of the particle list
        for(int i=0; i<10; i++)
        {
            //std::cout<<"adding boid"<<endl;
            m_boidList.push_back(new Boid(rng->getRandomPoint(s_extents,s_extents,s_extents),dir));

            ++m_numberOfBoids;
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------
void Flock::removeBoids()
{
    if (m_numberOfBoids > 10)
    {
        for (int i = 0; i < 10; i++)
        {
            m_boidList.pop_back();
            --m_numberOfBoids;
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------
void Flock::resetBoids()
{
    m_boidList.clear();
    Vector dir;
    Random *rng=Random::instance();
    for(int i=0; i<m_numberOfBoids; ++i)
    {
        dir=rng->getRandomVector();
        m_boidList.push_back(new Boid(rng->getRandomPoint(s_extents,s_extents,s_extents),dir));
    }
}
//----------------------------------------------------------------------------------------------------------------------

void Flock::update()
{
    // Modern flocking update using pure GLM that exactly replicates legacy emergent behavior
    static int modernFrameCount = 0;
    modernFrameCount++;
    
    // Only print debug info every 300 frames to reduce spam
    if (modernFrameCount % 300 == 1) {
        std::cout << "Using modern GLM-based flocking update (frame " << modernFrameCount << ")..." << std::endl;
    }
    
    // Handle collisions using legacy system for compatibility
    checkCollisions();
    
    // Apply modern flocking behavior to all boids using pure GLM calculations
    // This implementation exactly replicates the legacy calculations but uses GLM
    int boidIndex = 0;
    for(Boid *boid : m_boidList)
    {
        // Get current boid state
        Vector currentPos = boid->getPosition();
        Vector currentVel = boid->getVelocity();
        glm::vec3 glmCurrentPos(currentPos.m_x, currentPos.m_y, currentPos.m_z);
        glm::vec3 glmCurrentVel(currentVel.m_x, currentVel.m_y, currentVel.m_z);
        
        // COHESION - exactly match legacy Behaviours::Cohesion() calculation
        glm::vec3 coherence(0.0f, 0.0f, 0.0f);
        int count = 1; // Legacy starts with 1
        
        for(int i = 0; i < static_cast<int>(m_boidList.size()); i++)
        {
            if(i != boidIndex)
            {
                Vector neighborPos = m_boidList[i]->getPosition();
                glm::vec3 glmNeighborPos(neighborPos.m_x, neighborPos.m_y, neighborPos.m_z);
                
                glm::vec3 boidDistance = glmCurrentPos - glmNeighborPos;
                
                if(glm::length(boidDistance) < m_behaviours->getBehaviourDistance())
                {
                    coherence += glmNeighborPos;
                    count++;
                }
            }
        }
        
        coherence /= static_cast<float>(count);
        coherence = coherence - glmCurrentPos;
        if (glm::length(coherence) > 0.0001f) {
            coherence = glm::normalize(coherence);
        } else {
            coherence = glm::vec3(0.0f); // Avoid normalizing zero vector
        }
        
        // ALIGNMENT - exactly match legacy Behaviours::Alignment() calculation
        glm::vec3 alignmentForce(0.0f, 0.0f, 0.0f);
        count = 1; // Reset count for alignment, legacy starts with 1
        
        for(int i = 0; i < static_cast<int>(m_boidList.size()); i++)
        {
            if(i != boidIndex)
            {
                Vector neighborPos = m_boidList[i]->getPosition();
                glm::vec3 glmNeighborPos(neighborPos.m_x, neighborPos.m_y, neighborPos.m_z);
                
                glm::vec3 boidDistance = glmCurrentPos - glmNeighborPos;
                
                if(glm::length(boidDistance) < m_behaviours->getBehaviourDistance())
                {
                    Vector neighborVel = m_boidList[i]->getVelocity();
                    glm::vec3 glmNeighborVel(neighborVel.m_x, neighborVel.m_y, neighborVel.m_z);
                    alignmentForce += glmNeighborVel;
                    count++;
                }
            }
        }
        
        if (glm::length(alignmentForce) > m_behaviours->getBehaviourDistance())
        {
            if (glm::length(alignmentForce) > 0.0001f) {
                alignmentForce = glm::normalize(alignmentForce);
            } else {
                alignmentForce = glm::vec3(0.0f); // Avoid normalizing zero vector
            }
        }
        alignmentForce /= static_cast<float>(count);
        alignmentForce = alignmentForce - glmCurrentVel;
        
        // SEPARATION - exactly match legacy Behaviours::Seperation() calculation
        glm::vec3 separation(0.0f, 0.0f, 0.0f);
        
        for(int i = 0; i < static_cast<int>(m_boidList.size()); i++)
        {
            if(i != boidIndex)
            {
                Vector neighborPos = m_boidList[i]->getPosition();
                glm::vec3 glmNeighborPos(neighborPos.m_x, neighborPos.m_y, neighborPos.m_z);
                
                glm::vec3 boidDistance = glmCurrentPos - glmNeighborPos;
                
                if(glm::length(boidDistance) < m_behaviours->getFlockDistance())
                {
                    separation -= (glmCurrentPos - glmNeighborPos);
                }
            }
        }
        
        // BEHAVIOR SETUP - exactly match legacy Behaviours::BehaviourSetup() calculation
        glm::vec3 separationCorrection(-1.0f, -1.0f, -1.0f);
        glm::vec3 separationSet = (static_cast<float>(m_behaviours->getSeparationForce()) * separation) * separationCorrection;
        glm::vec3 cohesionSet = static_cast<float>(m_behaviours->getCohesionForce()) * coherence;
        glm::vec3 alignmentSet = alignmentForce * static_cast<float>(m_behaviours->getAlignment());
        
        glm::vec3 behaviourSetup = separationSet + cohesionSet + alignmentSet;
        
        if (glm::length(behaviourSetup) > 0.5f)
        {
            if (glm::length(behaviourSetup) > 0.0001f) {
                behaviourSetup = glm::normalize(behaviourSetup);
                behaviourSetup *= 0.5f;
            } else {
                behaviourSetup = glm::vec3(0.0f); // Avoid normalizing zero vector
            }
        }
        
        // Convert back to ngl format and apply using exact legacy sequence
        Vector nglBehaviourSetup(behaviourSetup.x, behaviourSetup.y, behaviourSetup.z);
        
        // Apply speed multiplier to the behavior forces
        nglBehaviourSetup = nglBehaviourSetup * m_speedMultiplier;
        
        // Update boid using exact legacy interface sequence
        boid->updateVelocity(nglBehaviourSetup);
        boid->velocityConstraint();
        boid->boidDirection();
        
        boidIndex++;
    }
    
    if (modernFrameCount % 300 == 1) {
        std::cout << "Modern GLM-based update completed using pure GLM calculations for " << m_boidList.size() << " boids" << std::endl;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setBoidSize(double size)
{
    for(Boid *s : m_boidList)
    {
        s->setScale(Vector(size, size, size));
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setColour(Colour colour)
{
    for(Boid *s : m_boidList)
    {
        s->setColour(colour);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setWireframe(bool value)
{
    for(Boid *s : m_boidList)
    {
        s->setWireframe(value);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setSpeedMultiplier(float multiplier)
{
    m_speedMultiplier = multiplier;
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setSimDistance(double distance)
{
    m_behaviours->setBehaviourDistance(distance);
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setSimFlockDistance(double distance)
{
    m_behaviours->setFlockDistance(distance);
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setSimCohesion(double cohesion)
{
    m_behaviours->setCohesionForce(cohesion);
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setSimSeparation(double separation)
{
    m_behaviours->setSeparationForce(separation);
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setSimAlignment(double alignment)
{
    m_behaviours->setAlignment(alignment);
}
//----------------------------------------------------------------------------------------------------------------------

/// The following section is modified from :-
/// John Macey(2011).Collisions Example, BoundingBox. [Accessed 2012]
/// Available from: bzr branch http://nccastaff.bournemouth.ac.uk/jmacey/Code/Collisions
void Flock::validateBoundingBoxCollision()
{
    //create an array of the extents of the bounding box
    float ext[6];
    ext[0]=ext[1]=(m_bbox->height()/2.0f);
    ext[2]=ext[3]=(m_bbox->width()/2.0f);
    ext[4]=ext[5]=(m_bbox->depth()/2.0f);
    // Dot product needs a Vector so we convert The Point Temp into a Vector so we can
    // do a dot product on it
    Vector point;
    // D is the distance of the Agent from the Plane. If it is less than ext[i] then there is
    // no collision
    GLfloat Distance;
    // Loop for each sphere in the vector list
    for(Boid *s : m_boidList)
    {
        point=s->getPosition();
        //Now we need to check the Sphere agains all 6 planes of the BBOx
        //If a collision is found we change the dir of the Sphere then Break
        for(int i=0; i<6; ++i)
        {
            //to calculate the distance we take the dotporduct of the Plane Normal
            //with the new point P
            Distance=m_bbox->getNormalArray()[i].dot(point);
            //Now Add the Radius of the sphere to the offsett
            Distance+=s->getSize();
            // If this is greater or equal to the BBox extent /2 then there is a collision
            //So we calculate the Spheres new direction
            if(Distance >=ext[i])
            {
                //We use the same calculation as in raytracing to determine the
                // the new direction
                GLfloat x= 2*( s->getVelocity().dot((m_bbox->getNormalArray()[i])));
                Vector d =m_bbox->getNormalArray()[i]*x;
                s->setVelocity(s->getNextPosition()-d * 5.0);
                s->isHit();
            }//end of hit test
        }//end of each face test
    }//end of for
}
/// end of Citation
//----------------------------------------------------------------------------------------------------------------------
/// The following section is modified from :-
/// John Macey(2011).Collisions Example, BoundingBox. [Accessed 2012]
/// Available from: bzr branch http://nccastaff.bournemouth.ac.uk/jmacey/Code/Collisions
bool Flock::sphereSphereCollision(
        Vector _pos,
        GLfloat _rad,
        Vector _pos1,
        GLfloat _rad1
        )
{
    // the relative position of the spheres
    Vector relPos;
    //min an max distances of the spheres
    GLfloat dist;
    GLfloat minDist;
    GLfloat len;
    relPos =_pos-_pos1;
    // and the distance
    len=relPos.length();
    dist=len*len;
    minDist =_rad+_rad1;
    // if it is a hit
    if(dist <=(3 * (minDist * minDist)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/// end of citation
//----------------------------------------------------------------------------------------------------------------------
void  Flock::checkSphereCollisions()
{
    unsigned int size=m_boidList.size();
    GLfloat obstacleRadius = m_obstacle->getSphereRadius();
    Vector obstaclePos = m_obstacle->getSpherePosition();

    for(unsigned int Current=0; Current<size; ++Current)
    {
        Vector boidPos = m_boidList.at(Current)->getPosition();
        Vector toObstacle = obstaclePos - boidPos;
        GLfloat distance = toObstacle.length();
        
        // Define avoidance zones
        GLfloat avoidanceRadius = obstacleRadius * 2.5f;  // Start avoiding at this distance
        GLfloat collisionRadius = obstacleRadius * 1.2f;  // Hard collision boundary
        
        if (distance < avoidanceRadius && distance > 0.0001f) {
            // Calculate smooth repulsion force
            Vector repulsionDir = boidPos - obstaclePos;
            repulsionDir.normalize();
            
            // Force strength inversely proportional to distance (stronger when closer)
            GLfloat forceStrength = (avoidanceRadius - distance) / avoidanceRadius;
            forceStrength = forceStrength * forceStrength; // Quadratic falloff for smoother transition
            
            // Apply gentle repulsion force (add to existing velocity, don't replace it)
            Vector repulsionForce = repulsionDir * (forceStrength * 0.3f);
            m_boidList.at(Current)->addVelocity(repulsionForce);
            
            // Hard collision: push boid away if too close
            if (distance < collisionRadius) {
                Vector newPos = obstaclePos + (repulsionDir * collisionRadius);
                m_boidList.at(Current)->setPosition(newPos);
                m_boidList.at(Current)->setHit();
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
void  Flock::checkCollisions()
{
    if(m_checkSphereSphere == true)
    {
        checkSphereCollisions();
    }

    validateBoundingBoxCollision();
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Flock::demonstrateModernFlocking()
{
    // This method demonstrates the modern GLM-based flocking system
    std::cout << "\n=== Modern Flocking Demonstration ===" << std::endl;
    std::cout << "Modern GLM-based flocking is now active!" << std::endl;
    std::cout << "Flock size: " << m_boidList.size() << " boids" << std::endl;
    
    if (!m_boidList.empty()) {
        // Demonstrate accessing boid data in modern format
        Boid* firstBoid = m_boidList[0];
        Vector pos = firstBoid->getPosition();
        Vector vel = firstBoid->getVelocity();
        
        std::cout << "Sample boid position: (" << pos.m_x << ", " << pos.m_y << ", " << pos.m_z << ")" << std::endl;
        std::cout << "Sample boid velocity: (" << vel.m_x << ", " << vel.m_y << ", " << vel.m_z << ")" << std::endl;
        std::cout << "Modern flocking logic can now access and process this data using GLM types!" << std::endl;
    }
    
    std::cout << "======================================\n" << std::endl;
}
//----------------------------------------------------------------------------------------------------------------------

