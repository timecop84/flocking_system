#ifndef NGL_LIGHT_H
#define NGL_LIGHT_H

#include <string>
#include "Vector.h"
#include "Colour.h"
#include <glm/glm.hpp>

namespace ngl {

enum LightType {
    DIRECTIONAL,
    POINTLIGHT,
    SPOTLIGHT
};

class Light {
public:
    Light(const Vector& position, const Colour& diffuse, const Colour& specular, LightType type);
    ~Light();
    
    void setTransform(const glm::mat4& transform) { m_transform = transform; }
    void loadToShader(const std::string& /*uniformName*/) {} // Stub implementation
    
private:
    Vector m_position;
    Colour m_diffuse;
    Colour m_specular;
    LightType m_type;
    glm::mat4 m_transform;
};

} // namespace ngl

#endif // NGL_LIGHT_H