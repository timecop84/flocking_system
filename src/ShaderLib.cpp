#include "ShaderLib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QOpenGLFunctions>
#include <glm/gtc/type_ptr.hpp>

ShaderLib* ShaderLib::s_instance = nullptr;

ShaderLib* ShaderLib::instance() {
    if (!s_instance) {
        s_instance = new ShaderLib();
    }
    return s_instance;
}

struct ShaderInfo {
    int type;
    std::string source;
};

static std::unordered_map<std::string, ShaderInfo> m_shaderSources;
static std::unordered_map<std::string, unsigned int> m_compiledShaders;

void ShaderLib::createShader(const std::string& name) {
    std::cout << "ShaderLib: Creating shader '" << name << "'" << std::endl;
    // This creates individual shader objects, not needed for our simplified approach
}

void ShaderLib::createShaderProgram(const std::string& name) {
    std::cout << "ShaderLib: Creating shader program '" << name << "'" << std::endl;
    // Note: For Qt OpenGL, we need to ensure we have a valid OpenGL context
    // For now, use a placeholder and implement actual OpenGL calls when context is available
    unsigned int program = 1; // Placeholder - will be set when we implement full OpenGL
    m_programs[name] = program;
    std::cout << "Created program placeholder ID: " << program << std::endl;
}

void ShaderLib::attachShader(const std::string& name, int type) {
    std::cout << "ShaderLib: Attaching shader '" << name << "' of type " << type << std::endl;
    // Store shader info for later compilation
    m_shaderSources[name] = {type, ""};
}

void ShaderLib::loadShaderSource(const std::string& name, const std::string& filename) {
    std::cout << "ShaderLib: Loading shader source '" << name << "' from '" << filename << "' (stub)" << std::endl;
    // Stub for now - don't actually load files to avoid hanging
    m_shaderSources[name].source = "// Stub shader source";
}

void ShaderLib::compileShader(const std::string& name) {
    std::cout << "ShaderLib: Compiling shader '" << name << "' (stub)" << std::endl;
    // Stub for now - will be implemented with proper OpenGL context
}

void ShaderLib::attachShaderToProgram(const std::string& program, const std::string& shader) {
    std::cout << "ShaderLib: Attaching shader '" << shader << "' to program '" << program << "' (stub)" << std::endl;
}

void ShaderLib::bindAttribute(const std::string& program, int index, const std::string& name) {
    std::cout << "ShaderLib: Binding attribute '" << name << "' to index " << index << " in program '" << program << "' (stub)" << std::endl;
}

void ShaderLib::linkProgramObject(const std::string& name) {
    std::cout << "ShaderLib: Linking program '" << name << "' (stub)" << std::endl;
}

void ShaderLib::use(const std::string& name) {
    std::cout << "ShaderLib: Using program '" << name << "' (stub)" << std::endl;
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        m_currentProgram = it->second;
    }
}

void ShaderLib::setShaderParam(const std::string& paramName, const glm::mat4& value) {
    // Stub for now
    (void)paramName;
    (void)value;
}

void ShaderLib::setShaderParam(const std::string& paramName, const glm::mat3& value) {
    // Stub for now
    (void)paramName;
    (void)value;
}

void ShaderLib::setShaderParam(const std::string& paramName, const glm::vec4& value) {
    // Stub for now
    (void)paramName;
    (void)value;
}

void ShaderLib::setShaderParam(const std::string& paramName, const glm::vec3& value) {
    // Stub for now
    (void)paramName;
    (void)value;
}

void ShaderLib::setShaderParam(const std::string& paramName, float value) {
    // Stub for now
    (void)paramName;
    (void)value;
}

void ShaderLib::setShaderParam(const std::string& paramName, int value) {
    // Stub for now
    (void)paramName;
    (void)value;
}

void ShaderLib::setShaderParam3f(const std::string& paramName, float x, float y, float z) {
    // Stub for now
    (void)paramName;
    (void)x;
    (void)y;
    (void)z;
}

void ShaderLib::setShaderParam4f(const std::string& paramName, float x, float y, float z, float w) {
    // Stub for now
    (void)paramName;
    (void)x;
    (void)y;
    (void)z;
    (void)w;
}

void ShaderLib::setShaderParamFromMatrix(const std::string& paramName, const glm::mat4& matrix) {
    setShaderParam(paramName, matrix);
}

void ShaderLib::setShaderParamFromMat3x3(const std::string& paramName, const glm::mat3& matrix) {
    setShaderParam(paramName, matrix);
}

void ShaderLib::setRegisteredUniform(const std::string& uniformName, const glm::mat4& value) {
    setShaderParam(uniformName, value);
}

void ShaderLib::setRegisteredUniform(const std::string& uniformName, const glm::mat3& value) {
    setShaderParam(uniformName, value);
}

void ShaderLib::setRegisteredUniform(const std::string& uniformName, const glm::vec4& value) {
    setShaderParam(uniformName, value);
}

void ShaderLib::setRegisteredUniform(const std::string& uniformName, const glm::vec3& value) {
    setShaderParam(uniformName, value);
}

void ShaderLib::setRegisteredUniform(const std::string& uniformName, float value) {
    setShaderParam(uniformName, value);
}

ShaderLib::ProgramWrapper* ShaderLib::operator[](const std::string& name) {
    static ProgramWrapper wrapper;
    return &wrapper;
}
