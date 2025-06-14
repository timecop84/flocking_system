#ifndef BOID_H
#define BOID_H

#include "ngl/Vector.h"
#include <ngl/ShaderLib.h>
#include <ngl/TransformStack.h>
#include <ngl/Camera.h>

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
    Boid(ngl::Vector position,ngl::Vector direction);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the current position of the boid
    /// @param [in] m_position passed and gets the value of the current position.
    inline ngl::Vector getPosition()const {return m_position;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief takes the current position of the boid
    /// @param [in] m_position sets the value of the current position. Used in Collision method.
    inline void setPosition(ngl::Vector Position) {m_position = Position;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the next position of the boid.
    /// @param [in] m_nextPosition returns the value for the nextPosition. Used for Direction.
    inline ngl::Vector getNextPosition()const {return m_nextPosition;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the velocity of the boid.
    /// @param [in] m_velocity sets the velocity of the boid. Used by Direction in Boid class.
    inline void setVelocity(ngl::Vector _d){m_velocity=_d;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to get the velocity value of the boid.
    /// @param [in] m_velocity gets and gets the value of the current velocity. Used for the BBox collision.
    inline ngl::Vector getVelocity () {return m_velocity;}
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
    void updateVelocity(ngl::Vector direction);
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
    void setScale(ngl::Vector scale) { m_scale = scale; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief updates the velocity constraints.
    void velocityConstraint();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief drawing the VBO sphere
    /// @param [in] _shaderName value
    /// @param [in] _transformStack  values
    /// @param [in] _cam camera values
    void draw(const std::string &_shaderName,ngl::TransformStack &_transformStack,ngl::Camera *_cam)const;
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
    /// @brief sets the color of the boids
    /// @param [in] m_colour sets the color of the boids.
    void setColour(ngl::Colour colour) {m_colour = colour;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the wireframe for the boids
    /// @param [in] m_wireframe sets the wireframe on/off.
    void setWireframe(bool value) {m_wireframe = value;}
    //----------------------------------------------------------------------------------------------------------------------


private:
    //----------------------------------------------------------------------------------------------------------------------
    /*! flag to indicate if the sphere has been hit by ray */
    bool m_hit;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief member to store the newDirection of the boid
    ngl::Vector m_newDirection;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief member to store the current direction of the boids
    ngl::Vector m_direction;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the current position of the boid
    ngl::Vector m_position;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the last position of the boid in the world
    ngl::Vector m_lastPosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the next position of the boid within the world
    ngl::Vector m_nextPosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the initial velocity of the boid
    ngl::Vector m_velocity;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the scale of the boid
    ngl::Vector m_scale;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store a maximum allowed velocity (used as a velocity constraint)
    GLfloat m_maxVelocity;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the minimum allowed velocity (used as a velocity constraint)
    GLfloat m_minVelocity;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the colour
    ngl::Colour m_colour;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a member to store the size of the boid
    GLfloat m_size;
    //----------------------------------------------------------------------------------------------------------------------
    bool m_wireframe;
    //----------------------------------------------------------------------------------------------------------------------

protected:
    void loadMatricesToShader(
            ngl::TransformStack &_tx,
            ngl::Camera *_cam
            )const;



};

#endif // BOID_H
