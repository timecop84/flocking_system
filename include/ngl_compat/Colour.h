#ifndef NGL_COLOUR_H
#define NGL_COLOUR_H

namespace ngl {

class Colour {
public:
    float m_r, m_g, m_b, m_a;
    
    Colour() : m_r(1.0f), m_g(1.0f), m_b(1.0f), m_a(1.0f) {}
    Colour(float r, float g, float b, float a = 1.0f) : m_r(r), m_g(g), m_b(b), m_a(a) {}
    Colour(const Colour& other) : m_r(other.m_r), m_g(other.m_g), m_b(other.m_b), m_a(other.m_a) {}
    
    void set(float r, float g, float b, float a = 1.0f) {
        m_r = r; m_g = g; m_b = b; m_a = a;
    }
    
    Colour& operator=(const Colour& other) {
        m_r = other.m_r; m_g = other.m_g; m_b = other.m_b; m_a = other.m_a;
        return *this;
    }
};

} // namespace ngl

#endif // NGL_COLOUR_H