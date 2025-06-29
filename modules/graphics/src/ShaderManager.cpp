#include "ShaderManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace graphics {

// Helper function to get OpenGL functions - implement as stub for now
static void* getGL() {
    // Return null to avoid OpenGL calls during static initialization
    // This will be properly implemented when OpenGL context is available
    return nullptr;
}

// ============================================================================
// Shader Implementation
// ============================================================================

Shader::~Shader() {
    cleanup();
}

Shader::Shader(Shader&& other) noexcept 
    : m_program(other.m_program)
    , m_shaders(std::move(other.m_shaders))
    , m_uniformLocations(std::move(other.m_uniformLocations))
    , m_compiled(other.m_compiled) {
    other.m_program = 0;
    other.m_compiled = false;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        cleanup();
        
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
    std::cout << "loadShaderFromFile called with filename: " << filename << std::endl;
    std::string source = loadShaderSource(filename);
    if (source.empty()) {
        std::cerr << "Failed to load shader source from: " << filename << std::endl;
        return false;
    }
    std::cout << "Loaded shader source, size: " << source.size() << " characters" << std::endl;
    return loadShaderFromString(type, source);
}

bool Shader::loadShaderFromString(Type type, const std::string& source) {
    void* gl = getGL();
    if (!gl) {
        std::cerr << "No valid OpenGL context" << std::endl;
        return false;
    }
    
    // Stub implementation - return false for now
    std::cout << "Shader::loadShaderFromString called (stub)" << std::endl;
    return false;
}

bool Shader::link() {
    QOpenGLFunctions* gl = getGL();
    if (!gl) {
        std::cerr << "No valid OpenGL context" << std::endl;
        return false;
    }
    
    if (m_program == 0) {
        std::cerr << "No shader program to link" << std::endl;
        return false;
    }

    gl->glLinkProgram(m_program);

    GLint success;
    gl->glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        gl->glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
        return false;
    }

    // Clean up individual shaders after linking
    for (GLuint shader : m_shaders) {
        gl->glDetachShader(m_program, shader);
        gl->glDeleteShader(shader);
    }
    m_shaders.clear();

    m_compiled = true;
    return true;
}

void Shader::use() const {
    if (isValid()) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glUseProgram(m_program);
        }
    }
}

void Shader::unuse() {
    QOpenGLFunctions* gl = getGL();
    if (gl) {
        gl->glUseProgram(0);
    }
}

void Shader::setUniform(const std::string& name, int value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glUniform1i(location, value);
        }
    }
}

void Shader::setUniform(const std::string& name, float value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glUniform1f(location, value);
        }
    }
}

void Shader::setUniform(const std::string& name, const glm::vec2& value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glUniform2fv(location, 1, glm::value_ptr(value));
        }
    }
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glUniform3fv(location, 1, glm::value_ptr(value));
        }
    }
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glUniform4fv(location, 1, glm::value_ptr(value));
        }
    }
}

void Shader::setUniform(const std::string& name, const glm::mat3& value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
    }
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
    }
}

void Shader::bindAttribute(GLuint location, const std::string& name) {
    if (m_program != 0) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            gl->glBindAttribLocation(m_program, location, name.c_str());
        }
    }
}

GLint Shader::getUniformLocation(const std::string& name) {
    auto it = m_uniformLocations.find(name);
    if (it != m_uniformLocations.end()) {
        return it->second;
    }

    QOpenGLFunctions* gl = getGL();
    if (!gl) {
        return -1;
    }

    GLint location = gl->glGetUniformLocation(m_program, name.c_str());
    m_uniformLocations[name] = location;
    
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader" << std::endl;
    }
    
    return location;
}

std::string Shader::loadShaderSource(const std::string& filename) {
    std::cout << "loadShaderSource called with filename: " << filename << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filename << std::endl;
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
        return "";
    }
    std::cout << "Shader file opened successfully" << std::endl;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string result = buffer.str();
    std::cout << "Read " << result.size() << " characters from shader file" << std::endl;
    return result;
}

