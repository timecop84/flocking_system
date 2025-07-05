#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "../../../include/glew_compat.h"

namespace FlockingGeometry {
    class SphereGeometry;
}

namespace FlockingGraphics {

struct BoidInstanceData {
    glm::mat4 modelMatrix;
    glm::vec4 color;
};

//----------------------------------------------------------------------------------------------------------------------
/// @brief High-performance instanced renderer for boids using a single sphere geometry and instanced rendering
/// @details Renders thousands of boids efficiently using OpenGL instancing to achieve 60+ FPS with 2000+ boids
//----------------------------------------------------------------------------------------------------------------------
class InstancedBoidRenderer {
public:
    InstancedBoidRenderer();
    ~InstancedBoidRenderer();
    
    // Initialize the renderer with sphere geometry
    void initialize(float boidRadius = 0.5f, int segments = 16);
    
    // Clear all instance data for the current frame
    void clearInstances();
    
    // Add a boid instance to be rendered
    void addInstance(const glm::mat4& modelMatrix, const glm::vec4& color = glm::vec4(1.0f));
    
    // Render all instances in a single draw call
    void renderInstances(const std::string& shaderName);
    
    // Get performance stats
    size_t getInstanceCount() const { return m_instanceData.size(); }
    
    // Cleanup resources
    void cleanup();

private:
    // Sphere geometry (shared by all instances)
    std::unique_ptr<FlockingGeometry::SphereGeometry> m_sphereGeometry;
    
    // Instance data
    std::vector<BoidInstanceData> m_instanceData;
    
    // OpenGL objects for instanced rendering
    GLuint m_instanceVBO;
    bool m_initialized;
    
    // Update the instance buffer with current data
    void updateInstanceBuffer();
    
    // Setup instanced vertex attributes
    void setupInstancedAttributes();
};

} // namespace FlockingGraphics
