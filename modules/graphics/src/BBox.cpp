#include "../include/BBox.h"
#include <iostream>
#include <GL/gl.h>
#include <vector>

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
    // Draw bounding box as wireframe using immediate mode OpenGL
    Vector min = getMin();
    Vector max = getMax();
    
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    
    // Draw wireframe box
    glBegin(GL_LINES);
        // Bottom face
        glVertex3f(min.m_x, min.m_y, min.m_z);
        glVertex3f(max.m_x, min.m_y, min.m_z);
        
        glVertex3f(max.m_x, min.m_y, min.m_z);
        glVertex3f(max.m_x, min.m_y, max.m_z);
        
        glVertex3f(max.m_x, min.m_y, max.m_z);
        glVertex3f(min.m_x, min.m_y, max.m_z);
        
        glVertex3f(min.m_x, min.m_y, max.m_z);
        glVertex3f(min.m_x, min.m_y, min.m_z);
        
        // Top face
        glVertex3f(min.m_x, max.m_y, min.m_z);
        glVertex3f(max.m_x, max.m_y, min.m_z);
        
        glVertex3f(max.m_x, max.m_y, min.m_z);
        glVertex3f(max.m_x, max.m_y, max.m_z);
        
        glVertex3f(max.m_x, max.m_y, max.m_z);
        glVertex3f(min.m_x, max.m_y, max.m_z);
        
        glVertex3f(min.m_x, max.m_y, max.m_z);
        glVertex3f(min.m_x, max.m_y, min.m_z);
        
        // Vertical edges
        glVertex3f(min.m_x, min.m_y, min.m_z);
        glVertex3f(min.m_x, max.m_y, min.m_z);
        
        glVertex3f(max.m_x, min.m_y, min.m_z);
        glVertex3f(max.m_x, max.m_y, min.m_z);
        
        glVertex3f(max.m_x, min.m_y, max.m_z);
        glVertex3f(max.m_x, max.m_y, max.m_z);
        
        glVertex3f(min.m_x, min.m_y, max.m_z);
        glVertex3f(min.m_x, max.m_y, max.m_z);
    glEnd();
    
    glPopAttrib();
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
