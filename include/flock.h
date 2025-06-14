#ifndef FLOCK_H
#define FLOCK_H
#include "boid.h"
#include "ngl/Vector.h"
#include "ngl/TransformStack.h"
#include "ngl/ShaderLib.h"
#include "ngl/BBox.h"
#include "avoidance.h"
#include "obstacle.h"
#include "Behaviours.h"

/*! \brief The Flock class */
/// @file Flock.h
/// @brief handles the drawing of the the flock, the update movement and the collision.
/// @brief creation of boid method modified from Jon Macey's example. Collision Demo.
/// @brief BBox and spheretoSphere collision methods taken form Jon Macey's NGL Demos, Collision
/// @author Dionysios Toufexis
/// @version 1.0
/// @date 20/6/2012
/// @class Flock


class Flock
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor
    Flock(ngl::BBox *bbox, Obstacle *obstacle);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor
    ~Flock();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief add new boids
    void addBoids();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief removes the last 10 boids
    void removeBoids();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief creates the boids.
    void resetBoids();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our flock draw function
    void draw(const std::string &_shaderName, ngl::TransformStack &_transformStack, ngl::Camera *_cam) const;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our function to do the bounding box collision between the boids and the box.
    void validateBoundingBoxCollision();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a function to caclulate if the collision is true.
    void checkCollisions();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a function to calculate the final velocity for the flock.
    ngl::Vector finalFlockVelocity();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the update method to do all the updates. Then the update is called in the GLWindow in the time event.
    void update();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief GUI related functions.
    int getFlockSize() {return m_numberOfBoids;}
    void setFlockSize(int size) {m_numberOfBoids = size;}
    void setBoidSize(double size);
    void setColour(ngl::Colour colour);
    void setWireframe(bool value);
    void setSimDistance(double distance);
    void setSimFlockDistance(double distance);
    void setSimCohesion(double cohesion);
    void setSimSeparation(double separation);
    void setSimAlignment(double alignment);
    //----------------------------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a dynamic array to contain the boids.
    std::vector <Boid*> m_boidList;
    //----------------------------------------------------------------------------------------------------------------------
    /*! flag to indicate if the sphere has been hit by ray */
    bool m_hit;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the number of boids  are created
    int m_numberOfBoids;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief pointer to object of Behaviour class
    Flock *m_react;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if we need to do spheresphere checks
    bool m_checkSphereSphere;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the boid count
    int _boidId;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief pointer to boid class.
    Boid *_boid;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to the bbox.
    ngl::BBox *m_bbox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief shader method
    void loadMatricesToShader(ngl::TransformStack &_tx, ngl::Camera *_cam) const;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our sphere collision method.
    void  checkSphereCollisions();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer for the obstacle class
    Obstacle *m_obstacle;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer for the behaviour class
    Behaviours *m_behaviours;
    //----------------------------------------------------------------------------------------------------------------------
    double m_boidScale;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the color of the boid.
    ngl::Colour m_boidColour;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief do the actual sphereSphere collisions
    /// @param[in] _pos1 the position of the first sphere
    ///	@param[in] _radius1 the radius of the first sphere
    /// @param[in] _pos2 the position of the second sphere
    ///	@param[in] _radius2 the radius of the second sphere
    bool sphereSphereCollision(
                                     ngl::Vector _pos1,
                                     GLfloat _radius1,
                                     ngl::Vector _pos2,
                                     GLfloat _radius2
                                 );


    protected:


};

#endif // FLOCK_H
