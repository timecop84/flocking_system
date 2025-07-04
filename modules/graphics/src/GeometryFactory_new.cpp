#include "../include/GeometryFactory.h"
#include <iostream>
#include "../../../include/glew_compat.h"

namespace FlockingGraphics {

// Geometry struct implementation
Geometry::~Geometry() {
    cleanup();
}

void Geometry::bind() const {
    if (VAO != 0) {
        glBindVertexArray(VAO);
    }
}

void Geometry::render() const {
    if (VAO != 0) {
        glBindVertexArray(VAO);
        if (EBO != 0) {
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }
    }
}

void Geometry::cleanup() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
}

// GeometryFactory implementation
GeometryFactory& GeometryFactory::instance() {
    static GeometryFactory instance;
    return instance;
}

std::shared_ptr<Geometry> GeometryFactory::createGeometry(const std::string& name,
                                                         const std::vector<float>& vertices,
                                                         const std::vector<unsigned int>& indices) {
    // Check if geometry already exists
    auto it = m_geometries.find(name);
    if (it != m_geometries.end()) {
        return it->second;
    }
    
    // Create new geometry
    auto geometry = std::make_shared<Geometry>();
    createVAO(geometry.get(), vertices, indices);
    
    // Cache it
    m_geometries[name] = geometry;
    
    return geometry;
}

std::shared_ptr<Geometry> GeometryFactory::getGeometry(const std::string& name) {
    auto it = m_geometries.find(name);
    if (it != m_geometries.end()) {
        return it->second;
    }
    return nullptr;
}

void GeometryFactory::releaseGeometry(const std::string& name) {
    auto it = m_geometries.find(name);
    if (it != m_geometries.end()) {
        // Remove from cache - shared_ptr will handle cleanup when ref count reaches 0
        m_geometries.erase(it);
    }
}

std::shared_ptr<Geometry> GeometryFactory::createSphere(float radius, int segments) {
    std::string name = "sphere_" + std::to_string(radius) + "_" + std::to_string(segments);
    
    // Check if already exists
    auto existing = getGeometry(name);
    if (existing) {
        return existing;
    }
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Generate sphere vertices
    for (int i = 0; i <= segments; ++i) {
        float phi = M_PI * i / segments;
        for (int j = 0; j <= segments; ++j) {
            float theta = 2.0f * M_PI * j / segments;
            
            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);
            
            // Position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            
            // Normal
            vertices.push_back(x / radius);
            vertices.push_back(y / radius);
            vertices.push_back(z / radius);
        }
    }
    
    // Generate indices
    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            int first = i * (segments + 1) + j;
            int second = first + segments + 1;
            
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    
    return createGeometry(name, vertices, indices);
}

std::shared_ptr<Geometry> GeometryFactory::createCube(float size) {
    std::string name = "cube_" + std::to_string(size);
    
    // Check if already exists
    auto existing = getGeometry(name);
    if (existing) {
        return existing;
    }
    
    float half = size / 2.0f;
    
    std::vector<float> vertices = {
        // Front face
        -half, -half,  half,  0.0f,  0.0f,  1.0f,
         half, -half,  half,  0.0f,  0.0f,  1.0f,
         half,  half,  half,  0.0f,  0.0f,  1.0f,
        -half,  half,  half,  0.0f,  0.0f,  1.0f,
        
        // Back face
        -half, -half, -half,  0.0f,  0.0f, -1.0f,
        -half,  half, -half,  0.0f,  0.0f, -1.0f,
         half,  half, -half,  0.0f,  0.0f, -1.0f,
         half, -half, -half,  0.0f,  0.0f, -1.0f,
        
        // Left face
        -half,  half,  half, -1.0f,  0.0f,  0.0f,
        -half,  half, -half, -1.0f,  0.0f,  0.0f,
        -half, -half, -half, -1.0f,  0.0f,  0.0f,
        -half, -half,  half, -1.0f,  0.0f,  0.0f,
        
        // Right face
         half,  half,  half,  1.0f,  0.0f,  0.0f,
         half, -half,  half,  1.0f,  0.0f,  0.0f,
         half, -half, -half,  1.0f,  0.0f,  0.0f,
         half,  half, -half,  1.0f,  0.0f,  0.0f,
        
        // Top face
        -half,  half, -half,  0.0f,  1.0f,  0.0f,
        -half,  half,  half,  0.0f,  1.0f,  0.0f,
         half,  half,  half,  0.0f,  1.0f,  0.0f,
         half,  half, -half,  0.0f,  1.0f,  0.0f,
        
        // Bottom face
        -half, -half, -half,  0.0f, -1.0f,  0.0f,
         half, -half, -half,  0.0f, -1.0f,  0.0f,
         half, -half,  half,  0.0f, -1.0f,  0.0f,
        -half, -half,  half,  0.0f, -1.0f,  0.0f,
    };
    
    std::vector<unsigned int> indices = {
        0,  1,  2,  0,  2,  3,   // Front face
        4,  5,  6,  4,  6,  7,   // Back face
        8,  9, 10,  8, 10, 11,   // Left face
        12, 13, 14, 12, 14, 15,   // Right face
        16, 17, 18, 16, 18, 19,   // Top face
        20, 21, 22, 20, 22, 23    // Bottom face
    };
    
    return createGeometry(name, vertices, indices);
}

std::shared_ptr<Geometry> GeometryFactory::createBoundingBox() {
    std::string name = "bbox";
    
    // Check if already exists
    auto existing = getGeometry(name);
    if (existing) {
        return existing;
    }
    
    std::vector<float> vertices = {
        // Position only for bounding box wireframe
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };
    
    std::vector<unsigned int> indices = {
        0, 1, 1, 2, 2, 3, 3, 0, // Bottom face
        4, 5, 5, 6, 6, 7, 7, 4, // Top face
        0, 4, 1, 5, 2, 6, 3, 7  // Vertical edges
    };
    
    return createGeometry(name, vertices, indices);
}

void GeometryFactory::clear() {
    for (auto& pair : m_geometries) {
        pair.second->cleanup();
    }
    m_geometries.clear();
}

size_t GeometryFactory::getGeometryCount() const {
    return m_geometries.size();
}

void GeometryFactory::printStats() const {
    std::cout << "GeometryFactory Stats:" << std::endl;
    std::cout << "  Total geometries: " << m_geometries.size() << std::endl;
    
    for (const auto& pair : m_geometries) {
        std::cout << "  - " << pair.first << " (refs: " << pair.second.use_count() << ")" << std::endl;
    }
}

void GeometryFactory::createVAO(Geometry* geometry, const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    // Generate VAO
    glGenVertexArrays(1, &geometry->VAO);
    glGenBuffers(1, &geometry->VBO);
    
    glBindVertexArray(geometry->VAO);
    
    // Bind and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, geometry->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Set vertex attributes (position + normal)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Handle indices if provided
    if (!indices.empty()) {
        glGenBuffers(1, &geometry->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        geometry->indexCount = indices.size();
    }
    
    geometry->vertexCount = vertices.size() / 6; // 6 floats per vertex (pos + normal)
    
    glBindVertexArray(0);
}

} // namespace FlockingGraphics
