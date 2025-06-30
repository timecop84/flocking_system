#include "ShaderLib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QFile>
#include <QTextStream>
#include <glm/gtc/type_ptr.hpp>

ShaderLib* ShaderLib::s_instance = nullptr;

ShaderLib* ShaderLib::instance() {
    if (!s_instance) {
        s_instance = new ShaderLib();
        // Initialize OpenGL functions
        auto context = QOpenGLContext::currentContext();
        if (context) {
            s_instance->m_gl = context->functions();
            if (!s_instance->m_gl) {
                std::cerr << "Failed to get OpenGL functions from Qt context" << std::endl;
            }
        } else {
            std::cerr << "No current OpenGL context available" << std::endl;
        }
    }
    return s_instance;
}

void ShaderLib::createShader(const std::string& name) {
    std::cout << "ShaderLib: Creating shader '" << name << "'" << std::endl;
    // Individual shaders are created when attachShader is called
}

void ShaderLib::createShaderProgram(const std::string& name) {
    std::cout << "ShaderLib: Creating shader program '" << name << "'" << std::endl;
    
    if (!m_gl) {
        std::cerr << "OpenGL functions not available" << std::endl;
        return;
    }
    
    unsigned int programId = m_gl->glCreateProgram();
    if (programId == 0) {
        std::cerr << "Failed to create OpenGL shader program: " << name << std::endl;
        return;
    }
    
    auto wrapper = std::make_unique<ProgramWrapper>(programId);
    wrapper->setOpenGLFunctions(m_gl);
    
    m_wrappers[name] = std::move(wrapper);
    m_programs[name] = programId;
    
    std::cout << "Created OpenGL shader program: " << name << " (ID: " << programId << ")" << std::endl;
}

void ShaderLib::attachShader(const std::string& name, int type) {
    std::cout << "ShaderLib: Attaching shader '" << name << "' of type " << type << std::endl;
    
    if (!m_gl) {
        std::cerr << "OpenGL functions not available" << std::endl;
        return;
    }
    
    unsigned int shaderId = m_gl->glCreateShader(glShaderType(type));
    if (shaderId == 0) {
        std::cerr << "Failed to create OpenGL shader: " << name << std::endl;
        return;
    }
    
    m_shaders[name] = shaderId;
    m_shaderTypes[name] = type;
    
    std::cout << "Created OpenGL shader: " << name << " (ID: " << shaderId << ", Type: " << type << ")" << std::endl;
}

void ShaderLib::loadShaderSource(const std::string& name, const std::string& filename) {
    std::cout << "ShaderLib: Loading shader source '" << name << "' from '" << filename << "'" << std::endl;
    
    std::string source;
    if (loadShaderFromFile(filename, source)) {
        m_shaderSources[name] = source;
        std::cout << "Successfully loaded shader source: " << filename << " (" << source.length() << " chars)" << std::endl;
    } else {
        std::cerr << "Failed to load shader source: " << filename << std::endl;
        m_shaderSources[name] = "// Failed to load shader source";
    }
}

void ShaderLib::compileShader(const std::string& name) {
    std::cout << "ShaderLib: Compiling shader '" << name << "'" << std::endl;
    
    if (!m_gl) {
        std::cerr << "OpenGL functions not available" << std::endl;
        return;
    }
    
    auto shaderIt = m_shaders.find(name);
    auto sourceIt = m_shaderSources.find(name);
    
    if (shaderIt != m_shaders.end() && sourceIt != m_shaderSources.end()) {
        unsigned int shaderId = shaderIt->second;
        const std::string& source = sourceIt->second;
        
        // Set shader source
        const char* sourcePtr = source.c_str();
        m_gl->glShaderSource(shaderId, 1, &sourcePtr, nullptr);
        
        // Compile shader
        m_gl->glCompileShader(shaderId);
        
        // Check compilation status
        if (checkShaderCompilation(shaderId, name)) {
            std::cout << "Successfully compiled shader: " << name << std::endl;
        }
    } else {
        std::cerr << "Shader not found for compilation: " << name << std::endl;
    }
}

