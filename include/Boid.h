// ...existing code...
#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <glad/gl.h>
#include "FlockTypes.h"
#include "Vector.h"
#include "Colour.h"
#include "Camera.h"
#include "TransformStack.h"
#include "SphereGeometry.h"

/**
 * @file boid.h
 * @brief The Boid class represents an individual agent in the flock.
 *
 * Modernized for clarity, maintainability, and efficiency. All type aliases and math helpers are now in FlockTypes.h.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
class Boid {
public:
    Boid();
    ~Boid();
    Boid(Vector position, Vector direction);
    inline Vector getPosition() const { return m_position; }
    inline Vector getLastPosition() const { return m_lastPosition; }
    inline void setPosition(Vector position) { m_position = position; }
    inline Vector getNextPosition() const { return m_nextPosition; }
    inline void setVelocity(Vector v) { m_velocity = v; }
    inline Vector getVelocity() { return m_velocity; }
    inline void setMaxVelocity(GLfloat maxVelocity) { m_maxVelocity = maxVelocity; }
    inline void setMinVelocity(GLfloat minVelocity) { m_minVelocity = minVelocity; }
    void updateVelocity(Vector direction);
    inline void addVelocity(const Vector& force) { m_velocity += force; }
    void boidDirection();
    float getSize() const { return m_size; }
    void setScale(Vector scale) { m_scale = scale; }
    Vector getScale() const { return m_scale; }
    void velocityConstraint();
    void draw(const std::string &_shaderName, TransformStack &_transformStack, Camera *_cam) const;
    /// @brief drawing the boid using modern VBO/VAO rendering with UBO shaders
    /// @param [in] _shaderName value
    /// @param [in] _transformStack  values
    /// @param [in] _cam camera values
    void drawModern(const std::string &_shaderName, TransformStack &_transformStack, Camera *_cam) const;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief reverse function
    /// @param [in] m_velocity sets the velocity to have a new direction plus the next position. Called during boid to obstacle collision.
    inline void reverse() { m_velocity = (m_newDirection + getNextPosition()) * -1.2; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the hit function of the boid
    /// @param [in] m_hit called from boids to check if there is collision.
    inline void setHit() { m_hit = true; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the hit function of the boid
    /// @param [in] m_hit passed from boids for the value of collisions
    inline bool isHit() const { return m_hit; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the colour of the boid
    /// @param [in] colour the new colour for the boid
    inline void setColour(const Colour& colour) {
        m_colour = colour;
    }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the wireframe mode of the boid
    /// @param [in] value true for wireframe, false for solid
    inline void setWireframe(bool value) { m_wireframe = value; }

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
