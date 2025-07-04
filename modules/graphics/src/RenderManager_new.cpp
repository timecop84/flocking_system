#include "../include/RenderManager.h"
#include "../include/ShaderLib.h"
#include "../include/UBOStructures.h"
#include <iostream>
#include <algorithm>
#include <GL/glew.h>

namespace FlockingGraphics {

RenderManager::~RenderManager() {
    cleanup();
}

RenderManager& RenderManager::getInstance() {
    static RenderManager instance;
    return instance;
}

void RenderManager::initialize() {
    // Initialize rendering systems
    glEnable(GL_DEPTH_TEST);
    reset();
}

void RenderManager::destroy() {
    cleanup();
}

void RenderManager::beginFrame(Camera* camera, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) {
    m_currentCamera = camera;
    m_viewMatrix = viewMatrix;
    m_projMatrix = projMatrix;
    
    // Clear the render queue
    m_renderQueue.clear();
    
    // Reset stats
    if (m_statsEnabled) {
        m_stats.reset();
        m_frameStartTime = std::chrono::high_resolution_clock::now();
    }
    
    // Update global UBOs
    updateGlobalUBOs();
}

void RenderManager::endFrame() {
    // Render all submitted objects
    flushBatches();
    
    // Print performance stats if enabled
    if (m_statsEnabled) {
        printFrameStats();
    }
}

void RenderManager::submitRenderable(const RenderableObject& renderable) {
    m_renderQueue.push_back(renderable);
    
    if (m_statsEnabled) {
        m_stats.totalRenderables++;
        m_stats.renderablesByType[renderable.type]++;
    }
}

void RenderManager::flushBatches() {
    if (m_renderQueue.empty()) return;
    
    // Sort renderables for optimal rendering
    sortRenderables();
    
    // Render batches
    renderBatches();
    
    // Clear the queue
    m_renderQueue.clear();
}

void RenderManager::sortRenderables() {
    // Sort by shader, then by material, then by depth
    std::sort(m_renderQueue.begin(), m_renderQueue.end(), 
        [](const RenderableObject& a, const RenderableObject& b) {
            // Sort by geometry type first (batching optimization)
            if (a.type != b.type) {
                return a.type < b.type;
            }
            
            // Then by depth (back to front for transparency)
            return a.depth > b.depth;
        });
}

void RenderManager::renderBatches() {
    if (m_renderQueue.empty()) return;
    
    GeometryType currentType = m_renderQueue[0].type;
    
    for (const auto& renderable : m_renderQueue) {
        if (renderable.type != currentType) {
            currentType = renderable.type;
            if (m_statsEnabled) {
                m_stats.shaderSwitches++;
            }
        }
        
        renderObject(renderable);
    }
}

void RenderManager::renderObject(const RenderableObject& renderable) {
    // Update transform UBO
    updateTransformUBO(renderable.transform);
    
    // Update material UBO
    updateMaterialUBO(renderable.material);
    
    // Set wireframe mode
    if (renderable.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    // Render based on type
    switch (renderable.type) {
        case GeometryType::SPHERE:
            renderSphere(renderable);
            break;
        case GeometryType::BBOX:
            renderBBox(renderable);
            break;
        case GeometryType::CUSTOM:
            renderCustom(renderable);
            break;
    }
    
    if (m_statsEnabled) {
        m_stats.drawCalls++;
    }
}

void RenderManager::renderSphere(const RenderableObject& renderable) {
    // Use geometry factory or direct rendering
    if (renderable.geometryRender) {
        renderable.geometryRender();
    }
}

void RenderManager::renderBBox(const RenderableObject& renderable) {
    // Use geometry factory or direct rendering
    if (renderable.geometryRender) {
        renderable.geometryRender();
    }
}

void RenderManager::renderCustom(const RenderableObject& renderable) {
    // Use geometry factory or direct rendering
    if (renderable.geometryRender) {
        renderable.geometryRender();
    }
}

void RenderManager::renderObjects(const std::vector<RenderableObject>& objects, 
                                 const std::string& shaderName,
                                 TransformStack& transformStack, 
                                 Camera* camera) {
    // Legacy support method
    useShader(shaderName);
    
    for (const auto& obj : objects) {
        renderObject(obj, shaderName, transformStack, camera);
    }
}

void RenderManager::renderObject(const RenderableObject& object,
                                const std::string& shaderName,
                                TransformStack& transformStack, 
                                Camera* camera) {
    // Legacy support method
    useShader(shaderName);
    updateMatrixUBO(transformStack, camera);
    updateMaterialUBO(object.material);
    
    if (object.geometryRender) {
        object.geometryRender();
    }
}

void RenderManager::useShader(const std::string& shaderName) {
    if (m_currentShader != shaderName) {
        ShaderLib* shaderLib = ShaderLib::instance();
        ShaderLib::ProgramWrapper* program = (*shaderLib)[shaderName];
        if (program) {
            program->use();
            m_currentShader = shaderName;
            
            if (m_statsEnabled) {
                m_stats.shaderSwitches++;
            }
        }
    }
}

void RenderManager::updateGlobalUBOs() {
    updateCameraUBO();
    updateLightUBO();
}

void RenderManager::updateCameraUBO() {
    if (!m_currentCamera) return;
    
    // Update camera UBO data
    m_cachedCamera.viewMatrix = m_viewMatrix;
    m_cachedCamera.projMatrix = m_projMatrix;
    m_cachedCamera.viewProjMatrix = m_projMatrix * m_viewMatrix;
    m_cachedCamera.cameraPosition = glm::vec4(m_currentCamera->getEye(), 1.0f);
    
    // TODO: Update actual UBO when UBOCache is available
    // UBOCache::getInstance().updateCameraUBO(m_cachedCamera);
}

void RenderManager::updateTransformUBO(const glm::mat4& modelMatrix) {
    // Update transform UBO data
    m_cachedMatrices.modelMatrix = modelMatrix;
    m_cachedMatrices.viewMatrix = m_viewMatrix;
    m_cachedMatrices.projMatrix = m_projMatrix;
    m_cachedMatrices.mvpMatrix = m_projMatrix * m_viewMatrix * modelMatrix;
    m_cachedMatrices.normalMatrix = glm::transpose(glm::inverse(modelMatrix));
    
    // TODO: Update actual UBO when UBOCache is available
    // UBOCache::getInstance().updateMatrixUBO(m_cachedMatrices);
}

void RenderManager::updateMatrixUBO(const TransformStack& transformStack, Camera* camera) {
    // Legacy support method
    glm::mat4 modelMatrix = transformStack.getCurrentTransform();
    updateTransformUBO(modelMatrix);
}

void RenderManager::updateMaterialUBO(const Material& material) {
    // Update material UBO data
    Colour ambient = material.getAmbient();
    Colour diffuse = material.getDiffuse();
    Colour specular = material.getSpecular();
    
    m_cachedMaterial.ambient = glm::vec4(ambient.m_r, ambient.m_g, ambient.m_b, ambient.m_a);
    m_cachedMaterial.diffuse = glm::vec4(diffuse.m_r, diffuse.m_g, diffuse.m_b, diffuse.m_a);
    m_cachedMaterial.specular = glm::vec4(specular.m_r, specular.m_g, specular.m_b, specular.m_a);
    m_cachedMaterial.shininess = material.getShininess();
    
    // TODO: Update actual UBO when UBOCache is available
    // UBOCache::getInstance().updateMaterialUBO(m_cachedMaterial);
}

void RenderManager::updateLightUBO() {
    // TODO: Implement light UBO updates
}

void RenderManager::setWireframeMode(bool enabled) {
    m_wireframeState = enabled;
}

void RenderManager::setStatsEnabled(bool enabled) {
    m_statsEnabled = enabled;
}

void RenderManager::printFrameStats() {
    if (!m_statsEnabled) return;
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_frameStartTime);
    
    std::cout << "Frame Stats:" << std::endl;
    std::cout << "  Renderables: " << m_stats.totalRenderables << std::endl;
    std::cout << "  Draw Calls: " << m_stats.drawCalls << std::endl;
    std::cout << "  Shader Switches: " << m_stats.shaderSwitches << std::endl;
    std::cout << "  Material Switches: " << m_stats.materialSwitches << std::endl;
    std::cout << "  Frame Time: " << duration.count() << " µs" << std::endl;
    
    for (const auto& pair : m_stats.renderablesByType) {
        std::cout << "  Type " << static_cast<int>(pair.first) << ": " << pair.second << std::endl;
    }
}

void RenderManager::reset() {
    m_renderQueue.clear();
    m_currentShader.clear();
    m_wireframeState = false;
    m_statsEnabled = false;
    m_stats.reset();
}

void RenderManager::cleanup() {
    m_renderQueue.clear();
    m_currentCamera = nullptr;
}

void RenderManager::RenderStats::reset() {
    totalRenderables = 0;
    drawCalls = 0;
    shaderSwitches = 0;
    materialSwitches = 0;
    renderablesByType.clear();
}

} // namespace FlockingGraphics
