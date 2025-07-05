#include "../include/SmartShaderManager.h"
#include "../include/ShaderLib.h"
#include <iostream>
#include <algorithm>
#include "../../../include/glew_compat.h"

SmartShaderManager* SmartShaderManager::s_instance = nullptr;

SmartShaderManager* SmartShaderManager::instance() {
    if (!s_instance) {
        s_instance = new SmartShaderManager();
    }
    return s_instance;
}

void SmartShaderManager::useShader(const std::string& shaderName) {
    if (m_currentShader == shaderName) {
        m_stats.redundantSwitches++;
        return;
    }
    
    ShaderLib* shaderLib = ShaderLib::instance();
    ShaderLib::ProgramWrapper* program = (*shaderLib)[shaderName];
    if (program) {
        program->use();
        m_currentShader = shaderName;
        m_stats.shaderSwitches++;
    }
}

void SmartShaderManager::addRenderCommand(const RenderCommand& command) {
    m_renderCommands.push_back(command);
}

void SmartShaderManager::executeRenderCommands() {
    if (m_renderCommands.empty()) return;
    
    // Sort by shader to minimize switches
    sortRenderCommandsByShader();
    
    // Execute commands
    for (const auto& command : m_renderCommands) {
        useShader(command.shaderName);
        command.renderFunction();
    }
}

void SmartShaderManager::clearRenderCommands() {
    m_renderCommands.clear();
}

void SmartShaderManager::setUniformIfChanged(const std::string& name, const glm::mat4& value) {
    auto& cache = m_uniformCaches[m_currentShader];
    auto it = cache.mat4Values.find(name);
    
    if (it == cache.mat4Values.end() || it->second != value) {
        // Value changed or not cached, update it
        ShaderLib* shaderLib = ShaderLib::instance();
        ShaderLib::ProgramWrapper* program = (*shaderLib)[m_currentShader];
        if (program) {
            program->setUniform(name, value);
            cache.mat4Values[name] = value;
            m_stats.uniformUpdates++;
        }
    } else {
        m_stats.uniformsSkipped++;
    }
}

void SmartShaderManager::setUniformIfChanged(const std::string& name, const glm::vec4& value) {
    auto& cache = m_uniformCaches[m_currentShader];
    auto it = cache.vec4Values.find(name);
    
    if (it == cache.vec4Values.end() || it->second != value) {
        // Value changed or not cached, update it
        ShaderLib* shaderLib = ShaderLib::instance();
        ShaderLib::ProgramWrapper* program = (*shaderLib)[m_currentShader];
        if (program) {
            program->setUniform(name, value);
            cache.vec4Values[name] = value;
            m_stats.uniformUpdates++;
        }
    } else {
        m_stats.uniformsSkipped++;
    }
}

void SmartShaderManager::setUniformIfChanged(const std::string& name, float value) {
    auto& cache = m_uniformCaches[m_currentShader];
    auto it = cache.floatValues.find(name);
    
    if (it == cache.floatValues.end() || it->second != value) {
        // Value changed or not cached, update it
        ShaderLib* shaderLib = ShaderLib::instance();
        ShaderLib::ProgramWrapper* program = (*shaderLib)[m_currentShader];
        if (program) {
            program->setUniform(name, value);
            cache.floatValues[name] = value;
            m_stats.uniformUpdates++;
        }
    } else {
        m_stats.uniformsSkipped++;
    }
}

void SmartShaderManager::pushShaderState() {
    m_shaderStack.push_back(m_currentShader);
}

void SmartShaderManager::popShaderState() {
    if (!m_shaderStack.empty()) {
        std::string previousShader = m_shaderStack.back();
        m_shaderStack.pop_back();
        useShader(previousShader);
    }
}

void SmartShaderManager::sortRenderCommandsByShader() {
    std::sort(m_renderCommands.begin(), m_renderCommands.end(), 
        [](const RenderCommand& a, const RenderCommand& b) {
            // First sort by priority, then by shader name
            if (a.priority != b.priority) {
                return a.priority > b.priority; // Higher priority first
            }
            return a.shaderName < b.shaderName; // Group by shader
        });
}
