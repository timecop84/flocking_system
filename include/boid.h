#ifndef BOID_H
#define BOID_H

#include <string>
#include <GL/gl.h>
#include "Vector.h"
#include "Colour.h"
#include "Camera.h"
#include "TransformStack.h"
// Modern includes for gradual migration
#include "FlockTypes.h"
#include "SphereGeometry.h"
#include <memory>

/*! \brief the boids class */
/// @file boids.h
/// @brief the boid class. Creates the initial boid.
/// @author Dionysios Toufexis
/// @version 1.0
/// @date 18/6/2012
/// Revision History :8/7/2012
/// @class Boid
/// @brief stores information about the boid and its linked to the flock class.

class Boid
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our ctor
    Boid();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our dtor
    ~Boid();
    //----------------------------------------------------------------------------------------------------------------------
    /// @param [in] position the position of the boid
    /// @param [in] direction the direction of the boid
    //----------------------------------------------------------------------------------------------------------------------
    Boid(Vector position,Vector direction);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the current position of the boid
    /// @param [in] m_position passed and gets the value of the current position.
    inline Vector getPosition()const {return m_position;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief takes the current position of the boid
    /// @param [in] m_position sets the value of the current position. Used in Collision method.
    inline void setPosition(Vector Position) {m_position = Position;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the next position of the boid.
    /// @param [in] m_nextPosition returns the value for the nextPosition. Used for Direction.
    inline Vector getNextPosition()const {return m_nextPosition;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the velocity of the boid.
    /// @param [in] m_velocity sets the velocity of the boid. Used by Direction in Boid class.
    inline void setVelocity(Vector _d){m_velocity=_d;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to get the velocity value of the boid.
    /// @param [in] m_velocity gets and gets the value of the current velocity. Used for the BBox collision.
    inline Vector getVelocity () {return m_velocity;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the maximum velocity.
    /// @param [in] m_velocity maxValue sets the maximum velocity of the boid
    inline void setMaxVelocity(GLfloat maxVelocity){m_maxVelocity = maxVelocity;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the minimum velocity.
    /// @param [in] m_velocity minValue sets the minimum velocity of the boid
    inline void setMinVelocity(GLfloat minVelocity){m_minVelocity = minVelocity;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief updates the velocity.
    /// @param [in] direction gets the value of direction of the boid
    void updateVelocity(Vector direction);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds to the current velocity.
    /// @param [in] force force to add to the current velocity
    inline void addVelocity(const Vector& force) { m_velocity += force; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief updates the boid direction.
    void boidDirection();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief gets the velocity.
    /// @param [in] m_size gets the size of the boid. Used in BBox and Sphere Collision.
    float getSize()const{ return  m_size; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable for boid Scale
    /// @param [in] m_scale sets the scale of the boid.
    void setScale(Vector scale) { m_scale = scale; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief updates the velocity constraints.
    void velocityConstraint();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief drawing the VBO sphere
    /// @param [in] _shaderName value
    /// @param [in] _transformStack  values
    /// @param [in] _cam camera values
    void draw(const std::string &_shaderName,TransformStack &_transformStack,Camera *_cam)const;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief drawing the boid using modern VBO/VAO rendering with UBO shaders
    /// @param [in] _shaderName value
    /// @param [in] _transformStack  values
    /// @param [in] _cam camera values
    void drawModern(const std::string &_shaderName,TransformStack &_transformStack,Camera *_cam)const;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief reverse function
    /// @param [in] m_velocity sets the velocity to have a new direction plus the next position. Called during boid to obstacle collision.
    inline void reverse(){m_velocity= (m_newDirection + getNextPosition()) * - 20.0;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the hit function of the boid
    /// @param [in] m_hit called from boids to check if there is collision.
    inline void setHit(){m_hit=true;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the hit function of the boid
    /// @param [in] m_hit passed from boids for the value of collisions
    inline bool isHit()const {return m_hit;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the colour of the boid
    /// @param [in] colour the new colour for the boid
    inline void setColour(const Colour& colour) {m_colour = colour;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the wireframe mode of the boid
    /// @param [in] value true for wireframe, false for solid
    inline void setWireframe(bool value) {m_wireframe = value;}

    //----------------------------------------------------------------------------------------------------------------------
    // Modern GLM-based accessors for gradual migration
    //----------------------------------------------------------------------------------------------------------------------
    
    /// @brief Get position as modern GLM vector
    flock::Vec3 getPositionModern() const { 
        return flock::Vec3(m_position.m_x, m_position.m_y, m_position.m_z); 
    }
    
    /// @brief Set position from modern GLM vector
    void setPositionModern(const flock::Vec3& pos) { 
        m_position.set(pos.x, pos.y, pos.z); 
    }
    
    /// @brief Get velocity as modern GLM vector
    flock::Vec3 getVelocityModern() const { 
        return flock::Vec3(m_velocity.m_x, m_velocity.m_y, m_velocity.m_z); 
    }
    
    /// @brief Set velocity from modern GLM vector
    void setVelocityModern(const flock::Vec3& vel) { 
        m_velocity.set(vel.x, vel.y, vel.z); 
    }
    
    /// @brief Get direction as modern GLM vector
    flock::Vec3 getDirectionModern() const { 
        return flock::Vec3(m_direction.m_x, m_direction.m_y, m_direction.m_z); 
    }
    
    /// @brief Set direction from modern GLM vector
    void setDirectionModern(const flock::Vec3& dir) { 
        m_direction.set(dir.x, dir.y, dir.z); 
    }
    
    /// @brief Get color as modern GLM vector
    flock::Color getColorModern() const { 
        return flock::Color(m_colour.m_r, m_colour.m_g, m_colour.m_b, m_colour.m_a); 
    }
    
    /// @brief Set color from modern GLM vector
    void setColorModern(const flock::Color& color) { 
        m_colour.set(color.r, color.g, color.b, color.a); 
    }
    
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Modern flocking calculation using GLM (demonstration)
    /// @param [in] neighborPositions positions of neighboring boids
    /// @param [in] neighborVelocities velocities of neighboring boids
    /// @return combined flocking force as GLM vector
    flock::Vec3 calculateModernFlocking(const std::vector<flock::Vec3>& neighborPositions,
                                       const std::vector<flock::Vec3>& neighborVelocities) const;

    //----------------------------------------------------------------------------------------------------------------------


private:
    //----------------------------------------------------------------------------------------------------------------------
    /*! flag to indicate if the sphere has been hit by ray */
    bool m_hit;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief member to store the newDirection of the boid
    Vector m_newDirection;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief member to store the current direction of the boids
    Vector m_direction;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the current position of the boid
    Vector m_position;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the last position of the boid in the world
    Vector m_lastPosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the next position of the boid within the world
    Vector m_nextPosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the initial velocity of the boid
    Vector m_velocity;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the scale of the boid
    Vector m_scale;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store a maximum allowed velocity (used as a velocity constraint)
    GLfloat m_maxVelocity;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the minimum allowed velocity (used as a velocity constraint)
    GLfloat m_minVelocity;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the colour
    Colour m_colour;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the size of the boid
    GLfloat m_size;
    //----------------------------------------------------------------------------------------------------------------------
    bool m_wireframe;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sphere geometry for modern VBO/VAO rendering
    mutable std::unique_ptr<FlockingGeometry::SphereGeometry> m_sphereGeometry;
    //----------------------------------------------------------------------------------------------------------------------

protected:
    // Legacy matrix loading function removed - UBO-based rendering handles matrix updates automatically

};

#endif // BOID_H
