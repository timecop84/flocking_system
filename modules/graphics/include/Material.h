#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <string>

/// @brief Modern Material class for compatibility
/// @details Simple material representation

// Material presets
enum MaterialType {
    GOLD,
    SILVER,
    BRONZE,
    RUBY,
    EMERALD,
    PEARL,
    PLASTIC,
    CHROME,
    BRASS,
    COPPER
};

class Material {
public:
    Material();
    Material(MaterialType type);
    Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess);
    
    // Getters
    const glm::vec3& getAmbient() const { return m_ambient; }
    const glm::vec3& getDiffuse() const { return m_diffuse; }
    const glm::vec3& getSpecular() const { return m_specular; }
    float getShininess() const { return m_shininess; }
    
    // Setters
    void setAmbient(const glm::vec3& ambient) { m_ambient = ambient; }
    void setDiffuse(const glm::vec3& diffuse) { m_diffuse = diffuse; }
    void setSpecular(const glm::vec3& specular) { m_specular = specular; }
    void setShininess(float shininess) { m_shininess = shininess; }

    // Apply material (stub)
    void apply() const;
    
    // Load to shader
    void loadToShader() const;
    void loadToShader(const std::string& uniformName) const;
    
private:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse; 
    glm::vec3 m_specular;
    float m_shininess;
    
    void setMaterialType(MaterialType type);
};

#endif // MATERIAL_H
