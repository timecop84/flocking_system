
/**
 * @file ShaderLib.cpp
 * @brief Implementation of a centralized shader management library for the renderer.
 *
 * Handles creation, compilation, linking, and management of OpenGL shader programs using GLAD.
 * Provides a singleton interface for easy access throughout the application.
 */

#include <glad/gl.h>
#include "ShaderLib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

// Static singleton instance pointer
ShaderLib* ShaderLib::s_instance = nullptr;

ShaderLib* ShaderLib::instance() {
    if (!s_instance) {
        s_instance = new ShaderLib();
    }
    return s_instance;
}

void ShaderLib::createShader(const std::string& /*name*/) {
    // Individual shaders are created when attachShader is called
}

void ShaderLib::createShaderProgram(const std::string& name) {
    unsigned int programId = glCreateProgram();
    if (programId == 0) {
        std::cerr << "Failed to create OpenGL shader program: " << name << std::endl;
        return;
    }
    
    auto wrapper = std::make_unique<ProgramWrapper>(programId);
    
    m_wrappers[name] = std::move(wrapper);
    m_programs[name] = programId;
}

void ShaderLib::attachShader(const std::string& name, int type) {
    unsigned int shaderId = glCreateShader(glShaderType(type));
    if (shaderId == 0) {
        std::cerr << "Failed to create OpenGL shader: " << name << std::endl;
        return;
    }
    
    m_shaders[name] = shaderId;
    m_shaderTypes[name] = type;
}

void ShaderLib::loadShaderSource(const std::string& name, const std::string& filename) {
    std::string source;
    if (loadShaderFromFile(filename, source)) {
        m_shaderSources[name] = source;
    } else {
        std::cerr << "Failed to load shader source: " << filename << std::endl;
        m_shaderSources[name] = "// Failed to load shader source";
    }
}

void ShaderLib::compileShader(const std::string& name) {
    auto shaderIt = m_shaders.find(name);
    auto sourceIt = m_shaderSources.find(name);
    
    if (shaderIt != m_shaders.end() && sourceIt != m_shaderSources.end()) {
        unsigned int shaderId = shaderIt->second;
        const std::string& source = sourceIt->second;
        
        const char* sourcePtr = source.c_str();
        glShaderSource(shaderId, 1, &sourcePtr, nullptr);
        glCompileShader(shaderId);
        
        checkShaderCompilation(shaderId, name);
    } else {
        std::cerr << "Shader not found for compilation: " << name << std::endl;
    }
}

void ShaderLib::attachShaderToProgram(const std::string& program, const std::string& shader) {
    auto& attachedShaders = m_programShaderAttachments[program];
    if (attachedShaders.find(shader) != attachedShaders.end()) {
        return;
    }
    
    auto programIt = m_programs.find(program);
    auto shaderIt = m_shaders.find(shader);
    
    if (programIt != m_programs.end() && shaderIt != m_shaders.end()) {
        unsigned int programId = programIt->second;
        unsigned int shaderId = shaderIt->second;
        
        glAttachShader(programId, shaderId);
        attachedShaders.insert(shader);
    } else {
        std::cerr << "Program or shader not found: " << program << ", " << shader << std::endl;
    }
}

void ShaderLib::bindAttribute(const std::string& program, int index, const std::string& name) {
    auto it = m_programs.find(program);
    if (it != m_programs.end()) {
        unsigned int programId = it->second;
        glBindAttribLocation(programId, index, name.c_str());
    } else {
        std::cerr << "Program not found for attribute binding: " << program << std::endl;
    }
}

void ShaderLib::linkProgramObject(const std::string& name) {
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        unsigned int programId = it->second;
        glLinkProgram(programId);
        checkProgramLinking(programId, name);
    } else {
        std::cerr << "Program not found for linking: " << name << std::endl;
    }
}

void ShaderLib::use(const std::string& name) {
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        unsigned int programId = it->second;
        glUseProgram(programId);
        
        m_currentShader = name;
        m_currentWrapper = m_wrappers[name].get();
    } else {
        std::cerr << "Program not found: " << name << std::endl;
    }
}

// ProgramWrapper implementation
ShaderLib::ProgramWrapper::ProgramWrapper(unsigned int programId) : m_programId(programId) {}

void ShaderLib::ProgramWrapper::use() {
    glUseProgram(m_programId);
}

