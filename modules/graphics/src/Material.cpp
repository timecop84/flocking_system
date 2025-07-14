#include "Material.h"
#include <UBOStructures.h>
#include <ShaderLib.h>
#include <Colour.h>
#include <iostream> // Include iostream for debug output

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
    // Fill MaterialBlock struct with current material values
    FlockingShaders::MaterialBlock block;
    Colour ambient = getAmbient();
    Colour diffuse = getDiffuse();
    Colour specular = getSpecular();
    block.ambient = glm::vec4(ambient.m_r, ambient.m_g, ambient.m_b, ambient.m_a);
    block.diffuse = glm::vec4(diffuse.m_r, diffuse.m_g, diffuse.m_b, diffuse.m_a);
    block.specular = glm::vec4(specular.m_r, specular.m_g, specular.m_b, specular.m_a);
    block.shininess = getShininess();
    block.padding[0] = block.padding[1] = block.padding[2] = 0.0f;
    
    // Debug output
    std::cout << "[Material] loadToShader: diffuse=" << diffuse.m_r << ", " << diffuse.m_g << ", " << diffuse.m_b << std::endl;
    
    // Update the UBO (binding point 1, name is always "MaterialUBO")
    ShaderLib::instance()->updateUBO("MaterialUBO", &block, sizeof(FlockingShaders::MaterialBlock));
}

void Material::loadToShader(const std::string& /*uniformName*/) const {
    // Overload for API compatibility; same as above
    loadToShader();
}