void ShaderLib::attachShaderToProgram(const std::string& program, const std::string& shader) {
    std::cout << "ShaderLib: Attaching shader '" << shader << "' to program '" << program << "'" << std::endl;
    
    // Check if this shader is already attached to this program
    auto& attachedShaders = m_programShaderAttachments[program];
    if (attachedShaders.find(shader) != attachedShaders.end()) {
        std::cout << "Shader '" << shader << "' is already attached to program '" << program << "', skipping" << std::endl;
        return;
    }
    
    auto programIt = m_programs.find(program);
    auto shaderIt = m_shaders.find(shader);
    
    if (programIt != m_programs.end() && shaderIt != m_shaders.end()) {
        unsigned int programId = programIt->second;
        unsigned int shaderId = shaderIt->second;
        
        // Attach shader to program
        if (m_gl) {
            m_gl->glAttachShader(programId, shaderId);
            
            // Track this attachment
            attachedShaders.insert(shader);
            std::cout << "Successfully attached shader '" << shader << "' (ID: " << shaderId << ") to program '" << program << "' (ID: " << programId << ")" << std::endl;
        } else {
            std::cerr << "OpenGL functions not available for attaching shader" << std::endl;
        }
    } else {
        std::cerr << "Program or shader not found: " << program << ", " << shader << std::endl;
    }
}

void ShaderLib::bindAttribute(const std::string& program, int index, const std::string& name) {
    std::cout << "ShaderLib: Binding attribute '" << name << "' to index " << index << " in program '" << program << "'" << std::endl;
    
    auto it = m_programs.find(program);
    if (it != m_programs.end()) {
        unsigned int programId = it->second;
        if (m_gl) {
            m_gl->glBindAttribLocation(programId, index, name.c_str());
            std::cout << "Successfully bound attribute: " << name << " to index " << index << " in program " << programId << std::endl;
        } else {
            std::cerr << "OpenGL functions not available for binding attribute" << std::endl;
        }
    } else {
        std::cerr << "Program not found for attribute binding: " << program << std::endl;
    }
}

void ShaderLib::linkProgramObject(const std::string& name) {
    std::cout << "ShaderLib: Linking program '" << name << "'" << std::endl;
    
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        unsigned int programId = it->second;
        
        // Link the program
        if (m_gl) {
            m_gl->glLinkProgram(programId);
            
            // Check linking status
            if (checkProgramLinking(programId, name)) {
                std::cout << "Successfully linked program: " << name << " (ID: " << programId << ")" << std::endl;
            }
        } else {
            std::cerr << "OpenGL functions not available for linking" << std::endl;
        }
    } else {
        std::cerr << "Program not found for linking: " << name << std::endl;
    }
}

void ShaderLib::use(const std::string& name) {
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        unsigned int programId = it->second;
        if (m_gl) {
            m_gl->glUseProgram(programId);
            
            m_currentShader = name;
            m_currentWrapper = m_wrappers[name].get();
            std::cout << "ShaderLib: Using program '" << name << "' (ID: " << programId << ")" << std::endl;
        } else {
            std::cerr << "OpenGL functions not available for using program" << std::endl;
        }
    } else {
        std::cerr << "Program not found: " << name << std::endl;
    }
}

// ProgramWrapper implementation
ShaderLib::ProgramWrapper::ProgramWrapper(unsigned int programId) : m_programId(programId) {}

void ShaderLib::ProgramWrapper::use() {
    if (m_gl) {
        m_gl->glUseProgram(m_programId);
    }
}

