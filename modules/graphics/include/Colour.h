#ifndef COLOUR_H
#define COLOUR_H

#include <glm/glm.hpp>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern Colour class using GLM as backend instead of ngl_compat
/// @details Interface-compatible replacement for ngl::Colour using glm::vec4
//----------------------------------------------------------------------------------------------------------------------
class Colour {
public:
    float m_r, m_g, m_b, m_a;
    
    Colour() : m_r(1.0f), m_g(1.0f), m_b(1.0f), m_a(1.0f) {}
    Colour(float r, float g, float b, float a = 1.0f) : m_r(r), m_g(g), m_b(b), m_a(a) {}
    Colour(const glm::vec3& rgb, float a = 1.0f) : m_r(rgb.x), m_g(rgb.y), m_b(rgb.z), m_a(a) {}
    Colour(const glm::vec4& rgba) : m_r(rgba.x), m_g(rgba.y), m_b(rgba.z), m_a(rgba.w) {}
    
    // Conversion operators
    operator glm::vec3() const { return glm::vec3(m_r, m_g, m_b); }
    operator glm::vec4() const { return glm::vec4(m_r, m_g, m_b, m_a); }
    
    // Setters
    void set(float r, float g, float b, float a = 1.0f) { m_r = r; m_g = g; m_b = b; m_a = a; }
    void setR(float r) { m_r = r; }
    void setG(float g) { m_g = g; }
    void setB(float b) { m_b = b; }
    void setA(float a) { m_a = a; }
    
    // Getters
    float getR() const { return m_r; }
    float getG() const { return m_g; }
    float getB() const { return m_b; }
    float getA() const { return m_a; }
    
    // Operators
    Colour operator+(const Colour& c) const { return Colour(m_r + c.m_r, m_g + c.m_g, m_b + c.m_b, m_a + c.m_a); }
    Colour operator-(const Colour& c) const { return Colour(m_r - c.m_r, m_g - c.m_g, m_b - c.m_b, m_a - c.m_a); }
    Colour operator*(float scalar) const { return Colour(m_r * scalar, m_g * scalar, m_b * scalar, m_a * scalar); }
    Colour operator*(const Colour& c) const { return Colour(m_r * c.m_r, m_g * c.m_g, m_b * c.m_b, m_a * c.m_a); }
    
    Colour& operator+=(const Colour& c) { m_r += c.m_r; m_g += c.m_g; m_b += c.m_b; m_a += c.m_a; return *this; }
    Colour& operator-=(const Colour& c) { m_r -= c.m_r; m_g -= c.m_g; m_b -= c.m_b; m_a -= c.m_a; return *this; }
    Colour& operator*=(float scalar) { m_r *= scalar; m_g *= scalar; m_b *= scalar; m_a *= scalar; return *this; }
    
    bool operator==(const Colour& c) const { return m_r == c.m_r && m_g == c.m_g && m_b == c.m_b && m_a == c.m_a; }
    bool operator!=(const Colour& c) const { return !(*this == c); }
    
    // Predefined colors
    static Colour white() { return Colour(1.0f, 1.0f, 1.0f, 1.0f); }
    static Colour black() { return Colour(0.0f, 0.0f, 0.0f, 1.0f); }
    static Colour red() { return Colour(1.0f, 0.0f, 0.0f, 1.0f); }
    static Colour green() { return Colour(0.0f, 1.0f, 0.0f, 1.0f); }
    static Colour blue() { return Colour(0.0f, 0.0f, 1.0f, 1.0f); }
    static Colour yellow() { return Colour(1.0f, 1.0f, 0.0f, 1.0f); }
    static Colour cyan() { return Colour(0.0f, 1.0f, 1.0f, 1.0f); }
    static Colour magenta() { return Colour(1.0f, 0.0f, 1.0f, 1.0f); }
    
    // Stream operator
    friend std::ostream& operator<<(std::ostream& os, const Colour& c);
};

// Global operators for scalar multiplication
Colour operator*(float scalar, const Colour& c);

#endif // COLOUR_H
