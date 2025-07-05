#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Camera.h"
#include "Vector.h"
#include "Colour.h"
#include "TransformStack.h"
// Modern includes for gradual migration
#include "FlockTypes.h"
#include "SphereGeometry.h"
#include <string>
#include <memory>
#include <GL/gl.h>

/*! \brief The obstacle class */
/// @file obstacle.h
/// @brief the obstacle class. Creates the obstacle
/// @author Dionysios Toufexis
/// @version 1.0
/// @date 01/7/2012
/// @class Obstacle
/// @brief create the obstacle and its properties to be used by the flock collision.


class Obstacle

{
public:
    /// @ ctor
    /// @param [in] spherePosition the initilized value of the obstacle position
    /// @param [in] sphereRadius the initilized value of the obstacle radius
    Obstacle(Vector spherePosition,
             float sphereRadius
             );
    
    /// @ destructor
    ~Obstacle();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief drawing the modern VBO-based obstacle using UBO shaders
    /// @param [in] _shaderName value
    /// @param [in] _transformStack  values
    /// @param [in] _cam camera values
    void ObsDraw(const std::string &_shaderName,
                 TransformStack &_transformStack,
                 Camera *_cam
                 )const ;
    
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Legacy matrix loading function removed - UBO-based rendering handles matrix updates automatically
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the obstacle position
    /// @param [in] _spherePosition the obstacle Position within the space.
    inline Vector getSpherePosition()const{return _spherePosition;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the obstacle position
    /// @param [in] _spherePosition returns the obstacle position.
    void setSpherePosition(Vector position) {_spherePosition = position;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief gets the obstacle size
    /// @param [in] _spherePosition returns the obstacle size.
    inline float getSphereRadius()const{return _sphereRadius;}
    //----------------------------------------------------------------------------------------------------------------------   
    /// @brief sets the obstacle radius
    /// @param [in] _spherePosition sets the obstacle size value.
    void setSphereRadius(float radius) {_sphereRadius = radius;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the color of the obstacle
    /// @param [in] m_color sets color value for the obstacle
    void setColour(Colour colour) {m_colour = colour;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the wireframe for the obstacle
    /// @param [in] m_wireframe sets the wireframe value on/off.
    void setWireframe(bool value) {m_wireframe = value;}
    
    //----------------------------------------------------------------------------------------------------------------------
    // Modern GLM-based accessors for gradual migration
    //----------------------------------------------------------------------------------------------------------------------
    
    /// @brief Get obstacle position as modern GLM vector
    flock::Vec3 getPositionModern() const { 
        return flock::Vec3(_spherePosition.m_x, _spherePosition.m_y, _spherePosition.m_z); 
    }
    
    /// @brief Set obstacle position from modern GLM vector
    void setPositionModern(const flock::Vec3& pos) { 
        _spherePosition.set(pos.x, pos.y, pos.z); 
    }
    
    /// @brief Get obstacle color as modern GLM vector
    flock::Color getColorModern() const { 
        return flock::Color(m_colour.m_r, m_colour.m_g, m_colour.m_b, m_colour.m_a); 
    }
    
    /// @brief Set obstacle color from modern GLM vector
    void setColorModern(const flock::Color& color) { 
        m_colour.set(color.r, color.g, color.b, color.a); 
    }
    
    /// @brief Get obstacle radius (same for modern, no conversion needed)
    float getRadiusModern() const {
        return _sphereRadius;
    }
    
    /// @brief Set obstacle radius (same for modern, no conversion needed)
    void setRadiusModern(float radius) {
        _sphereRadius = radius;
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


#endif // COLLISION_H
