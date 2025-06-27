#include "../include/ngl_compat/Light.h"

namespace ngl {

Light::Light(const Vector& position, const Colour& diffuse, const Colour& specular, LightType type)
    : m_position(position), m_diffuse(diffuse), m_specular(specular), m_type(type) {
    m_transform = glm::mat4(1.0f);
}

Light::~Light() {
}

} // namespace ngl