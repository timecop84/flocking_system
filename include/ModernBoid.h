#pragma once

#include <string>
#include <GL/gl.h>
#include "FlockTypes.h"

/**
 * Modern Boid class using GLM and standard C++
 * Represents a single boid in the flocking simulation
 */
class Boid {
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// Default constructor
    Boid();
    
    /// Destructor
    ~Boid() = default;
    
    /// Constructor with position and direction
    /// @param position Initial position of the boid
    /// @param direction Initial direction of the boid
    Boid(const flock::Vec3& position, const flock::Vec3& direction);
    
    //----------------------------------------------------------------------------------------------------------------------
    // Position and movement accessors
    //----------------------------------------------------------------------------------------------------------------------
    
    /// Get current position
    const flock::Vec3& getPosition() const { return m_position; }
    
    /// Set current position
    void setPosition(const flock::Vec3& position) { m_position = position; }
    
    /// Get next position
    const flock::Vec3& getNextPosition() const { return m_nextPosition; }
    
    /// Set velocity
    void setVelocity(const flock::Vec3& velocity) { m_velocity = velocity; }
    
    /// Get velocity
    const flock::Vec3& getVelocity() const { return m_velocity; }
    
    /// Get direction
    const flock::Vec3& getDirection() const { return m_direction; }
    
    /// Set direction
    void setDirection(const flock::Vec3& direction) { m_direction = direction; }
    
    //----------------------------------------------------------------------------------------------------------------------
    // Simulation methods
    //----------------------------------------------------------------------------------------------------------------------
    
    /// Update boid velocity based on direction
    /// @param direction New direction vector
    void updateVelocity(const flock::Vec3& direction);
    
    /// Update boid position and movement
    void update();
    
    //----------------------------------------------------------------------------------------------------------------------
    // Rendering
    //----------------------------------------------------------------------------------------------------------------------
    
    /// Draw the boid using modern OpenGL
    void draw() const;
    
    /// Set rendering scale
    void setScale(const flock::Vec3& scale) { m_scale = scale; }
    
    /// Get rendering scale
    const flock::Vec3& getScale() const { return m_scale; }
    
    /// Set wireframe mode
    void setWireframe(bool wireframe) { m_wireframe = wireframe; }
    
    /// Get wireframe mode
    bool isWireframe() const { return m_wireframe; }
    
    /// Set boid color
    void setColor(const flock::Color& color) { m_color = color; }
    
    /// Get boid color
    const flock::Color& getColor() const { return m_color; }
    
    //----------------------------------------------------------------------------------------------------------------------
    // Boundary checking
    //----------------------------------------------------------------------------------------------------------------------
    
    /// Check if boid needs direction change due to boundaries
    /// @param min Minimum boundary
    /// @param max Maximum boundary
    /// @return New direction vector if boundary hit, zero vector otherwise
    flock::Vec3 checkBoundaryDirection(const flock::Vec3& min, const flock::Vec3& max);

private:
    //----------------------------------------------------------------------------------------------------------------------
    // Movement data
    //----------------------------------------------------------------------------------------------------------------------
    flock::Vec3 m_position{0.0f};       ///< Current position
    flock::Vec3 m_lastPosition{0.0f};   ///< Previous position
    flock::Vec3 m_nextPosition{0.0f};   ///< Next position
    flock::Vec3 m_velocity{0.0f};       ///< Current velocity
    flock::Vec3 m_direction{0.0f};      ///< Current direction
    flock::Vec3 m_newDirection{0.0f};   ///< New direction for next update
    
    //----------------------------------------------------------------------------------------------------------------------
    // Rendering data
    //----------------------------------------------------------------------------------------------------------------------
    flock::Vec3 m_scale{1.0f};           ///< Rendering scale
    flock::Color m_color{flock::Colors::WHITE}; ///< Boid color
    bool m_wireframe = false;            ///< Wireframe rendering mode
    
    //----------------------------------------------------------------------------------------------------------------------
    // Physics constants
    //----------------------------------------------------------------------------------------------------------------------
    static constexpr float MAX_VELOCITY = 2.0f;     ///< Maximum velocity
    static constexpr float MIN_VELOCITY = 0.1f;     ///< Minimum velocity
    static constexpr float DAMPING = 0.98f;          ///< Velocity damping factor
    static constexpr float TIME_STEP = 0.016f;       ///< Time step for updates
};
