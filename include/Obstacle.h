// ...existing code...
#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <GL/gl.h>
#include "FlockTypes.h"
#include "Camera.h"
#include "Vector.h"
#include "Colour.h"
#include "TransformStack.h"
#include "SphereGeometry.h"

/**
 * @file obstacle.h
 * @brief The Obstacle class represents a spherical obstacle in the flocking simulation.
 *
 * Modernized for clarity, maintainability, and efficiency. All type aliases and math helpers are now in FlockTypes.h.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */
class Obstacle {
public:
    /**
     * @brief Construct a new Obstacle object
     * 
     * @param spherePosition Initial position of the obstacle
     * @param sphereRadius Initial radius of the obstacle
     */
    Obstacle(Vector spherePosition, float sphereRadius);
    
    /**
     * @brief Destroy the Obstacle object
     */
    ~Obstacle();
    
    /**
     * @brief Draw the obstacle using the specified shader and transform stack
     * 
     * @param shaderName Name of the shader to use for drawing
     * @param transformStack Transform stack to apply to the obstacle
     * @param cam Camera object for view and projection matrices
     */
    void ObsDraw(const std::string& shaderName, TransformStack& transformStack, Camera* cam) const;
    
    /**
     * @brief Get the position of the obstacle
     * 
     * @return Vector The position of the obstacle
     */
    Vector getSpherePosition() const { return _spherePosition; }
    
    /**
     * @brief Set the position of the obstacle
     * 
     * @param position New position for the obstacle
     */
    void setSpherePosition(Vector position) { _spherePosition = position; }
    
    /**
     * @brief Get the radius of the obstacle
     * 
     * @return float The radius of the obstacle
     */
    float getSphereRadius() const { return _sphereRadius; }
    
    /**
     * @brief Set the radius of the obstacle
     * 
     * @param radius New radius for the obstacle
     */
    void setSphereRadius(float radius) { 
        _sphereRadius = radius; 
        m_sphereGeometry = nullptr; // Force geometry to be recreated with new size
    }
    
    /**
     * @brief Set the color of the obstacle
     * 
     * @param colour New color for the obstacle
     */
    void setColour(Colour colour) { m_colour = colour; }
    
    /**
     * @brief Enable or disable wireframe mode for the obstacle
     * 
     * @param value True to enable wireframe mode, false to disable
     */
    void setWireframe(bool value) { m_wireframe = value; }
    
    //----------------------------------------------------------------------------------------------------------------------
    // Modern GLM-based accessors for gradual migration
    //----------------------------------------------------------------------------------------------------------------------
    
    /// @brief Get obstacle position as modern GLM vector
    flock::Vec3 getPositionModern() const { 
        return flock::Vec3(_spherePosition.m_x, _spherePosition.m_y, _spherePosition.m_z); 
    }
    
    /// @brief Set obstacle position from modern GLM vector
    void setPositionModern(const flock::Vec3& pos) { 
        std::cout << "[Obstacle] setPositionModern: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
        _spherePosition.set(pos.x, pos.y, pos.z); // sync legacy
    }
    
    /// @brief Get obstacle color as modern GLM vector
    flock::Color getColorModern() const { 
        return flock::Color(m_colour.m_r, m_colour.m_g, m_colour.m_b, m_colour.m_a); 
    }
    
    /// @brief Set obstacle color from modern GLM vector
    void setColorModern(const flock::Color& color) { 
        std::cout << "[Obstacle] setColorModern: " << color.r << ", " << color.g << ", " << color.b << ", " << color.a << std::endl;
        m_colour.set(color.r, color.g, color.b, color.a); // sync legacy
    }
    
    /// @brief Get obstacle radius (same for modern, no conversion needed)
    float getRadiusModern() const {
        return _sphereRadius;
    }
    
    /// @brief Set obstacle radius (same for modern, no conversion needed)
    void setRadiusModern(float radius) {
        std::cout << "[Obstacle] setRadiusModern: " << radius << std::endl;
        _sphereRadius = radius;
        m_sphereGeometry = nullptr; // Force geometry to be recreated with new size
    }
    
    //----------------------------------------------------------------------------------------------------------------------

private:
    /// @brief a variable to store the obstacle position
    Vector _spherePosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a variable to store the obstacle radius
    float _sphereRadius;
    //----------------------------------------------------------------------------------------------------------------------
    bool _hit;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the wireframe value
    bool m_wireframe;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the color value
    Colour m_colour;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sphere geometry for modern VBO/VAO rendering
    mutable std::unique_ptr<FlockingGeometry::SphereGeometry> m_sphereGeometry;
    //----------------------------------------------------------------------------------------------------------------------

};
