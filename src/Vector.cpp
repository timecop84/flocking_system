#include "Vector.h"
#include <glm/geometric.hpp>
#include <cmath>

float Vector::length() const {
    return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

float Vector::lengthSquared() const {
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

Vector Vector::normalize() const {
    float len = length();
    if (len > 0.0f) {
        return Vector(m_x / len, m_y / len, m_z / len);
    }
    return Vector(0, 0, 0);
}

void Vector::normalizeIP() {
    float len = length();
    if (len > 0.0f) {
        m_x /= len;
        m_y /= len;
        m_z /= len;
    }
}

float Vector::dot(const Vector& v) const {
    return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
}

Vector Vector::cross(const Vector& v) const {
    return Vector(
        m_y * v.m_z - m_z * v.m_y,
        m_z * v.m_x - m_x * v.m_z,
        m_x * v.m_y - m_y * v.m_x
    );
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << "(" << v.m_x << ", " << v.m_y << ", " << v.m_z << ")";
    return os;
}

Vector operator*(float scalar, const Vector& v) {
    return v * scalar;
}
