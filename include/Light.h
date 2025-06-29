#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <string>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern Light class using GLM instead of ngl_compat
/// @details Simple light representation for basic lighting calculations
//----------------------------------------------------------------------------------------------------------------------
class Light {
public:
    Light();
    Light(const glm::vec3& position, const glm::vec3& color = glm::vec3(1.0f));
    Light(const class Vector& position, const class Colour& diffuse, const class Colour& specular, int lightType);
    
    // Position
    void setPosition(const glm::vec3& position) { m_position = position; }
    glm::vec3 getPosition() const { return m_position; }
    
    // Transform
    void setTransform(const glm::mat4& transform);
    
    // Color/intensity
    void setColour(const glm::vec3& color) { m_color = color; }
    void setColor(const glm::vec3& color) { m_color = color; }
    glm::vec3 getColour() const { return m_color; }
    glm::vec3 getColor() const { return m_color; }
    
    // Attenuation
    void setAttenuation(float constant, float linear, float quadratic);
    void setConstantAttenuation(float constant) { m_constantAttenuation = constant; }
    void setLinearAttenuation(float linear) { m_linearAttenuation = linear; }
    void setQuadraticAttenuation(float quadratic) { m_quadraticAttenuation = quadratic; }
    
    float getConstantAttenuation() const { return m_constantAttenuation; }
    float getLinearAttenuation() const { return m_linearAttenuation; }
    float getQuadraticAttenuation() const { return m_quadraticAttenuation; }
    
    // Enable/disable
    void enable() { m_enabled = true; }
    void disable() { m_enabled = false; }
    bool isEnabled() const { return m_enabled; }
    
    // Shader loading
    void loadToShader(const std::string& uniformName) const;
    
private:
    glm::vec3 m_position;
    glm::vec3 m_color;
    glm::mat4 m_transform;
    
    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
    
    bool m_enabled;
};

#endif // LIGHT_H
