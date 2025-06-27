#ifndef NGL_SHADERLIB_H
#define NGL_SHADERLIB_H

#include <string>
#include <map>
#include <memory>
#include <glm/glm.hpp>

namespace ngl {

enum ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader {
public:
    void use() {} // Stub implementation
};

class ShaderLib {
public:
    static ShaderLib* instance() {
        static ShaderLib instance;
        return &instance;
    }
    
    void createShaderProgram(const std::string& name) {
        m_programs[name] = std::make_shared<Shader>();
    }
    
    void use(const std::string& /*programName*/) {
        // Stub implementation
    }
    
    void attachShader(const std::string& /*name*/, ShaderType /*type*/) {}
    void loadShaderSource(const std::string& /*name*/, const std::string& /*filename*/) {}
    void compileShader(const std::string& /*name*/) {}
    void attachShaderToProgram(const std::string& /*program*/, const std::string& /*shader*/) {}
    void bindAttribute(const std::string& /*program*/, int /*location*/, const std::string& /*name*/) {}
    void linkProgramObject(const std::string& /*name*/) {}
    
    std::shared_ptr<Shader> operator[](const std::string& name) {
        return m_programs[name];
    }
    
    void setShaderParam3f(const std::string& /*name*/, float /*x*/, float /*y*/, float /*z*/) {}
    void setShaderParam4f(const std::string& /*name*/, float /*x*/, float /*y*/, float /*z*/, float /*w*/) {}
    void setShaderParamFromMatrix(const std::string& /*name*/, const glm::mat4& /*matrix*/) {}
    void setShaderParamFromMat3x3(const std::string& /*name*/, const glm::mat3& /*matrix*/) {}
    
private:
    std::map<std::string, std::shared_ptr<Shader>> m_programs;
};

} // namespace ngl

#endif // NGL_SHADERLIB_H
