#ifndef NGL_MATERIAL_H
#define NGL_MATERIAL_H

#include <string>
#include "Colour.h"

namespace ngl {

enum MaterialType {
    GOLD,
    SILVER,
    BRONZE,
    BLACKPLASTIC,
    PEWTER
};

class Material {
public:
    Material() : m_type(GOLD) {}
    Material(MaterialType type) : m_type(type) {}
    
    void set(MaterialType type) {
        m_type = type;
    }
    
    void setDiffuse(const Colour& /*colour*/) {
        // Stub implementation
    }
    
    void loadToShader(const std::string& /*uniformName*/) {
        // Stub implementation
    }
    
private:
    MaterialType m_type;
};

} // namespace ngl

#endif // NGL_MATERIAL_H