int ShaderLib::ProgramWrapper::getUniformLocation(const std::string& name) {
    int location = glGetUniformLocation(m_programId, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' not found in program " << m_programId << std::endl;
    }
    return location;
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, const glm::mat4& value) {
    int location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, const glm::mat3& value) {
    int location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, const glm::vec4& value) {
    int location = getUniformLocation(name);
    if (location != -1) {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, const glm::vec3& value) {
    int location = getUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, float value) {
    int location = getUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void ShaderLib::ProgramWrapper::setUniform(const std::string& name, int value) {
    int location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
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
        case 0x8B31: return 0x8B31;   // GL_VERTEX_SHADER (ShaderLib enum)
        case 0x8B30: return 0x8B30;   // GL_FRAGMENT_SHADER (ShaderLib enum)
        case 0x8DD9: return 0x8DD9;   // GL_GEOMETRY_SHADER (ShaderLib enum)
        case 0x91B9: return 0x91B9;   // GL_COMPUTE_SHADER (ShaderLib enum)
        
        // Legacy shader_constants.h values
        case 0: return 0x8B31;        // VERTEX = 0 -> GL_VERTEX_SHADER
        case 1: return 0x8B30;        // FRAGMENT = 1 -> GL_FRAGMENT_SHADER
        case 2: return 0x8DD9;        // GEOMETRY = 2 -> GL_GEOMETRY_SHADER
        
        default: return 0x8B31;       // Default to vertex shader
    }
}

bool ShaderLib::checkShaderCompilation(unsigned int shaderId, const std::string& name) {
    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        std::cerr << "Failed to compile shader '" << name << "': " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool ShaderLib::checkProgramLinking(unsigned int programId, const std::string& name) {
    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    
    if (!success) {
        int logLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        
        if (logLength > 0) {
            std::vector<char> infoLog(static_cast<size_t>(logLength));
            glGetProgramInfoLog(programId, logLength, nullptr, infoLog.data());
            std::cerr << "Failed to link program '" << name << "': " << std::string(infoLog.data()) << std::endl;
        } else {
            std::cerr << "Failed to link program '" << name << "': Unknown error (no log available)" << std::endl;
        }
        return false;
    }
    return true;
}

bool ShaderLib::loadShaderFromFile(const std::string& filename, std::string& source) {
    std::vector<std::string> possiblePaths = {
        filename,
        "../../" + filename,
        "../" + filename,
        "../../shaders/" + filename,
    };
    
    for (const auto& path : possiblePaths) {
        std::ifstream file(path);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            source = buffer.str();
            if (!source.empty()) {
                return true;
            }
        }
    }
    
    std::cerr << "Cannot open shader file: " << filename << " (tried multiple paths)" << std::endl;
    return false;
}

// UBO (Uniform Buffer Object) implementation
unsigned int ShaderLib::createUBO(const std::string& name, size_t size) {
    unsigned int uboId;
    glGenBuffers(1, &uboId);
    glBindBuffer(GL_UNIFORM_BUFFER, uboId);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    m_ubos[name] = uboId;
    
    return uboId;
}

void ShaderLib::bindUBOToBindingPoint(const std::string& uboName, unsigned int bindingPoint) {
    auto it = m_ubos.find(uboName);
    if (it == m_ubos.end()) {
        std::cerr << "UBO '" << uboName << "' not found" << std::endl;
        return;
    }
    
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, it->second);
}

void ShaderLib::bindUniformBlockToBindingPoint(const std::string& programName, const std::string& blockName, unsigned int bindingPoint) {
    auto it = m_programs.find(programName);
    if (it == m_programs.end()) {
        std::cerr << "Program '" << programName << "' not found" << std::endl;
        return;
    }
    
    unsigned int blockIndex = glGetUniformBlockIndex(it->second, blockName.c_str());
    if (blockIndex == GL_INVALID_INDEX) {
        std::cerr << "Uniform block '" << blockName << "' not found in program '" << programName << "'" << std::endl;
        return;
    }
    
    glUniformBlockBinding(it->second, blockIndex, bindingPoint);
}

void ShaderLib::updateUBO(const std::string& uboName, const void* data, size_t size, size_t offset) {
    auto it = m_ubos.find(uboName);
    if (it == m_ubos.end()) {
        std::cerr << "UBO '" << uboName << "' not found" << std::endl;
        return;
    }
    
    glBindBuffer(GL_UNIFORM_BUFFER, it->second);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderLib::deleteUBO(const std::string& uboName) {
    auto it = m_ubos.find(uboName);
    if (it == m_ubos.end()) {
        std::cerr << "UBO '" << uboName << "' not found" << std::endl;
        return;
    }
    
    glDeleteBuffers(1, &it->second);
    m_ubos.erase(it);
}
