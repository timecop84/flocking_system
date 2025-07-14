#include "Colour.h"

std::ostream& operator<<(std::ostream& os, const Colour& c) {
    os << "RGBA(" << c.m_r << ", " << c.m_g << ", " << c.m_b << ", " << c.m_a << ")";
    return os;
}

Colour operator*(float scalar, const Colour& c) {
    return c * scalar;
}
