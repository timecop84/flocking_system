#include "flock.h"
#include "Matrix.h"
#include "ShaderLib.h"
#include "MathUtils.h"
#include <iostream>
#include <cstdlib>  // For rand()
#include <ctime>    // For time-based seeding
#include "PerformanceProfiler.h"

// Obstacle avoidance tuning parameters
constexpr float OBSTACLE_AVOIDANCE_RADIUS_SCALE = 3.0f;   // Was 2.5f, increase for earlier avoidance
constexpr float OBSTACLE_COLLISION_RADIUS_SCALE = 1.3f;   // Was 1.2f, increase for more buffer
constexpr float OBSTACLE_REPULSION_FORCE = 0.45f;         // Was 0.3f, increase for stronger avoidance

//----------------------------------------------------------------------------------------------------------------------
const static int s_extents=5;  // Original legacy value - spawn in small central area
//----------------------------------------------------------------------------------------------------------------------
Flock::Flock(BBox *bbox, Obstacle *obstacle) : m_spatialGrid(15.0f) // Cell size optimized for typical flock behavior distances
{
    m_behaviours = new Behaviours();
    m_bbox = bbox;
    m_numberOfBoids = 200;
    m_checkSphereSphere=true;
    m_obstacle = obstacle;
    m_speedMultiplier = 1.0f;  // Default speed multiplier

    // Seed random number generator for swarm randomness
    srand(static_cast<unsigned int>(time(nullptr)));
    
    std::cout << "=== SWARM-OF-FLIES BEHAVIOR ENABLED ===" << std::endl;
    std::cout << "Behavior: Tighter grouping with chaotic individual movement" << std::endl;
    std::cout << "Parameters: Low alignment, high cohesion, random jitter" << std::endl;
    std::cout << "=======================================" << std::endl;

    resetBoids();
}
//----------------------------------------------------------------------------------------------------------------------

void Flock::draw(const std::string &_shaderName, TransformStack &_transformStack, Camera *_cam)const
{
    PROFILE_SCOPE("Flock::draw");
    
    // For debugging only - can be removed in production
    if (_shaderName != "Phong") {
        std::cerr << "Warning: Flock only supports Phong shader in modern mode, got: " << _shaderName << std::endl;
    }
    
    // Use modern UBO-based rendering pipeline
    ShaderLib *shader = ShaderLib::instance();
    shader->use(_shaderName);
    
    _transformStack.pushTransform();

    // Render each boid with individual transforms and materials
    {
        PROFILE_SCOPE("Individual Boid Rendering");
        for(Boid *b : m_boidList)
        {
            // Push a new transform level for each boid
            _transformStack.pushTransform();
            {
                // Set up the boid's transform (translate to boid position and scale)
                flock::Vec3 boidPos = b->getPositionModern();
                Matrix boidTransform;
                boidTransform.identity();
                boidTransform.translate(boidPos.x, boidPos.y, boidPos.z);
                
                // Scale the boid (same size as in immediate mode)
                // Use each boid's randomized scale
                Vector scaleVec = b->getScale();
                boidTransform.scale(scaleVec.m_x * 2.0f, scaleVec.m_y * 2.0f, scaleVec.m_z * 2.0f);
                
                _transformStack.setModel(boidTransform.getGLMMat4());
                
                // Note: Matrix UBO update should be handled by the main rendering loop
                // Material UBO should also be updated by the main rendering loop
                
                // Render the boid using the modern pipeline
                b->draw(_shaderName, _transformStack, _cam);
            }
            _transformStack.popTransform();
        }
    }

    _transformStack.popTransform();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}
