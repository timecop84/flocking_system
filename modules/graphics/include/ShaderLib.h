// ShaderLib.h - Modernized, documented, and cleaned up for maintainability
#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <set>
#include "FlockTypes.h"
/**
 * @file ShaderLib.h
 * @brief Modern OpenGL Shader Library using raw OpenGL calls.
 * Uses direct OpenGL shader management with no external GUI framework dependencies.
 * Uses FlockTypes.h for type aliases and helpers.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
class ShaderLib {
public:
    enum ShaderType {
        VERTEX = 0x8B31,
        FRAGMENT = 0x8B30,
        GEOMETRY = 0x8DD9,
        COMPUTE = 0x91B9
    };
    
    /**
     * @brief Get the singleton instance of ShaderLib
     * @return Pointer to the ShaderLib instance
     */
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
    
    /**
     * @brief Wrapper class for shader programs to provide operator[] access
     */
    class ProgramWrapper {
    public:
        explicit ProgramWrapper(unsigned int programId);
        void use();
        
        // Uniform setters
        void setUniform(const std::string& name, const glm::mat4& value);
        void setUniform(const std::string& name, const glm::mat3& value);
        void setUniform(const std::string& name, const glm::vec4& value);
        void setUniform(const std::string& name, const glm::vec3& value);
        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, int value);
        
        unsigned int getProgramId() const { return m_programId; }
        
    private:
        unsigned int m_programId;
        int getUniformLocation(const std::string& name);
    };
    
    /**
     * @brief Access a shader program by name
     * @param name The name of the shader program
     * @return Pointer to ProgramWrapper for the program
     */
    ProgramWrapper* operator[](const std::string& name);
    
    // Legacy uniform setters for backward compatibility
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
    
    // UBO (Uniform Buffer Object) support
    unsigned int createUBO(const std::string& name, size_t size);
    void bindUBOToBindingPoint(const std::string& uboName, unsigned int bindingPoint);
    void bindUniformBlockToBindingPoint(const std::string& programName, const std::string& blockName, unsigned int bindingPoint);
    void updateUBO(const std::string& uboName, const void* data, size_t size, size_t offset = 0);
    void deleteUBO(const std::string& uboName);
    
    /**
     * @brief Get the currently active shader program name
     * @return Name of the current shader program
     */
    std::string getCurrentShader() const { return m_currentShader; }
    
private:
    ShaderLib() = default;
    ~ShaderLib() = default;
    
    // Singleton instance
    static ShaderLib* s_instance;
    
    // Storage for shaders and programs using raw OpenGL IDs
    std::unordered_map<std::string, unsigned int> m_shaders;        // Shader object IDs
    std::unordered_map<std::string, unsigned int> m_programs;       // Program object IDs
    std::unordered_map<std::string, int> m_shaderTypes;             // Shader types
    std::unordered_map<std::string, std::unique_ptr<ProgramWrapper>> m_wrappers;
    std::unordered_map<std::string, std::string> m_shaderSources;
    
    // UBO storage
    std::unordered_map<std::string, unsigned int> m_ubos;           // UBO object IDs
    
    // Track which shaders are attached to which programs to prevent duplicates
    std::unordered_map<std::string, std::set<std::string>> m_programShaderAttachments;
    
    // Current state
    std::string m_currentShader;
    ProgramWrapper* m_currentWrapper = nullptr;
    
    // Helper methods
    unsigned int glShaderType(int type);
    bool checkShaderCompilation(unsigned int shaderId, const std::string& name);
    bool checkProgramLinking(unsigned int programId, const std::string& name);
    bool loadShaderFromFile(const std::string& filename, std::string& source);
};
