#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <set>

class QOpenGLFunctions;

/**
 * @brief Modern OpenGL Shader Library using raw OpenGL calls
 * @details Replaces Qt's wrapper with direct OpenGL shader management
 * for better compatibility and control over shader compilation/linking.
 */
class ShaderLib {
public:
    enum ShaderType {
        VERTEX = 0x8B31,    // GL_VERTEX_SHADER
        FRAGMENT = 0x8B30,  // GL_FRAGMENT_SHADER
        GEOMETRY = 0x8DD9,  // GL_GEOMETRY_SHADER
        COMPUTE = 0x91B9    // GL_COMPUTE_SHADER
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
        
        // Set OpenGL functions reference
        void setOpenGLFunctions(QOpenGLFunctions* gl) { m_gl = gl; }
        
    private:
        unsigned int m_programId;
        QOpenGLFunctions* m_gl = nullptr;
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
    
    // OpenGL functions
    QOpenGLFunctions* m_gl = nullptr;
    
    // Storage for shaders and programs using raw OpenGL IDs
    std::unordered_map<std::string, unsigned int> m_shaders;        // Shader object IDs
    std::unordered_map<std::string, unsigned int> m_programs;       // Program object IDs
    std::unordered_map<std::string, int> m_shaderTypes;             // Shader types
    std::unordered_map<std::string, std::unique_ptr<ProgramWrapper>> m_wrappers;
    std::unordered_map<std::string, std::string> m_shaderSources;
    
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
