#ifndef FLOCK_H
#define FLOCK_H
#include <vector>
#include "boid.h"
#include "Vector.h"
#include "Colour.h"
#include "modules/graphics/include/BBox.h"
#include "TransformStack.h"
#include "Camera.h"
#include "obstacle.h"
#include "Behaviours.h"
#include "SpatialHashGrid.h" // Add spatial partitioning for performance

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
    Flock(BBox *bbox, Obstacle *obstacle);
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
    void draw(const std::string &_shaderName, TransformStack &_transformStack, Camera *_cam) const;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our function to do the bounding box collision between the boids and the box.
    void validateBoundingBoxCollision();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a function to caclulate if the collision is true.
    void checkCollisions();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a function to calculate the final velocity for the flock.
    Vector finalFlockVelocity();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the update method using modern GLM-based flocking logic
    void update();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief GUI related functions.
    int getFlockSize() {return m_numberOfBoids;}
    void setFlockSize(int size) {m_numberOfBoids = size;}
    void setBoidSize(double size);
    void setColour(Colour colour);
    Colour getColour() const { return m_boidColour; }
    void setWireframe(bool value);
    void setSpeedMultiplier(float multiplier);
    float getSpeedMultiplier() const { return m_speedMultiplier; }
    void setSimDistance(double distance);
    void setSimFlockDistance(double distance);
    void setSimCohesion(double cohesion);
    void setSimSeparation(double separation);
    void setSimAlignment(double alignment);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set obstacle collision checking enabled/disabled
    void setObstacleCollisionEnabled(bool enabled);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Demonstrate modern flocking system
    void demonstrateModernFlocking();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Get the boid list for validation and testing
    const std::vector<Boid*>& getBoidList() const { return m_boidList; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Get the behaviors instance for validation and testing  
    Behaviours* getBehaviours() const { return m_behaviours; }
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
    BBox *m_bbox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Legacy matrix loading function removed - UBO-based rendering handles matrix updates automatically
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
    Colour m_boidColour;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief speed multiplier for controlling flock movement speed
    float m_speedMultiplier;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief spatial hash grid for efficient neighbor queries (O(N) instead of O(N²))
    flock::SpatialHashGrid m_spatialGrid;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief do the actual sphereSphere collisions
    /// @param[in] _pos1 the position of the first sphere
    ///	@param[in] _radius1 the radius of the first sphere
    /// @param[in] _pos2 the position of the second sphere
    ///	@param[in] _radius2 the radius of the second sphere
    bool sphereSphereCollision(
                                     Vector _pos1,
                                     GLfloat _radius1,
                                     Vector _pos2,
                                     GLfloat _radius2
                                 );


    protected:


};

#endif // FLOCK_H
