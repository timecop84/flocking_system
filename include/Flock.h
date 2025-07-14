// ...existing code...
#pragma once
#include <vector>
#include <string>
#include "FlockTypes.h"
#include "Boid.h"
#include "modules/graphics/include/BBox.h"
#include "TransformStack.h"
#include "Camera.h"
#include "Obstacle.h"
#include "Behaviours.h"
#include "SpatialHashGrid.h"

/**
 * @file flock.h
 * @brief The Flock class handles flock simulation, drawing, and collision logic.
 *
 * Modernized for clarity, maintainability, and efficiency. All type aliases and math helpers are now in FlockTypes.h.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
class Flock {
public:
    /// @brief Construct a Flock with bounding box and obstacle
    Flock(BBox* bbox, Obstacle* obstacle);
    ~Flock();

    /// @brief Add new boids to the flock
    void addBoids();
    /// @brief Remove the last 10 boids
    void removeBoids();
    /// @brief Reset all boids
    void resetBoids();
    /// @brief Draw the flock
    void draw(const std::string& shaderName, TransformStack& transformStack, Camera* cam) const;
    /// @brief Validate bounding box collision
    void validateBoundingBoxCollision();
    /// @brief Check for collisions
    void checkCollisions();
    /// @brief Calculate the final velocity for the flock
    Vector finalFlockVelocity();
    /// @brief Update the flock (modern GLM-based logic)
    void update();
    /// @brief Get the current flock size
    int getFlockSize() const { return m_numberOfBoids; }
    void setFlockSize(int size);
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
    /// @brief set obstacle collision checking enabled/disabled
    void setObstacleCollisionEnabled(bool enabled);
    /// @brief Demonstrate modern flocking system
    void demonstrateModernFlocking();
    /// @brief Get the boid list for validation and testing
    const std::vector<Boid*>& getBoidList() const { return m_boidList; }
    /// @brief Get the behaviors instance for validation and testing  
    Behaviours* getBehaviours() const { return m_behaviours; }
    /// @brief Set the scale of the obstacle avoidance radius
    void setObstacleAvoidanceRadiusScale(float scale) { m_obstacleAvoidanceRadiusScale = scale; }
    /// @brief Set the scale of the obstacle collision radius
    void setObstacleCollisionRadiusScale(float scale) { m_obstacleCollisionRadiusScale = scale; }
    /// @brief Set the strength of the repulsion force from obstacles
    void setObstacleRepulsionForce(float force) { m_obstacleRepulsionForce = force; }
private:
    std::vector <Boid*> m_boidList;
    bool m_hit;
    int m_numberOfBoids;
    Flock *m_react;
    bool m_checkSphereSphere;
    int _boidId;
    Boid *_boid;
    BBox *m_bbox;
    void  checkSphereCollisions();
    Obstacle *m_obstacle;
    Behaviours *m_behaviours;
    double m_boidScale;
    Colour m_boidColour;
    float m_speedMultiplier;
    /// @brief spatial hash grid for efficient neighbor queries (O(N) instead of O(N²))
    flock::SpatialHashGrid m_spatialGrid;
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
    /// @brief scale of the obstacle avoidance radius
    float m_obstacleAvoidanceRadiusScale = 3.0f;
    /// @brief scale of the obstacle collision radius
    float m_obstacleCollisionRadiusScale = 1.3f;
    /// @brief strength of the repulsion force from obstacles
    float m_obstacleRepulsionForce = 0.45f;

};
