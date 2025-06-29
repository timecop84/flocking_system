#include "Light.h"
#include "Vector.h"
#include "Colour.h"

Light::Light()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_color(1.0f, 1.0f, 1.0f)
    , m_constantAttenuation(1.0f)
    , m_linearAttenuation(0.0f)
    , m_quadraticAttenuation(0.0f)
    , m_enabled(true)
{
}

Light::Light(const glm::vec3& position, const glm::vec3& color)
    : m_position(position)
    , m_color(color)
    , m_constantAttenuation(1.0f)
    , m_linearAttenuation(0.0f)
    , m_quadraticAttenuation(0.0f)
    , m_enabled(true)
{
}

Light::Light(const Vector& position, const Colour& diffuse, const Colour& specular, int lightType)
    : m_position(position.m_x, position.m_y, position.m_z)
    , m_color(diffuse.m_r, diffuse.m_g, diffuse.m_b)
    , m_constantAttenuation(1.0f)
    , m_linearAttenuation(0.0f)
    , m_quadraticAttenuation(0.0f)
    , m_enabled(true)
{
    // lightType parameter accepted but not used in this stub
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
    m_constantAttenuation = constant;
    m_linearAttenuation = linear;
    m_quadraticAttenuation = quadratic;
}

void Light::setTransform(const glm::mat4& transform) {
    m_transform = transform;
}

void Light::loadToShader(const std::string& uniformName) const {
    // Stub implementation
}