bool Shader::compileShader(GLuint shader, const std::string& source) {
    QOpenGLFunctions* gl = getGL();
    if (!gl) {
        return false;
    }
    
    const char* sourcePtr = source.c_str();
    gl->glShaderSource(shader, 1, &sourcePtr, nullptr);
    gl->glCompileShader(shader);

    GLint success;
    gl->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        gl->glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }

    return true;
}

void Shader::cleanup() {
    if (m_program != 0) {
        QOpenGLFunctions* gl = getGL();
        if (gl) {
            for (GLuint shader : m_shaders) {
                gl->glDetachShader(m_program, shader);
                gl->glDeleteShader(shader);
            }
            m_shaders.clear();
            
            gl->glDeleteProgram(m_program);
            m_program = 0;
        }
    }
    m_compiled = false;
    m_uniformLocations.clear();
}

// ============================================================================
// ShaderManager Implementation
// ============================================================================

ShaderManager& ShaderManager::getInstance() {
    // Avoid static initialization during program startup
    // Use a heap-allocated singleton to delay construction
    static ShaderManager* instance = nullptr;
    if (!instance) {
        instance = new ShaderManager();
    }
    return *instance;
}

std::shared_ptr<Shader> ShaderManager::loadShader(const std::string& name,
                                                  const std::string& vertexPath,
                                                  const std::string& fragmentPath,
                                                  const std::string& geometryPath) {
    std::cout << "ShaderManager::loadShader called with name: " << name << std::endl;
    std::cout << "Vertex path: " << vertexPath << std::endl;
    std::cout << "Fragment path: " << fragmentPath << std::endl;
    
    auto shader = std::make_shared<Shader>();

    if (!shader->loadShaderFromFile(Shader::VERTEX, vertexPath)) {
        std::cerr << "Failed to load vertex shader: " << vertexPath << std::endl;
        return nullptr;
    }
    std::cout << "Vertex shader loaded successfully" << std::endl;

    if (!shader->loadShaderFromFile(Shader::FRAGMENT, fragmentPath)) {
        std::cerr << "Failed to load fragment shader: " << fragmentPath << std::endl;
        return nullptr;
    }
    std::cout << "Fragment shader loaded successfully" << std::endl;

    if (!geometryPath.empty()) {
        if (!shader->loadShaderFromFile(Shader::GEOMETRY, geometryPath)) {
            std::cerr << "Failed to load geometry shader: " << geometryPath << std::endl;
            return nullptr;
        }
    }

    std::cout << "About to link shader program..." << std::endl;
    if (!shader->link()) {
        std::cerr << "Failed to link shader program: " << name << std::endl;
        return nullptr;
    }
    std::cout << "Shader program linked successfully" << std::endl;

    m_shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ShaderManager::loadShaderFromSource(const std::string& name,
                                                            const std::string& vertexSource,
                                                            const std::string& fragmentSource,
                                                            const std::string& geometrySource) {
    auto shader = std::make_shared<Shader>();

    if (!shader->loadShaderFromString(Shader::VERTEX, vertexSource)) {
        std::cerr << "Failed to compile vertex shader for: " << name << std::endl;
        return nullptr;
    }

    if (!shader->loadShaderFromString(Shader::FRAGMENT, fragmentSource)) {
        std::cerr << "Failed to compile fragment shader for: " << name << std::endl;
        return nullptr;
    }

    if (!geometrySource.empty()) {
        if (!shader->loadShaderFromString(Shader::GEOMETRY, geometrySource)) {
            std::cerr << "Failed to compile geometry shader for: " << name << std::endl;
            return nullptr;
        }
    }

    if (!shader->link()) {
        std::cerr << "Failed to link shader program: " << name << std::endl;
        return nullptr;
    }

    m_shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& name) {
    auto it = m_shaders.find(name);
    return (it != m_shaders.end()) ? it->second : nullptr;
}

bool ShaderManager::useShader(const std::string& name) {
    auto shader = getShader(name);
    if (shader && shader->isValid()) {
        shader->use();
        m_currentShader = shader;
        return true;
    }
    return false;
}

void ShaderManager::removeShader(const std::string& name) {
    m_shaders.erase(name);
}

void ShaderManager::clear() {
    m_shaders.clear();
    m_currentShader.reset();
}

std::shared_ptr<Shader> ShaderManager::operator[](const std::string& name) {
    return getShader(name);
}

} // namespace graphics
