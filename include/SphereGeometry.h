#pragma once

#include <vector>
#include <GL/gl.h>
#include "FlockTypes.h"

namespace FlockingGeometry {

struct Vertex {
    flock::Vec3 position;
    flock::Vec3 normal;
    glm::vec2 texCoord;
};

class SphereGeometry {
public:
    SphereGeometry(float radius = 1.0f, int sectors = 32, int stacks = 32);
    ~SphereGeometry();
    
    // Initialize OpenGL buffers
    void initializeBuffers();
    
    // Render the sphere
    void render() const;
    
    // Clean up resources
    void cleanup();
    
    // Get sphere data (for debugging or custom usage)
    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_indices; }
    
private:
    void generateSphere();
    
    float m_radius;
    int m_sectors;  // longitude slices
    int m_stacks;   // latitude slices
    
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    
    // OpenGL objects
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    bool m_buffersInitialized;
};

} // namespace FlockingGeometry
