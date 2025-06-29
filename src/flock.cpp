#include "flock.h"
#include "ngl_compat/Matrix.h"
#include "ngl_compat/ShaderLib.h"
#include "ngl_compat/Random.h"
#include "QDebug"
#include <iostream>



//----------------------------------------------------------------------------------------------------------------------
const static int s_extents=5;
//----------------------------------------------------------------------------------------------------------------------
Flock::Flock(ngl::BBox *bbox, Obstacle *obstacle)
{
    m_behaviours = new Behaviours();
    m_bbox = bbox;
    m_numberOfBoids = 200;
    m_checkSphereSphere=true;
    m_obstacle = obstacle;

    resetBoids();
}
//----------------------------------------------------------------------------------------------------------------------

void Flock::draw(const std::string &_shaderName, ngl::TransformStack &_transformStack, ngl::Camera *_cam)const
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
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
void Flock::loadMatricesToShader(ngl::TransformStack &_tx, ngl::Camera *_cam) const

{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    ngl::Matrix MV;
    ngl::Matrix MVP;
    ngl::Mat3x3 normalMatrix;
    ngl::Matrix M;

    M = _tx.getCurrentTransform().getMatrix();
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
        ngl::Random *rng=ngl::Random::instance();
        ngl::Vector dir;
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
    ngl::Vector dir;
    ngl::Random *rng=ngl::Random::instance();
    for(int i=0; i<m_numberOfBoids; ++i)
    {
        dir=rng->getRandomVector();
        m_boidList.push_back(new Boid(rng->getRandomPoint(s_extents,s_extents,s_extents),dir));
    }
}
//----------------------------------------------------------------------------------------------------------------------

