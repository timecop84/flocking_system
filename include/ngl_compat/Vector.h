#ifndef NGL_VECTOR_H
#define NGL_VECTOR_H

#include <glm/glm.hpp>
#include <cmath>

namespace ngl {

class Vector {
public:
    float m_x, m_y, m_z, m_w;
    
    Vector() : m_x(0.0f), m_y(0.0f), m_z(0.0f), m_w(1.0f) {}
    Vector(float x, float y, float z, float w = 1.0f) : m_x(x), m_y(y), m_z(z), m_w(w) {}
    Vector(const glm::vec3& v) : m_x(v.x), m_y(v.y), m_z(v.z), m_w(1.0f) {}
    Vector(const glm::vec4& v) : m_x(v.x), m_y(v.y), m_z(v.z), m_w(v.w) {}
    Vector(const Vector& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z), m_w(other.m_w) {}
    
    void set(float x, float y, float z, float w = 1.0f) {
        m_x = x; m_y = y; m_z = z; m_w = w;
    }
    
    void set(const Vector& other) {
        m_x = other.m_x; m_y = other.m_y; m_z = other.m_z; m_w = other.m_w;
    }
    
    // Conversion operators
    operator glm::vec3() const { return glm::vec3(m_x, m_y, m_z); }
    operator glm::vec4() const { return glm::vec4(m_x, m_y, m_z, m_w); }
    
    // Arithmetic operators
    Vector operator+(const Vector& rhs) const {
        return Vector(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z, m_w + rhs.m_w);
    }
    
    Vector operator-(const Vector& rhs) const {
        return Vector(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z, m_w - rhs.m_w);
    }
    
    Vector operator*(float scalar) const {
        return Vector(m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar);
    }
    
    // Component-wise multiplication
    Vector operator*(const Vector& rhs) const {
        return Vector(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z, m_w * rhs.m_w);
    }
    
    Vector& operator+=(const Vector& rhs) {
        m_x += rhs.m_x; m_y += rhs.m_y; m_z += rhs.m_z; m_w += rhs.m_w;
        return *this;
    }
    
    Vector& operator-=(const Vector& rhs) {
        m_x -= rhs.m_x; m_y -= rhs.m_y; m_z -= rhs.m_z; m_w -= rhs.m_w;
        return *this;
    }
    
    Vector& operator*=(float scalar) {
        m_x *= scalar; m_y *= scalar; m_z *= scalar; m_w *= scalar;
        return *this;
    }
    
    Vector& operator/=(float scalar) {
        if (scalar != 0.0f) {
            m_x /= scalar; m_y /= scalar; m_z /= scalar; m_w /= scalar;
        }
        return *this;
    }
    
    Vector& operator=(const Vector& rhs) {
        m_x = rhs.m_x; m_y = rhs.m_y; m_z = rhs.m_z; m_w = rhs.m_w;
        return *this;
    }
    
    // Utility functions
    float length() const {
        return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
    }
    
    float lengthSquared() const {
        return m_x * m_x + m_y * m_y + m_z * m_z;
    }
    
    float dot(const Vector& other) const {
        return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
    }
    
    void normalize() {
        float len = length();
        if (len > 0.0f) {
            m_x /= len; m_y /= len; m_z /= len;
        }
    }
    
    Vector normalized() const {
        Vector result = *this;
        result.normalize();
        return result;
    }
};

// Global operators for scalar-vector operations
inline Vector operator*(float scalar, const Vector& vec) {
    return Vector(scalar * vec.m_x, scalar * vec.m_y, scalar * vec.m_z, scalar * vec.m_w);
}

} // namespace ngl

#endif // NGL_VECTOR_H