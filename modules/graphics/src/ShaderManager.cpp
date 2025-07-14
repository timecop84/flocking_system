
/**
 * @file ShaderManager.cpp
 * @brief Implementation of a smart shader management system for the renderer.
 *
 * Handles efficient shader switching, uniform caching, and render command batching to minimize GPU state changes.
 * This system is designed to optimize rendering performance by reducing redundant shader switches and uniform uploads.
 *
 * @author Dennis Toufexis
 * @date 2025
 */

#include <glad/gl.h>
#include "../include/ShaderManager.h"
#include "../include/ShaderLib.h"
#include <iostream>
#include <algorithm>


// Static singleton instance pointer
ShaderManager* ShaderManager::s_instance = nullptr;


/**
 * @brief Get the singleton instance of the ShaderManager.
 *
 * Ensures only one manager exists for the lifetime of the application.
 * @return Pointer to the ShaderManager instance.
 */
ShaderManager* ShaderManager::instance() {
    if (!s_instance) {
        s_instance = new ShaderManager();
    }
    return s_instance;
}


/**
 * @brief Switch to the specified shader program if not already active.
 *
 * Tracks redundant switches and updates internal statistics.
 * @param shaderName The name of the shader program to use.
 */
void ShaderManager::useShader(const std::string& shaderName) {
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


/**
 * @brief Add a render command to the batch.
 *
 * Render commands are sorted and executed to minimize shader switches.
 * @param command The render command to add.
 */
void ShaderManager::addRenderCommand(const RenderCommand& command) {
    m_renderCommands.push_back(command);
}


/**
 * @brief Execute all batched render commands, optimizing shader usage.
 *
 * Sorts commands by shader and executes them, minimizing GPU state changes.
 */
void ShaderManager::executeRenderCommands() {
    if (m_renderCommands.empty()) return;
    // Sort by shader to minimize switches
    sortRenderCommandsByShader();
    // Execute commands
    for (const auto& command : m_renderCommands) {
        useShader(command.shaderName);
        command.renderFunction();
    }
}


/**
 * @brief Clear all batched render commands.
 *
 * Prepares the manager for the next frame.
 */
void ShaderManager::clearRenderCommands() {
    m_renderCommands.clear();
}

void ShaderManager::setUniformIfChanged(const std::string& name, const glm::mat4& value) {
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

void ShaderManager::setUniformIfChanged(const std::string& name, const glm::vec4& value) {
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

void ShaderManager::setUniformIfChanged(const std::string& name, float value) {
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

void ShaderManager::pushShaderState() {
    m_shaderStack.push_back(m_currentShader);
}

void ShaderManager::popShaderState() {
    if (!m_shaderStack.empty()) {
        std::string previousShader = m_shaderStack.back();
        m_shaderStack.pop_back();
        useShader(previousShader);
    }
}

void ShaderManager::sortRenderCommandsByShader() {
    std::sort(m_renderCommands.begin(), m_renderCommands.end(), 
        [](const RenderCommand& a, const RenderCommand& b) {
            // First sort by priority, then by shader name
            if (a.priority != b.priority) {
                return a.priority > b.priority; // Higher priority first
            }
            return a.shaderName < b.shaderName; // Group by shader
        });
}
