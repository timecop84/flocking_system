#ifndef OBSTACLERENDERER_H
#define OBSTACLERENDERER_H

#include "obstacle.h"
#include "TransformStack.h"
#include "Camera.h"
#include <Material.h>

/**
 * @class ObstacleRenderer
 * @brief Handles all OpenGL logic for drawing the obstacle (sphere) in the simulation.
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

#endif // OBSTACLERENDERER_H