int ShaderLib::ProgramWrapper::getUniformLocation(const std::string& name) {
    if (!m_gl) {
        std::cerr << "OpenGL functions not available for uniform location" << std::endl;
        return -1;
    }
    
    int location = m_gl->glGetUniformLocation(m_programId, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in program " << m_programId << std::endl;
    }
    return location;
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, const glm::mat4& value) {
    int location = getUniformLocation(name);
    if (location != -1 && m_gl) {
        m_gl->glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, const glm::mat3& value) {
    int location = getUniformLocation(name);
    if (location != -1 && m_gl) {
        m_gl->glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, const glm::vec4& value) {
    int location = getUniformLocation(name);
    if (location != -1 && m_gl) {
        m_gl->glUniform4f(location, value.x, value.y, value.z, value.w);
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, const glm::vec3& value) {
    int location = getUniformLocation(name);
    if (location != -1 && m_gl) {
        m_gl->glUniform3f(location, value.x, value.y, value.z);
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, float value) {
    int location = getUniformLocation(name);
    if (location != -1 && m_gl) {
        m_gl->glUniform1f(location, value);
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, int value) {
    int location = getUniformLocation(name);
    if (location != -1 && m_gl) {
        m_gl->glUniform1i(location, value);
    }
}

// Uniform setters
void ShaderLib::setShaderParam(const std::string& paramName, const glm::mat4& value) {
    if (m_currentWrapper) {
        m_currentWrapper->setUniform(paramName, value);
    }
}

void ShaderLib::setShaderParam(const std::string& paramName, const glm::mat3& value) {
    if (m_currentWrapper) {
        m_currentWrapper->setUniform(paramName, value);
    }
}

void ShaderLib::setShaderParam(const std::string& paramName, const glm::vec4& value) {
    if (m_currentWrapper) {
        m_currentWrapper->setUniform(paramName, value);
    }
}

void ShaderLib::setShaderParam(const std::string& paramName, const glm::vec3& value) {
    if (m_currentWrapper) {
        m_currentWrapper->setUniform(paramName, value);
    }
}

void ShaderLib::setShaderParam(const std::string& paramName, float value) {
    if (m_currentWrapper) {
        m_currentWrapper->setUniform(paramName, value);
    }
}

void ShaderLib::setShaderParam(const std::string& paramName, int value) {
    if (m_currentWrapper) {
        m_currentWrapper->setUniform(paramName, value);
    }
}

// Legacy compatibility methods
void ShaderLib::setShaderParam3f(const std::string& paramName, float x, float y, float z) {
    setShaderParam(paramName, glm::vec3(x, y, z));
}

void ShaderLib::setShaderParam4f(const std::string& paramName, float x, float y, float z, float w) {
    setShaderParam(paramName, glm::vec4(x, y, z, w));
}

void ShaderLib::setShaderParamFromMatrix(const std::string& paramName, const glm::mat4& matrix) {
    setShaderParam(paramName, matrix);
}

void ShaderLib::setShaderParamFromMat3x3(const std::string& paramName, const glm::mat3& matrix) {
    setShaderParam(paramName, matrix);
}

ShaderLib::ProgramWrapper* ShaderLib::operator[](const std::string& name) {
    auto it = m_wrappers.find(name);
    if (it != m_wrappers.end()) {
        return it->second.get();
    }
    return nullptr;
}

// Helper methods
unsigned int ShaderLib::glShaderType(int type) {
    switch(type) {
        case VERTEX: return 0x8B31;   // GL_VERTEX_SHADER
        case FRAGMENT: return 0x8B30; // GL_FRAGMENT_SHADER
        case GEOMETRY: return 0x8DD9; // GL_GEOMETRY_SHADER
        case COMPUTE: return 0x91B9;  // GL_COMPUTE_SHADER
        default: return 0x8B31;       // Default to vertex shader
    }
}

bool ShaderLib::checkShaderCompilation(unsigned int shaderId, const std::string& name) {
    if (!m_gl) {
        std::cerr << "OpenGL functions not available for checking shader compilation" << std::endl;
        return false;
    }
    
    int success;
    m_gl->glGetShaderiv(shaderId, 0x8B81, &success); // GL_COMPILE_STATUS
    
    if (!success) {
        char infoLog[512];
        m_gl->glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        std::cerr << "Failed to compile shader '" << name << "': " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool ShaderLib::checkProgramLinking(unsigned int programId, const std::string& name) {
    if (!m_gl) {
        std::cerr << "OpenGL functions not available for checking program linking" << std::endl;
        return false;
    }
    
    int success;
    m_gl->glGetProgramiv(programId, 0x8B82, &success); // GL_LINK_STATUS
    
    if (!success) {
        char infoLog[512];
        m_gl->glGetProgramInfoLog(programId, 512, nullptr, infoLog);
        std::cerr << "Failed to link program '" << name << "': " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool ShaderLib::loadShaderFromFile(const std::string& filename, std::string& source) {
    // Try multiple possible paths for shader files
    std::vector<std::string> possiblePaths = {
        filename,                    // Direct path
        "../../" + filename,         // From bin directory
        "../" + filename,            // From release directory
        "../../shaders/" + filename, // Fallback direct to shaders
    };
    
    for (const auto& path : possiblePaths) {
        QFile file(QString::fromStdString(path));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            source = in.readAll().toStdString();
            file.close();
            
            if (!source.empty()) {
                std::cout << "Successfully loaded shader from: " << path << std::endl;
                return true;
            }
        }
    }
    
    std::cerr << "Cannot open shader file: " << filename << " (tried multiple paths)" << std::endl;
    return false;
}
