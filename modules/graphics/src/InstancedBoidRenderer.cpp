#include "../include/InstancedBoidRenderer.h"
#include "../include/ShaderLib.h"
#include "../../../include/SphereGeometry.h"
#include <iostream>

namespace FlockingGraphics {

InstancedBoidRenderer::InstancedBoidRenderer() 
    : m_instanceVBO(0), m_initialized(false) {
}

InstancedBoidRenderer::~InstancedBoidRenderer() {
    cleanup();
}

void InstancedBoidRenderer::initialize(float boidRadius, int segments) {
    if (m_initialized) {
        cleanup();
    }
    
    // Create a single sphere geometry that will be shared by all instances
    m_sphereGeometry = std::make_unique<FlockingGeometry::SphereGeometry>(boidRadius, segments, segments);
    m_sphereGeometry->initializeBuffers();
    
    // Generate instance VBO
    glGenBuffers(1, &m_instanceVBO);
    
    // Setup instanced attributes
    setupInstancedAttributes();
    
    m_initialized = true;
}

void InstancedBoidRenderer::clearInstances() {
    m_instanceData.clear();
}

void InstancedBoidRenderer::addInstance(const glm::mat4& modelMatrix, const glm::vec4& color) {
    BoidInstanceData instance;
    instance.modelMatrix = modelMatrix;
    instance.color = color;
    m_instanceData.push_back(instance);
}

void InstancedBoidRenderer::renderInstances(const std::string& shaderName) {
    if (!m_initialized || m_instanceData.empty() || !m_sphereGeometry) {
        return;
    }
    
    // Update instance buffer with current data
    updateInstanceBuffer();
    
    // Use the specified shader
    ShaderLib* shader = ShaderLib::instance();
    shader->use(shaderName);
    
    // Bind the sphere geometry VAO
    glBindVertexArray(m_sphereGeometry->getVAO());
    
    // Render all instances in a single draw call
    glDrawElementsInstanced(
        GL_TRIANGLES,
        m_sphereGeometry->getIndexCount(),
        GL_UNSIGNED_INT,
        0,
        static_cast<GLsizei>(m_instanceData.size())
    );
    
    // Unbind VAO
    glBindVertexArray(0);
}

void InstancedBoidRenderer::updateInstanceBuffer() {
    if (m_instanceData.empty()) return;
    
    size_t dataSize = m_instanceData.size() * sizeof(BoidInstanceData);
    
    // Bind and update the instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    
    // Use GL_STREAM_DRAW for frequently updated data
    glBufferData(GL_ARRAY_BUFFER, dataSize, m_instanceData.data(), GL_STREAM_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedBoidRenderer::setupInstancedAttributes() {
    if (!m_sphereGeometry) return;
    
    // Bind the sphere geometry VAO to setup instanced attributes
    glBindVertexArray(m_sphereGeometry->getVAO());
    
    // Bind instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    
    // Setup matrix attributes (4 vec4s for mat4)
    // Attribute locations 3, 4, 5, 6 for the 4 columns of the matrix
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, 
                             sizeof(BoidInstanceData), 
                             (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(3 + i, 1); // Advance once per instance
    }
    
    // Setup color attribute (location 7)
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 
                         sizeof(BoidInstanceData), 
                         (void*)(sizeof(glm::mat4)));
    glVertexAttribDivisor(7, 1); // Advance once per instance
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void InstancedBoidRenderer::cleanup() {
    if (m_instanceVBO != 0) {
        glDeleteBuffers(1, &m_instanceVBO);
        m_instanceVBO = 0;
    }
    
    m_sphereGeometry.reset();
    m_instanceData.clear();
    m_initialized = false;
}

} // namespace FlockingGraphics
