// ObstacleRenderer.h - Modernized, documented, and cleaned up for maintainability
#pragma once

#include "Obstacle.h"
#include "TransformStack.h"
#include "Camera.h"
#include "Material.h"
#include "FlockTypes.h"

/**
 * @file ObstacleRenderer.h
 * @brief Handles all OpenGL logic for drawing the obstacle (sphere) in the simulation.
 *
 * Modernized for clarity and maintainability. Uses FlockTypes.h for type aliases and helpers.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
class ObstacleRenderer {
public:
    ObstacleRenderer();
    ~ObstacleRenderer();

    /**
     * @brief Render the obstacle using the provided transform and camera.
     * @param obstacle The obstacle to render.
     * @param transform The current transform stack.
     * @param cam The camera for view/projection matrices.
     */
    void render(const Obstacle* obstacle, TransformStack& transform, Camera* cam);
};