//----------------------------------------------------------------------------------------------------------------------
// Legacy function removed - UBO-based rendering handles matrix updates automatically
//----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
void Flock::addBoids()
{
    if (m_numberOfBoids <= 1990)
    {
        Vector dir;
        dir.m_x = math::utils::randomFloat();
        dir.m_y = math::utils::randomFloat();
        dir.m_z = math::utils::randomFloat();
        // add the spheres to the end of the particle list
        for(int i=0; i<10; i++)
        {
            glm::vec3 pos = math::utils::randomPoint(s_extents, s_extents, s_extents);
            Boid* boid = new Boid(Vector(pos.x, pos.y, pos.z), dir);
            float scale = 0.7f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
            boid->setScale(Vector(scale, scale, scale));
            m_boidList.push_back(boid);
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
    for(int i=0; i<m_numberOfBoids; ++i)
    {
        glm::vec3 dirVec = math::utils::randomVec3();
        dir.m_x = dirVec.x;
        dir.m_y = dirVec.y;
        dir.m_z = dirVec.z;
        
        glm::vec3 pos = math::utils::randomPoint(s_extents, s_extents, s_extents);
        Boid* boid = new Boid(Vector(pos.x, pos.y, pos.z), dir);
        // Randomize scale between 0.7 and 1.5 (can be adjusted)
        float scale = 0.7f + static_cast<float>(rand()) / RAND_MAX * 0.8f;
        boid->setScale(Vector(scale, scale, scale));
        m_boidList.push_back(boid);
    }
}
//----------------------------------------------------------------------------------------------------------------------

void Flock::update()
{
    PROFILE_SCOPE("Flock::update");
    
    // High-performance flocking update using spatial partitioning
    // This reduces complexity from O(N²) to approximately O(N)
    
    // Handle collisions using legacy system for compatibility
    {
        PROFILE_SCOPE("Collision Detection");
        checkCollisions();
    }
    
    // Step 1: Build spatial hash grid for efficient neighbor queries
    {
        PROFILE_SCOPE("Spatial Grid Build");
        m_spatialGrid.clear();
        for(int i = 0; i < static_cast<int>(m_boidList.size()); i++) {
            m_spatialGrid.addBoid(m_boidList[i], i);
        }
    }
    
    // Step 2: Apply optimized flocking behavior to all boids
    {
        PROFILE_SCOPE("Flocking Behavior Update");
        
        // Cache behavior distances for faster access
        const float behaviorDistance = static_cast<float>(m_behaviours->getBehaviourDistance());
        const float flockDistance = static_cast<float>(m_behaviours->getFlockDistance());
        
        // Cache behavior forces for faster access
        const float separationForce = static_cast<float>(m_behaviours->getSeparationForce());
        const float cohesionForce = static_cast<float>(m_behaviours->getCohesionForce());
        const float alignmentForce = static_cast<float>(m_behaviours->getAlignment());
        
        // Optimization: Pre-calculate squared distances to avoid sqrt() calls
        const float behaviorDistanceSq = behaviorDistance * behaviorDistance;
        const float flockDistanceSq = flockDistance * flockDistance;
        
        // Early termination limits for better performance with large flocks
        // const int maxNeighborsForBehavior = 15; // Removed - process all neighbors for natural behavior
        
        // OPTIMIZATION: Pre-cache all boid positions and velocities in glm format to reduce conversions
        const size_t boidCount = m_boidList.size();
        std::vector<glm::vec3> boidPositions(boidCount);
        std::vector<glm::vec3> boidVelocities(boidCount);
        
        for(size_t i = 0; i < boidCount; i++) {
            Vector pos = m_boidList[i]->getPosition();
            Vector vel = m_boidList[i]->getVelocity();
            boidPositions[i] = glm::vec3(pos.m_x, pos.m_y, pos.m_z);
            boidVelocities[i] = glm::vec3(vel.m_x, vel.m_y, vel.m_z);
        }
        
        int boidIndex = 0;
        for(Boid *boid : m_boidList)
        {
            // Use pre-cached positions (significant performance improvement)
            const glm::vec3& glmCurrentPos = boidPositions[boidIndex];
            
            // Get nearby boids using spatial partitioning
            std::vector<std::pair<Boid*, int>> nearbyBoids = m_spatialGrid.getNearbyBoids(glmCurrentPos, std::max(behaviorDistance, flockDistance));
            
            // Early exit if no neighbors (optimization for sparse areas)
            if(nearbyBoids.empty()) {
                boidIndex++;
                continue;
            }
            
            // Initialize behavior accumulators
            glm::vec3 coherence(0.0f);
            glm::vec3 alignmentSum(0.0f);
            glm::vec3 separation(0.0f);
            int cohesionCount = 0; // Start with 0, only count actual neighbors
            int alignmentCount = 0; // Start with 0, only count actual neighbors
            
            // SINGLE PASS: Calculate all behaviors simultaneously with optimized distance checks
            for(const auto& boidPair : nearbyBoids) {
                const int neighborIndex = boidPair.second;
                if(neighborIndex != boidIndex) // Don't include self
                {
                    // Use pre-cached positions and velocities (avoid repeated conversions)
                    const glm::vec3& glmNeighborPos = boidPositions[neighborIndex];
                    const glm::vec3& glmNeighborVel = boidVelocities[neighborIndex];
                    
                    // Calculate distance vector once per neighbor
                    glm::vec3 boidDistance = glmCurrentPos - glmNeighborPos;
                    
                    // Use squared distance to avoid expensive sqrt() calls
                    float distanceSq = glm::dot(boidDistance, boidDistance);
                    
                    // COHESION & ALIGNMENT: Check if within behavior distance (squared)
                    if(distanceSq < behaviorDistanceSq && distanceSq > 0.001f) // Avoid division by zero
                    {
                        coherence += glmNeighborPos;
                        cohesionCount++;
                        
                        // ALIGNMENT: Same distance check, so calculate here
                        alignmentSum += glmNeighborVel;
                        alignmentCount++;
                    }
                    
                    // SEPARATION: Check if within flock distance (squared)
                    if(distanceSq < flockDistanceSq && distanceSq > 0.001f)
                    {
                        // Apply distance-based separation force (no sqrt needed)
                        float separationStrength = (flockDistanceSq - distanceSq) / flockDistanceSq;
                        glm::vec3 separationForce = boidDistance * separationStrength;
                        separation += separationForce;
                    }
                }
            }
            
            // COHESION: Finalize calculation (compute centroid first, then direction)
            if (cohesionCount > 0) {
                coherence /= static_cast<float>(cohesionCount);
                coherence = coherence - glmCurrentPos;  // Direction toward centroid
                if (glm::length(coherence) > 0.0001f) {
                    coherence = glm::normalize(coherence);
                } else {
                    coherence = glm::vec3(0.0f);
                }
            } else {
                coherence = glm::vec3(0.0f);
            }
            
            // ALIGNMENT: Finalize calculation with reduced influence for chaotic movement
            if (alignmentCount > 0) {
                alignmentSum /= static_cast<float>(alignmentCount);
                // Use average velocity direction but weaken it significantly for swarm behavior
                if (glm::length(alignmentSum) > 0.0001f) {
                    alignmentSum = glm::normalize(alignmentSum);
                    // Reduce alignment strength by 50% for more chaotic movement
                    alignmentSum *= 0.5f;
                } else {
                    alignmentSum = glm::vec3(0.0f);
                }
            } else {
                alignmentSum = glm::vec3(0.0f);
            }
            
            // BEHAVIOR SETUP: Combine all forces with swarm-style weighting
            glm::vec3 separationSet = separation * separationForce;
            glm::vec3 cohesionSet = coherence * cohesionForce;
            glm::vec3 alignmentSet = alignmentSum * alignmentForce;
            
            // Add random jitter for fly-like chaotic movement
            glm::vec3 randomJitter(
                ((rand() % 1000) / 1000.0f - 0.5f) * 2.0f,  // Random -1 to 1
                ((rand() % 1000) / 1000.0f - 0.5f) * 2.0f,
                ((rand() % 1000) / 1000.0f - 0.5f) * 2.0f
            );
            randomJitter *= 1.5f;  // Reduced from 3.0f for more subtle chaos
            
            glm::vec3 behaviourSetup = separationSet + cohesionSet + alignmentSet + randomJitter;
            
            // Allow for more chaotic movement - higher force limit
            if (glm::length(behaviourSetup) > 1.5f)  // Increased from 0.5f for more dynamic movement
            {
                if (glm::length(behaviourSetup) > 0.0001f) {
                    behaviourSetup = glm::normalize(behaviourSetup);
                    behaviourSetup *= 1.5f;  // Increased force magnitude
                } else {
                    behaviourSetup = glm::vec3(0.0f);
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
void Flock::setObstacleCollisionEnabled(bool enabled)
{
    m_checkSphereSphere = enabled;
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
            
            // Define avoidance zone near the boundary
            GLfloat avoidanceZone = ext[i] * 0.9f;  // Start avoiding at 90% of the extent
            
            // If this is greater or equal to the BBox extent /2 then there is a collision
            if(Distance >= ext[i])
            {
                // Hard collision - reflect the velocity
                GLfloat x = 2 * (s->getVelocity().dot((m_bbox->getNormalArray()[i])));
                Vector d = m_bbox->getNormalArray()[i] * x;
                // Use a smaller multiplier for more reasonable reflection
                s->setVelocity(s->getNextPosition() - d * 0.8);
                s->isHit();
            }
            else if(Distance >= avoidanceZone)
            {
                // Soft avoidance - add a gentle force away from the boundary
                GLfloat avoidanceStrength = (Distance - avoidanceZone) / (ext[i] - avoidanceZone);
                Vector avoidanceForce = m_bbox->getNormalArray()[i] * (avoidanceStrength * 0.2f);
                s->addVelocity(avoidanceForce);
            }
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
        
        // Use member variables for avoidance/collision/force
        GLfloat avoidanceRadius = obstacleRadius * m_obstacleAvoidanceRadiusScale;
        GLfloat collisionRadius = obstacleRadius * m_obstacleCollisionRadiusScale;
        
        if (distance < avoidanceRadius && distance > 0.0001f) {
            // Calculate smooth repulsion force
            Vector repulsionDir = boidPos - obstaclePos;
            repulsionDir.normalize();
            
            // Force strength inversely proportional to distance (stronger when closer)
            GLfloat forceStrength = (avoidanceRadius - distance) / avoidanceRadius;
            forceStrength = forceStrength * forceStrength; // Quadratic falloff for smoother transition
            
            // Apply gentle repulsion force (add to existing velocity, don't replace it)
            Vector repulsionForce = repulsionDir * (forceStrength * m_obstacleRepulsionForce);
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
    // Debug outputs removed for cleaner console output
}
//----------------------------------------------------------------------------------------------------------------------

