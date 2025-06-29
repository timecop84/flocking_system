#ifndef VECTOR_H
#define VECTOR_H

#include <glm/glm.hpp>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern Vector class using GLM as backend instead of ngl_compat
/// @details Interface-compatible replacement for ngl::Vector using glm::vec3
//----------------------------------------------------------------------------------------------------------------------
class Vector {
public:
    float m_x, m_y, m_z;
    
    Vector() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
    Vector(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
    Vector(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z) {} // w component ignored for 3D vector
    Vector(const glm::vec3& v) : m_x(v.x), m_y(v.y), m_z(v.z) {}
    
    // Conversion operators
    operator glm::vec3() const { return glm::vec3(m_x, m_y, m_z); }
    
    // Setters
    void set(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
    void set(const Vector& v) { m_x = v.m_x; m_y = v.m_y; m_z = v.m_z; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    void setZ(float z) { m_z = z; }
    
    // Getters (for compatibility)
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    float getZ() const { return m_z; }
    
    // Operators
    Vector operator+(const Vector& v) const { return Vector(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z); }
    Vector operator-(const Vector& v) const { return Vector(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z); }
    Vector operator*(float scalar) const { return Vector(m_x * scalar, m_y * scalar, m_z * scalar); }
    Vector operator*(const Vector& v) const { return Vector(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z); }
    Vector operator/(float scalar) const { return Vector(m_x / scalar, m_y / scalar, m_z / scalar); }
    
    Vector& operator+=(const Vector& v) { m_x += v.m_x; m_y += v.m_y; m_z += v.m_z; return *this; }
    Vector& operator-=(const Vector& v) { m_x -= v.m_x; m_y -= v.m_y; m_z -= v.m_z; return *this; }
    Vector& operator*=(float scalar) { m_x *= scalar; m_y *= scalar; m_z *= scalar; return *this; }
    Vector& operator/=(float scalar) { m_x /= scalar; m_y /= scalar; m_z /= scalar; return *this; }
    
    bool operator==(const Vector& v) const { return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z; }
    bool operator!=(const Vector& v) const { return !(*this == v); }
    
    // Utility functions
    float length() const;
    float lengthSquared() const;
    Vector normalize() const;
    void normalizeIP(); // In-place normalize
    float dot(const Vector& v) const;
    Vector cross(const Vector& v) const;
    
    // Static functions for compatibility
    static Vector zero() { return Vector(0, 0, 0); }
    static Vector up() { return Vector(0, 1, 0); }
    static Vector right() { return Vector(1, 0, 0); }
    static Vector forward() { return Vector(0, 0, -1); }
    
    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const Vector& v);
};

// Global operators for scalar multiplication
Vector operator*(float scalar, const Vector& v);

#endif // VECTOR_H
