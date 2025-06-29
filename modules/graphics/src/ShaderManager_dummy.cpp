#include "ShaderManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

// Dummy OpenGL types for compilation without Qt/OpenGL
#ifndef GL_VERTEX_SHADER
typedef unsigned int GLuint;
typedef int GLint;
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_COMPUTE_SHADER 0x91B9
#endif

namespace graphics {

// Dummy Shader class implementations
Shader::~Shader() {
    // Dummy destructor
}

Shader::Shader(Shader&& other) noexcept 
    : m_program(other.m_program), m_shaders(std::move(other.m_shaders)), 
      m_uniformLocations(std::move(other.m_uniformLocations)), m_compiled(other.m_compiled) {
    other.m_program = 0;
    other.m_compiled = false;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        m_program = other.m_program;
        m_shaders = std::move(other.m_shaders);
        m_uniformLocations = std::move(other.m_uniformLocations);
        m_compiled = other.m_compiled;
        other.m_program = 0;
        other.m_compiled = false;
    }
    return *this;
}

bool Shader::loadShaderFromFile(Type type, const std::string& filename) {
    std::cout << "Shader::loadShaderFromFile (dummy): " << filename << std::endl;
    return false;
}

bool Shader::loadShaderFromString(Type type, const std::string& source) {
    std::cout << "Shader::loadShaderFromString (dummy)" << std::endl;
    return false;
}

bool Shader::link() {
    std::cout << "Shader::link (dummy)" << std::endl;
    return false;
}

void Shader::use() const {
    std::cout << "Shader::use (dummy)" << std::endl;
}

void Shader::unuse() {
    std::cout << "Shader::unuse (dummy)" << std::endl;
}

void Shader::setUniform(const std::string& name, int value) {
    std::cout << "Shader::setUniform (dummy): " << name << " = " << value << std::endl;
}

void Shader::setUniform(const std::string& name, float value) {
    std::cout << "Shader::setUniform (dummy): " << name << " = " << value << std::endl;
}

void Shader::setUniform(const std::string& name, const glm::vec2& value) {
    std::cout << "Shader::setUniform (dummy): " << name << " = vec2" << std::endl;
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) {
    std::cout << "Shader::setUniform (dummy): " << name << " = vec3" << std::endl;
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) {
    std::cout << "Shader::setUniform (dummy): " << name << " = vec4" << std::endl;
}

void Shader::setUniform(const std::string& name, const glm::mat3& value) {
    std::cout << "Shader::setUniform (dummy): " << name << " = mat3" << std::endl;
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) {
    std::cout << "Shader::setUniform (dummy): " << name << " = mat4" << std::endl;
}

void Shader::bindAttribute(GLuint location, const std::string& name) {
    std::cout << "Shader::bindAttribute (dummy): " << name << " at " << location << std::endl;
}

GLint Shader::getUniformLocation(const std::string& name) {
    std::cout << "Shader::getUniformLocation (dummy): " << name << std::endl;
    return -1;
}

std::string Shader::loadShaderSource(const std::string& filename) {
    std::cout << "Shader::loadShaderSource (dummy): " << filename << std::endl;
    return "";
}

bool Shader::compileShader(GLuint shader, const std::string& source) {
    std::cout << "Shader::compileShader (dummy)" << std::endl;
    return false;
}

void Shader::cleanup() {
    std::cout << "Shader::cleanup (dummy)" << std::endl;
}

// Dummy implementations to avoid Qt OpenGL dependencies during static init
ShaderManager& ShaderManager::getInstance() {
    static ShaderManager* instance = nullptr;
    if (!instance) {
        instance = new ShaderManager();
    }
    return *instance;
}

std::shared_ptr<Shader> ShaderManager::loadShader(const std::string& name,
                                                  const std::string& vertexShaderFile,
                                                  const std::string& fragmentShaderFile,
                                                  const std::string& geometryShaderFile) {
    std::cout << "ShaderManager::loadShader (dummy): " << name << std::endl;
    return nullptr;
}

std::shared_ptr<Shader> ShaderManager::loadShaderFromSource(const std::string& name,
                                                            const std::string& vertexSource,
                                                            const std::string& fragmentSource,
                                                            const std::string& geometrySource) {
    std::cout << "ShaderManager::loadShaderFromSource (dummy): " << name << std::endl;
    return nullptr;
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& name) {
    std::cout << "ShaderManager::getShader (dummy): " << name << std::endl;
    return nullptr;
}

bool ShaderManager::useShader(const std::string& name) {
    std::cout << "ShaderManager::useShader (dummy): " << name << std::endl;
    return false;
}

void ShaderManager::removeShader(const std::string& name) {
    std::cout << "ShaderManager::removeShader (dummy): " << name << std::endl;
}

void ShaderManager::clear() {
    std::cout << "ShaderManager::clear (dummy)" << std::endl;
}

std::shared_ptr<Shader> ShaderManager::operator[](const std::string& name) {
    return getShader(name);
}

} // namespace graphics
