#include "../include/BBox.h"
#include <iostream>
#include <GL/gl.h>
#include <vector>
#include "../../../include/glew_compat.h"

BBox::BBox() 
    : m_center(0, 0, 0), m_width(1), m_height(1), m_depth(1) {
}

BBox::BBox(const Vector& center, float width, float height, float depth)
    : m_center(center), m_width(width), m_height(height), m_depth(depth) {
}

BBox::BBox(const glm::vec3& center, float width, float height, float depth)
    : m_center(Vector(center)), m_width(width), m_height(height), m_depth(depth) {
}

Vector BBox::getMin() const {
    return Vector(
        m_center.m_x - m_width * 0.5f,
        m_center.m_y - m_height * 0.5f,
        m_center.m_z - m_depth * 0.5f
    );
}

Vector BBox::getMax() const {
    return Vector(
        m_center.m_x + m_width * 0.5f,
        m_center.m_y + m_height * 0.5f,
        m_center.m_z + m_depth * 0.5f
    );
}

glm::vec3 BBox::getMinGLM() const {
    return glm::vec3(getMin());
}

glm::vec3 BBox::getMaxGLM() const {
    return glm::vec3(getMax());
}

void BBox::setDimensions(float width, float height, float depth) {
    m_width = width;
    m_height = height;
    m_depth = depth;
}

bool BBox::contains(const Vector& point) const {
    Vector min = getMin();
    Vector max = getMax();
    
    return point.m_x >= min.m_x && point.m_x <= max.m_x &&
           point.m_y >= min.m_y && point.m_y <= max.m_y &&
           point.m_z >= min.m_z && point.m_z <= max.m_z;
}

bool BBox::contains(const glm::vec3& point) const {
    return contains(Vector(point));
}

bool BBox::intersects(const BBox& other) const {
    Vector thisMin = getMin();
    Vector thisMax = getMax();
    Vector otherMin = other.getMin();
    Vector otherMax = other.getMax();
    
    return !(thisMax.m_x < otherMin.m_x || thisMin.m_x > otherMax.m_x ||
             thisMax.m_y < otherMin.m_y || thisMin.m_y > otherMax.m_y ||
             thisMax.m_z < otherMin.m_z || thisMin.m_z > otherMax.m_z);
}

void BBox::draw() const {
    // Modern wireframe box rendering using VAO/VBO
    Vector min = getMin();
    Vector max = getMax();
    
    // Define vertices for a wireframe box (24 vertices, 12 lines)
    std::vector<glm::vec3> vertices = {
        // Bottom face
        glm::vec3(min.m_x, min.m_y, min.m_z), glm::vec3(max.m_x, min.m_y, min.m_z),
        glm::vec3(max.m_x, min.m_y, min.m_z), glm::vec3(max.m_x, min.m_y, max.m_z),
        glm::vec3(max.m_x, min.m_y, max.m_z), glm::vec3(min.m_x, min.m_y, max.m_z),
        glm::vec3(min.m_x, min.m_y, max.m_z), glm::vec3(min.m_x, min.m_y, min.m_z),
        
        // Top face
        glm::vec3(min.m_x, max.m_y, min.m_z), glm::vec3(max.m_x, max.m_y, min.m_z),
        glm::vec3(max.m_x, max.m_y, min.m_z), glm::vec3(max.m_x, max.m_y, max.m_z),
        glm::vec3(max.m_x, max.m_y, max.m_z), glm::vec3(min.m_x, max.m_y, max.m_z),
        glm::vec3(min.m_x, max.m_y, max.m_z), glm::vec3(min.m_x, max.m_y, min.m_z),
        
        // Vertical edges
        glm::vec3(min.m_x, min.m_y, min.m_z), glm::vec3(min.m_x, max.m_y, min.m_z),
        glm::vec3(max.m_x, min.m_y, min.m_z), glm::vec3(max.m_x, max.m_y, min.m_z),
        glm::vec3(max.m_x, min.m_y, max.m_z), glm::vec3(max.m_x, max.m_y, max.m_z),
        glm::vec3(min.m_x, min.m_y, max.m_z), glm::vec3(min.m_x, max.m_y, max.m_z)
    };
    
    // Create temporary VAO/VBO for wireframe rendering
    static unsigned int VAO = 0, VBO = 0;
    static bool initialized = false;
    
    if (!initialized) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        initialized = true;
    }
    
    // Bind and upload vertex data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
    
    // Set up vertex attributes for position only
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Set line width for wireframe
    glLineWidth(2.0f);
    
    // Draw the wireframe box
    glDrawArrays(GL_LINES, 0, vertices.size());
    
    // Unbind VAO
    glBindVertexArray(0);
}

void BBox::setDrawMode(int mode) {
    // Stub implementation - would set GL_LINE, GL_FILL, etc.
    std::cout << "BBox: Setting draw mode to " << mode << std::endl;
}

std::vector<Vector> BBox::getNormalArray() const {
    // Return a simple set of face normals for a box
    std::vector<Vector> normals;
    normals.push_back(Vector(1, 0, 0));   // +X face
    normals.push_back(Vector(-1, 0, 0));  // -X face
    normals.push_back(Vector(0, 1, 0));   // +Y face
    normals.push_back(Vector(0, -1, 0));  // -Y face
    normals.push_back(Vector(0, 0, 1));   // +Z face
    normals.push_back(Vector(0, 0, -1));  // -Z face
    return normals;
}
