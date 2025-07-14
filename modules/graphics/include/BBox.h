// BBox.h - Modernized, documented, and cleaned up for maintainability
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "FlockTypes.h"
#include "Vector.h"

/**
 * @file BBox.h
 * @brief Modern BBox class using GLM for math and FlockTypes.h for type aliases.
 *
 * Simple axis-aligned bounding box for collision and visualization.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
class BBox {
public:
    BBox();
    BBox(const Vector& center, float width, float height, float depth);
    BBox(const glm::vec3& center, float width, float height, float depth);
    
    // Getters
    Vector getCenter() const { return m_center; }
    glm::vec3 getCenterGLM() const { return glm::vec3(m_center); }
    
    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }
    float getDepth() const { return m_depth; }
    
    // Legacy compatibility methods
    float width() const { return m_width; }
    float height() const { return m_height; }
    float depth() const { return m_depth; }
    
    Vector getMin() const;
    Vector getMax() const;
    glm::vec3 getMinGLM() const;
    glm::vec3 getMaxGLM() const;
    
    // Setters
    void setCenter(const Vector& center) { m_center = center; }
    void setCenter(const glm::vec3& center) { m_center = Vector(center); }
    void setDimensions(float width, float height, float depth);
    
    // Collision detection
    bool contains(const Vector& point) const;
    bool contains(const glm::vec3& point) const;
    bool intersects(const BBox& other) const;
    
    // Drawing/visualization (stub)
    void draw() const;
    void setDrawMode(int mode);
    
    // Normal array for collision detection (stub)
    std::vector<Vector> getNormalArray() const;
    
private:
    Vector m_center;
    float m_width;
    float m_height;
    float m_depth;
};
