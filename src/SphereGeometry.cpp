/**
 * @file SphereGeometry.cpp
 * @brief Implementation of the SphereGeometry class for rendering spheres in the simulation.
 *
 * Handles geometry generation, OpenGL buffer management, and rendering. Uses FlockTypes.h for clarity and maintainability.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */

#include <glad/gl.h>
#include "SphereGeometry.h"
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
    m_vertices.reserve((m_stacks + 1) * (m_sectors + 1));
    m_indices.reserve(m_stacks * m_sectors * 6);
    
    const float PI = flock::Math::PI;
    
    // Generate vertices
    for (int stack = 0; stack <= m_stacks; ++stack) {
        float stackAngle = PI / 2 - stack * PI / m_stacks;
        float xy = m_radius * cosf(stackAngle);
        float z = m_radius * sinf(stackAngle);
        
        for (int sector = 0; sector <= m_sectors; ++sector) {
            float sectorAngle = sector * 2 * PI / m_sectors;
            
            Vertex vertex;
            
            vertex.position.x = xy * cosf(sectorAngle);
            vertex.position.y = xy * sinf(sectorAngle);
            vertex.position.z = z;
            
            // Normal (for sphere, normal = normalized position vector from center)
            flock::Vec3 normalVec(vertex.position.x, vertex.position.y, vertex.position.z);
            vertex.normal = flock::Utils::normalize(normalVec);
            
            vertex.texCoord.x = (float)sector / m_sectors;
            vertex.texCoord.y = (float)stack / m_stacks;
            
            m_vertices.push_back(vertex);
        }
    }
    
    for (int stack = 0; stack < m_stacks; ++stack) {
        int k1 = stack * (m_sectors + 1);
        int k2 = k1 + m_sectors + 1;
        
        for (int sector = 0; sector < m_sectors; ++sector) {
            if (stack != 0) {
                m_indices.push_back(k1);
                m_indices.push_back(k2);
                m_indices.push_back(k1 + 1);
            }
            
            if (stack != (m_stacks - 1)) {
                m_indices.push_back(k1 + 1);
                m_indices.push_back(k2);
                m_indices.push_back(k2 + 1);
            }
            
            ++k1;
            ++k2;
        }
    }
}

void SphereGeometry::initializeBuffers()
{
    if (m_buffersInitialized) {
        cleanup();
    }
    
    if (!glGenVertexArrays || !glBindVertexArray || !glDeleteVertexArrays) {
        std::cerr << "ERROR: VAO functions not available!" << std::endl;
        return;
    }
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error after buffer generation: " << error << std::endl;
        return;
    }
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), 
                 m_vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), 
                 m_indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error during vertex setup: " << error << std::endl;
        return;
    }
    
    glBindVertexArray(0);
    
    m_buffersInitialized = true;
}

void SphereGeometry::render() const
{
    if (!m_buffersInitialized) {
        std::cerr << "Error: Sphere buffers not initialized!" << std::endl;
        return;
    }
    
    
    glBindVertexArray(m_VAO);
    
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    
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
    }
}

} // namespace FlockingGeometry
