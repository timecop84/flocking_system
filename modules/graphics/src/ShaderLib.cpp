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
#include "../../../include/glew_compat.h" // For OpenGL 3.0+ functions

// Define global function pointers for UBO functions
PFNGLBINDBUFFERBASEPROC glBindBufferBase = nullptr;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = nullptr;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = nullptr;

// Define global function pointers for VAO functions
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;

// Define global function pointers for VBO functions
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;

// Define global function pointers for vertex attribute functions
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;

// Define global function pointers for instanced rendering functions
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = nullptr;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = nullptr;

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
            } else {
                // Initialize UBO function pointers
                glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)context->getProcAddress("glBindBufferBase");
                glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)context->getProcAddress("glGetUniformBlockIndex");
                glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)context->getProcAddress("glUniformBlockBinding");
                
                // Initialize VBO function pointers  
                glBufferSubData = (PFNGLBUFFERSUBDATAPROC)context->getProcAddress("glBufferSubData");
                
                // Initialize instanced rendering function pointers
                glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)context->getProcAddress("glDrawElementsInstanced");
                glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)context->getProcAddress("glVertexAttribDivisor");
                
                if (!glBindBufferBase || !glGetUniformBlockIndex || !glUniformBlockBinding) {
                    std::cerr << "Failed to initialize UBO function pointers" << std::endl;
                } else if (!glDrawElementsInstanced || !glVertexAttribDivisor) {
                    std::cerr << "Failed to initialize instanced rendering function pointers" << std::endl;
                } else if (!glBufferSubData) {
                    std::cerr << "Failed to initialize VBO function pointers" << std::endl;
                }
            }
        } else {
            std::cerr << "No current OpenGL context available" << std::endl;
        }
    }
    return s_instance;
}

void ShaderLib::createShader(const std::string& name) {
    // Individual shaders are created when attachShader is called
}

void ShaderLib::createShaderProgram(const std::string& name) {
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
}

void ShaderLib::attachShader(const std::string& name, int type) {
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
        checkShaderCompilation(shaderId, name);
    } else {
        std::cerr << "Shader not found for compilation: " << name << std::endl;
    }
}

void ShaderLib::attachShaderToProgram(const std::string& program, const std::string& shader) {
    // Check if this shader is already attached to this program
    auto& attachedShaders = m_programShaderAttachments[program];
    if (attachedShaders.find(shader) != attachedShaders.end()) {
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
        } else {
            std::cerr << "OpenGL functions not available for attaching shader" << std::endl;
        }
    } else {
        std::cerr << "Program or shader not found: " << program << ", " << shader << std::endl;
    }
}

void ShaderLib::bindAttribute(const std::string& program, int index, const std::string& name) {
    auto it = m_programs.find(program);
    if (it != m_programs.end()) {
        unsigned int programId = it->second;
        if (m_gl) {
            m_gl->glBindAttribLocation(programId, index, name.c_str());
        } else {
            std::cerr << "OpenGL functions not available for binding attribute" << std::endl;
        }
    } else {
        std::cerr << "Program not found for attribute binding: " << program << std::endl;
    }
}

void ShaderLib::linkProgramObject(const std::string& name) {
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        unsigned int programId = it->second;
        
        // Link the program
        if (m_gl) {
            m_gl->glLinkProgram(programId);
            
            // Check linking status
            checkProgramLinking(programId, name);
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
            // Debug output disabled for performance - was causing significant slowdown
            // std::cout << "ShaderLib: Using program '" << name << "' (ID: " << programId << ")" << std::endl;
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
        int logLength = 0;
        m_gl->glGetProgramiv(programId, 0x8B84, &logLength); // GL_INFO_LOG_LENGTH
        
        if (logLength > 0) {
            std::vector<char> infoLog(logLength);
            m_gl->glGetProgramInfoLog(programId, logLength, nullptr, infoLog.data());
            std::cerr << "Failed to link program '" << name << "': " << std::string(infoLog.data()) << std::endl;
        } else {
            std::cerr << "Failed to link program '" << name << "': Unknown error (no log available)" << std::endl;
        }
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
                return true;
            }
        }
    }
    
    std::cerr << "Cannot open shader file: " << filename << " (tried multiple paths)" << std::endl;
    return false;
}

// UBO (Uniform Buffer Object) implementation
unsigned int ShaderLib::createUBO(const std::string& name, size_t size) {
    if (!m_gl) {
        std::cerr << "OpenGL functions not available for UBO creation" << std::endl;
        return 0;
    }
    
    unsigned int uboId;
    m_gl->glGenBuffers(1, &uboId);
    m_gl->glBindBuffer(GL_UNIFORM_BUFFER, uboId);
    m_gl->glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    m_gl->glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    m_ubos[name] = uboId;
    
    return uboId;
}

void ShaderLib::bindUBOToBindingPoint(const std::string& uboName, unsigned int bindingPoint) {
    if (!m_gl) {
        std::cerr << "OpenGL functions not available for UBO binding" << std::endl;
        return;
    }
    
    auto it = m_ubos.find(uboName);
    if (it == m_ubos.end()) {
        std::cerr << "UBO '" << uboName << "' not found" << std::endl;
        return;
    }
    
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, it->second);
}

void ShaderLib::bindUniformBlockToBindingPoint(const std::string& programName, const std::string& blockName, unsigned int bindingPoint) {
    if (!m_gl) {
        std::cerr << "OpenGL functions not available for uniform block binding" << std::endl;
        return;
    }
    
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
    if (!m_gl) {
        std::cerr << "OpenGL functions not available for UBO update" << std::endl;
        return;
    }
    
    auto it = m_ubos.find(uboName);
    if (it == m_ubos.end()) {
        std::cerr << "UBO '" << uboName << "' not found" << std::endl;
        return;
    }
    
    m_gl->glBindBuffer(GL_UNIFORM_BUFFER, it->second);
    m_gl->glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    m_gl->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderLib::deleteUBO(const std::string& uboName) {
    if (!m_gl) {
        std::cerr << "OpenGL functions not available for UBO deletion" << std::endl;
        return;
    }
    
    auto it = m_ubos.find(uboName);
    if (it == m_ubos.end()) {
        std::cerr << "UBO '" << uboName << "' not found" << std::endl;
        return;
    }
    
    m_gl->glDeleteBuffers(1, &it->second);
    m_ubos.erase(it);
}
