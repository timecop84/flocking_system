#include "SphereGeometry.h"
#include "glew_compat.h"
#include <iostream>
#include <cmath>
#include <GL/gl.h>

namespace FlockingGeometry {

SphereGeometry::SphereGeometry(float radius, int sectors, int stacks)
    : m_radius(radius), m_sectors(sectors), m_stacks(stacks), 
      m_VAO(0), m_VBO(0), m_EBO(0), m_buffersInitialized(false)
{
    generateSphere();
}

SphereGeometry::~SphereGeometry()
{
    cleanup();
}

void SphereGeometry::generateSphere()
{
    m_vertices.clear();
    m_indices.clear();
    
    const float PI = flock::Math::PI;
    
    // Generate vertices
    for (int stack = 0; stack <= m_stacks; ++stack) {
        float stackAngle = PI / 2 - stack * PI / m_stacks;  // from π/2 to -π/2
        float xy = m_radius * cosf(stackAngle);              // r * cos(θ)
        float z = m_radius * sinf(stackAngle);               // r * sin(θ)
        
        for (int sector = 0; sector <= m_sectors; ++sector) {
            float sectorAngle = sector * 2 * PI / m_sectors; // from 0 to 2π
            
            Vertex vertex;
            
            // Position
            vertex.position.x = xy * cosf(sectorAngle);      // r * cos(θ) * cos(φ)
            vertex.position.y = xy * sinf(sectorAngle);      // r * cos(θ) * sin(φ)
            vertex.position.z = z;                           // r * sin(θ)
            
            // Normal (for sphere, normal = normalized position vector from center)
            // Since the sphere is centered at origin, normal is just the normalized position
            flock::Vec3 normalVec(vertex.position.x, vertex.position.y, vertex.position.z);
            vertex.normal = flock::Utils::normalize(normalVec);
            
            // Texture coordinates
            vertex.texCoord.x = (float)sector / m_sectors;
            vertex.texCoord.y = (float)stack / m_stacks;
            
            m_vertices.push_back(vertex);
        }
    }
    
    // Generate indices
    for (int stack = 0; stack < m_stacks; ++stack) {
        int k1 = stack * (m_sectors + 1);     // beginning of current stack
        int k2 = k1 + m_sectors + 1;          // beginning of next stack
        
        for (int sector = 0; sector < m_sectors; ++sector) {
            // Two triangles per sector
            if (stack != 0) {
                // First triangle (k1, k2, k1+1)
                m_indices.push_back(k1);
                m_indices.push_back(k2);
                m_indices.push_back(k1 + 1);
            }
            
            if (stack != (m_stacks - 1)) {
                // Second triangle (k1+1, k2, k2+1)
                m_indices.push_back(k1 + 1);
                m_indices.push_back(k2);
                m_indices.push_back(k2 + 1);
            }
            
            ++k1;
            ++k2;
        }
    }
    
    std::cout << "Generated sphere: " << m_vertices.size() << " vertices, " 
              << m_indices.size() << " indices" << std::endl;
}

void SphereGeometry::initializeBuffers()
{
    if (m_buffersInitialized) {
        cleanup(); // Clean up existing buffers
    }
    
    std::cout << "Initializing sphere buffers..." << std::endl;
    
    // Check if VAO functions are available
    if (!glGenVertexArrays || !glBindVertexArray || !glDeleteVertexArrays) {
        std::cerr << "ERROR: VAO functions not available!" << std::endl;
        return;
    }
    
    // Generate OpenGL objects
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    std::cout << "Generated buffers: VAO=" << m_VAO << ", VBO=" << m_VBO << ", EBO=" << m_EBO << std::endl;
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error after buffer generation: " << error << std::endl;
        return;
    }
    
    // Bind VAO first
    glBindVertexArray(m_VAO);
    
    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), 
                 m_vertices.data(), GL_STATIC_DRAW);
    
    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), 
                 m_indices.data(), GL_STATIC_DRAW);
    
    // Set up vertex attributes
    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    // Texture coordinate attribute (location = 1) 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);
    
    // Normal attribute (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    
    // Check for OpenGL errors
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error during vertex setup: " << error << std::endl;
        return;
    }
    
    // Unbind VAO (good practice)
    glBindVertexArray(0);
    
    m_buffersInitialized = true;
    
    std::cout << "Sphere geometry buffers initialized successfully!" << std::endl;
}

void SphereGeometry::render() const
{
    if (!m_buffersInitialized) {
        std::cerr << "Error: Sphere buffers not initialized!" << std::endl;
        return;
    }
    
    // For efficiency, don't log every frame in production
    static int renderCount = 0;
    if (renderCount++ % 60 == 0) {
        std::cout << "Rendering sphere using VAO " << m_VAO << std::endl;
    }
    
    // Bind our VAO which contains the sphere geometry
    glBindVertexArray(m_VAO);
    
    // Draw the sphere using indices
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind VAO when done
    glBindVertexArray(0);
}

void SphereGeometry::cleanup()
{
    if (m_buffersInitialized) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        
        m_VAO = 0;
        m_VBO = 0;
        m_EBO = 0;
        m_buffersInitialized = false;
        
        std::cout << "Sphere geometry buffers cleaned up" << std::endl;
    }
}

} // namespace FlockingGeometry
