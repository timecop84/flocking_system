#include "Material.h"
#include <iostream>

Material::Material() 
    : m_ambient(0.2f, 0.2f, 0.2f)
    , m_diffuse(0.8f, 0.8f, 0.8f)
    , m_specular(1.0f, 1.0f, 1.0f)
    , m_shininess(32.0f)
{
}

Material::Material(MaterialType type) : Material() {
    setMaterialType(type);
}

Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
    : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
{
}

void Material::setMaterialType(MaterialType type) {
    switch (type) {
        case GOLD:
            m_ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
            m_diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
            m_specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
            m_shininess = 51.2f;
            break;
        case SILVER:
            m_ambient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
            m_diffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
            m_specular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
            m_shininess = 51.2f;
            break;
        default:
            // Keep default values
            break;
    }
}

void Material::loadToShader() const {
    // Stub implementation
    std::cout << "Material: Loading material to default shader" << std::endl;
}

void Material::loadToShader(const std::string& uniformName) const {
    // Stub implementation
    std::cout << "Material: Loading material to shader uniform '" << uniformName << "'" << std::endl;
}
