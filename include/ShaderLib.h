#ifndef SHADERLIB_H
#define SHADERLIB_H

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern ShaderLib stub using GLM instead of ngl_compat
/// @details Basic shader library replacement - provides interface compatibility
//----------------------------------------------------------------------------------------------------------------------
class ShaderLib {
public:
    enum ShaderType {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        COMPUTE
    };
    
    static ShaderLib* instance();
    
    // Shader management
    void createShader(const std::string& name);
    void createShaderProgram(const std::string& name);
    void attachShader(const std::string& name, int type);
    void loadShaderSource(const std::string& name, const std::string& filename);
    void compileShader(const std::string& name);
    void attachShaderToProgram(const std::string& program, const std::string& shader);
    void bindAttribute(const std::string& program, int index, const std::string& name);
    void linkProgramObject(const std::string& name);
    void use(const std::string& name);
    
    // Operator for program access
    class ProgramWrapper {
    public:
        void use() {}
    };
    ProgramWrapper* operator[](const std::string& name);
    
    // Uniform setters
    void setShaderParam(const std::string& paramName, const glm::mat4& value);
    void setShaderParam(const std::string& paramName, const glm::mat3& value);
    void setShaderParam(const std::string& paramName, const glm::vec4& value);
    void setShaderParam(const std::string& paramName, const glm::vec3& value);
    void setShaderParam(const std::string& paramName, float value);
    void setShaderParam(const std::string& paramName, int value);
    void setShaderParam3f(const std::string& paramName, float x, float y, float z);
    void setShaderParam4f(const std::string& paramName, float x, float y, float z, float w);
    void setShaderParamFromMatrix(const std::string& paramName, const glm::mat4& matrix);
    void setShaderParamFromMat3x3(const std::string& paramName, const glm::mat3& matrix);
    
    // Legacy compatibility
    void setRegisteredUniform(const std::string& uniformName, const glm::mat4& value);
    void setRegisteredUniform(const std::string& uniformName, const glm::mat3& value);
    void setRegisteredUniform(const std::string& uniformName, const glm::vec4& value);
    void setRegisteredUniform(const std::string& uniformName, const glm::vec3& value);
    void setRegisteredUniform(const std::string& uniformName, float value);
    
private:
    ShaderLib() = default;
    static ShaderLib* s_instance;
    
    std::unordered_map<std::string, unsigned int> m_programs;
    unsigned int m_currentProgram;
};

#endif // SHADERLIB_H
