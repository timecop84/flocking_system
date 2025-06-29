#include "BBox.h"
#include <algorithm>
#include <iostream>

BBox::BBox() 
    : m_center(0, 0, 0), m_width(1.0f), m_height(1.0f), m_depth(1.0f) 
{
}

BBox::BBox(const Vector& center, float width, float height, float depth)
    : m_center(center), m_width(width), m_height(height), m_depth(depth)
{
}

BBox::BBox(const glm::vec3& center, float width, float height, float depth)
    : m_center(Vector(center)), m_width(width), m_height(height), m_depth(depth)
{
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
    Vector min = getMin();
    return glm::vec3(min.m_x, min.m_y, min.m_z);
}

glm::vec3 BBox::getMaxGLM() const {
    Vector max = getMax();
    return glm::vec3(max.m_x, max.m_y, max.m_z);
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
    
    return (thisMin.m_x <= otherMax.m_x && thisMax.m_x >= otherMin.m_x) &&
           (thisMin.m_y <= otherMax.m_y && thisMax.m_y >= otherMin.m_y) &&
           (thisMin.m_z <= otherMax.m_z && thisMax.m_z >= otherMin.m_z);
}

void BBox::draw() const {
    // Stub implementation - would draw bounding box wireframe in real OpenGL
    std::cout << "BBox::draw() - Drawing bounding box at " << m_center << std::endl;
}
