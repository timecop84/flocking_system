#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include "ShaderLib.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief Optimized shader manager with state caching and batching
/// @details Minimizes shader switches and redundant state changes
//----------------------------------------------------------------------------------------------------------------------
class SmartShaderManager {
public:
    static SmartShaderManager* instance();
    
    // Efficient shader usage with state caching
    void useShader(const std::string& shaderName);
    
    // Batch rendering by shader to minimize switches
    struct RenderCommand {
        std::string shaderName;
        std::function<void()> renderFunction;
        int priority = 0; // For sorting render order
    };
    
    void addRenderCommand(const RenderCommand& command);
    void executeRenderCommands(); // Batches by shader automatically
    void clearRenderCommands();
    
    // Uniform caching to avoid redundant uploads
    void setUniformIfChanged(const std::string& name, const glm::mat4& value);
    void setUniformIfChanged(const std::string& name, const glm::vec4& value);
    void setUniformIfChanged(const std::string& name, float value);
    
    // Shader state management
    void pushShaderState();
    void popShaderState();
    
    // Performance tracking
    struct ShaderStats {
        size_t shaderSwitches = 0;
        size_t redundantSwitches = 0;
        size_t uniformUpdates = 0;
        size_t uniformsSkipped = 0;
    };
    
    const ShaderStats& getStats() const { return m_stats; }
    void clearStats() { m_stats = {}; }
    
private:
    SmartShaderManager() = default;
    static SmartShaderManager* s_instance;
    
    std::string m_currentShader;
    std::vector<RenderCommand> m_renderCommands;
    std::vector<std::string> m_shaderStack; // For push/pop functionality
    
    // Uniform caching
    struct UniformCache {
        std::unordered_map<std::string, glm::mat4> mat4Values;
        std::unordered_map<std::string, glm::vec4> vec4Values;
        std::unordered_map<std::string, float> floatValues;
    };
    
    std::unordered_map<std::string, UniformCache> m_uniformCaches; // Per shader
    
    ShaderStats m_stats;
    
    void sortRenderCommandsByShader();
};