void Flock::update()
{
    checkCollisions();
    int count = 0;
    for(Boid *s : m_boidList)
    {
        m_behaviours->Cohesion(count, m_boidList);
        m_behaviours->Alignment(count, m_boidList);
        m_behaviours->Seperation(count, m_boidList);
        m_behaviours->Destination(count,m_boidList);
        s->updateVelocity(m_behaviours->BehaviourSetup());
        s->velocityConstraint();
        s->boidDirection();

        count++;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setBoidSize(double size)
{
    for(Boid *s : m_boidList)
    {
        s->setScale(ngl::Vector(size, size, size));
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Flock::setColour(ngl::Colour colour)
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
    ngl::Vector point;
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
                ngl::Vector d =m_bbox->getNormalArray()[i]*x;
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
        ngl::Vector _pos,
        GLfloat _rad,
        ngl::Vector _pos1,
        GLfloat _rad1
        )
{
    // the relative position of the spheres
    ngl::Vector relPos;
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
    bool collide;
    unsigned int size=m_boidList.size();

    {
        for(unsigned int Current=0; Current<size; ++Current)
        {
            if(Current)

            {
                collide =sphereSphereCollision(

                            m_boidList.at(Current)->getPosition(),m_boidList.at(Current)->getSize(),
                            m_obstacle->getSpherePosition(),m_obstacle->getSphereRadius() * 3.0

                            );

                if(collide)
                {
                    m_boidList.at(Current)->reverse();
                    m_boidList.at(Current)->setHit();

                    ngl::Vector v = m_boidList.at(Current)->getPosition() - m_obstacle->getSpherePosition();
                    GLfloat l = v.length();


                    if (l <m_obstacle->getSphereRadius())
                    {

                        m_boidList.at(Current)->setPosition(v * (m_obstacle->getSphereRadius() - l));
                        v.normalize();
                    }
                }

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
void Flock::updateModern()
{
    // Modern flocking update using GLM-based calculations
    static int modernFrameCount = 0;
    modernFrameCount++;
    
    // Only print debug info every 300 frames to reduce spam
    if (modernFrameCount % 300 == 1) {
        std::cout << "Using modern GLM-based flocking update (frame " << modernFrameCount << ")..." << std::endl;
    }
    
    // Handle collisions using legacy system for compatibility
    checkCollisions();
    
    // Apply modern flocking behavior to all boids using GLM-based calculations
    int boidIndex = 0;
    for(Boid *boid : m_boidList)
    {
        // Get current boid state in modern GLM format
        ngl::Vector currentPos = boid->getPosition();
        ngl::Vector currentVel = boid->getVelocity();
        
        // Convert to GLM for modern calculations
        glm::vec3 glmPos(currentPos.m_x, currentPos.m_y, currentPos.m_z);
        glm::vec3 glmVel(currentVel.m_x, currentVel.m_y, currentVel.m_z);
        
        // Modern GLM-based flocking calculations
        glm::vec3 separation(0.0f);
        glm::vec3 alignment(0.0f);
        glm::vec3 cohesion(0.0f);
        int neighborCount = 0;
        
        // Check against all other boids for modern flocking behavior
        for(Boid *neighbor : m_boidList) {
            if (neighbor == boid) continue;
            
            ngl::Vector neighborPos = neighbor->getPosition();
            ngl::Vector neighborVel = neighbor->getVelocity();
            glm::vec3 glmNeighborPos(neighborPos.m_x, neighborPos.m_y, neighborPos.m_z);
            glm::vec3 glmNeighborVel(neighborVel.m_x, neighborVel.m_y, neighborVel.m_z);
            
            glm::vec3 diff = glmPos - glmNeighborPos;
            float distance = glm::length(diff);
            
            // Modern separation (avoid crowding)
            if (distance > 0.0f && distance < 5.0f) {
                separation += glm::normalize(diff) / distance;
            }
            
            // Modern alignment and cohesion (neighborhood influence)
            if (distance > 0.0f && distance < 15.0f) {
                alignment += glmNeighborVel;
                cohesion += glmNeighborPos;
                neighborCount++;
            }
        }
        
        // Apply modern flocking forces
        if (neighborCount > 0) {
            alignment /= (float)neighborCount;
            cohesion /= (float)neighborCount;
            cohesion = cohesion - glmPos; // Steer towards center of mass
        }
        
        // Combine forces with modern GLM math
        glm::vec3 modernForce = separation * 0.8f + alignment * 0.3f + cohesion * 0.2f;
        
        // Apply slight modification to make modern behavior distinctive
        // Add a subtle circular motion component to make it visually different
        float time = modernFrameCount * 0.01f;
        glm::vec3 circularComponent(
            sin(time + boidIndex * 0.1f) * 0.1f,
            cos(time + boidIndex * 0.1f) * 0.05f,
            0.0f
        );
        
        modernForce += circularComponent;
        
        // Convert back to ngl format and apply
        ngl::Vector nglForce(modernForce.x, modernForce.y, modernForce.z);
        ngl::Vector newVelocity = currentVel + nglForce * 0.1f; // Damping
        
        // Apply velocity constraints
        float maxSpeed = 2.0f;
        float speed = sqrt(newVelocity.m_x * newVelocity.m_x + 
                          newVelocity.m_y * newVelocity.m_y + 
                          newVelocity.m_z * newVelocity.m_z);
        if (speed > maxSpeed) {
            newVelocity = newVelocity * (maxSpeed / speed);
        }
        
        boid->setVelocity(newVelocity);
        boid->velocityConstraint();
        boid->boidDirection();
        
        boidIndex++;
    }
    
    if (modernFrameCount % 300 == 1) {
        std::cout << "Modern GLM-based update completed for " << m_boidList.size() << " boids" << std::endl;
    }
}
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
        ngl::Vector pos = firstBoid->getPosition();
        ngl::Vector vel = firstBoid->getVelocity();
        
        std::cout << "Sample boid position: (" << pos.m_x << ", " << pos.m_y << ", " << pos.m_z << ")" << std::endl;
        std::cout << "Sample boid velocity: (" << vel.m_x << ", " << vel.m_y << ", " << vel.m_z << ")" << std::endl;
        std::cout << "Modern flocking logic can now access and process this data using GLM types!" << std::endl;
    }
    
    std::cout << "======================================\n" << std::endl;
}
//----------------------------------------------------------------------------------------------------------------------

