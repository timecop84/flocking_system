#ifndef BOIDRENDERER_H
#define BOIDRENDERER_H

#include <vector>
#include "boid.h"
#include "TransformStack.h"
#include "Camera.h"
#include "modules/graphics/include/InstancedBoidRenderer.h"

/**
 * @class BoidRenderer
 * @brief Handles all OpenGL logic for drawing boids, including instanced rendering.
 */
class BoidRenderer {
public:
    BoidRenderer();
    ~BoidRenderer();

    /**
     * @brief Initialize shaders and GPU resources for boid rendering.
     */
    void initialize();

    /**
     * @brief Render all boids using instanced or standard rendering.
     * @param boids The list of boid pointers to render.
     * @param transform The current transform stack.
     * @param cam The camera for view/projection matrices.
     */
    void render(const std::vector<Boid*>& boids, const TransformStack& transform, Camera* cam);

    /**
     * @brief Set the InstancedBoidRenderer to use for instanced rendering.
     * @param renderer Pointer to the InstancedBoidRenderer instance.
     */
    void setInstancedBoidRenderer(FlockingGraphics::InstancedBoidRenderer* renderer);

    /**
     * @brief Get the number of threads used in the rendering process.
     * @return The number of threads.
     */
    int getNumThreads() const;

private:
    FlockingGraphics::InstancedBoidRenderer* m_instancedRenderer = nullptr;
};

#endif // BOIDRENDERER_H
