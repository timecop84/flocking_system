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
    if (!s_instance) {
        s_instance = new RenderManager();
    }
    return s_instance;
}

void RenderManager::destroy()
{
    delete s_instance;
    s_instance = nullptr;
}

void RenderManager::beginFrame(Camera* camera, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
    m_frameNumber++;
    m_camera = camera;
    m_viewMatrix = viewMatrix;
    m_projMatrix = projMatrix;
    
    // Clear frame statistics
    m_frameStats.reset();
    
    // Reset batching state
    m_batchedRenderables.clear();
    m_sortedRenderables.clear();
    
    // Update global UBOs that don't change during the frame
    updateGlobalUBOs();
    
    // Enable depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void RenderManager::endFrame()
{
    // Flush any remaining batched renderables
    flushBatches();
    
    // Update statistics
    if (m_statsEnabled && m_frameNumber % 60 == 0) {
        printFrameStats();
    }
    
    // Clean up frame resources
    m_currentShader = nullptr;
    m_currentMaterial = nullptr;
}

void RenderManager::submitRenderable(const RenderableObject& renderable)
{
    // Add to batch queue
    m_batchedRenderables.push_back(renderable);
    
    // Update statistics
    m_frameStats.totalRenderables++;
    m_frameStats.renderablesByType[renderable.type]++;
}

void RenderManager::flushBatches()
{
    if (m_batchedRenderables.empty()) {
        return;
    }
    
    // Sort renderables for optimal rendering order
    sortRenderables();
    
    // Render batched objects
    renderBatches();
    
    // Clear the batch
    m_batchedRenderables.clear();
    m_sortedRenderables.clear();
}

void RenderManager::sortRenderables()
{
    m_sortedRenderables = m_batchedRenderables;
    
    // Sort by: shader -> material -> depth (back to front for transparency)
    std::sort(m_sortedRenderables.begin(), m_sortedRenderables.end(),
        [](const RenderableObject& a, const RenderableObject& b) {
            // First by shader
            if (a.shaderName != b.shaderName) {
                return a.shaderName < b.shaderName;
            }
            
            // Then by material (for batching)
            if (a.material != b.material) {
                return a.material < b.material;
            }
            
            // Finally by depth (back to front for proper alpha blending)
            return a.depth > b.depth;
        });
}

void RenderManager::renderBatches()
{
    std::string currentShader = "";
    const Material* currentMaterial = nullptr;
    
    for (const auto& renderable : m_sortedRenderables) {
        // Switch shader if needed
        if (renderable.shaderName != currentShader) {
            useShader(renderable.shaderName);
            currentShader = renderable.shaderName;
            m_frameStats.shaderSwitches++;
        }
        
        // Switch material if needed
        if (renderable.material != currentMaterial) {
            updateMaterialUBO(*renderable.material);
            currentMaterial = renderable.material;
            m_frameStats.materialSwitches++;
        }
        
        // Update transform UBO
        updateTransformUBO(renderable.transform);
        
        // Render the object
        renderObject(renderable);
        
        m_frameStats.drawCalls++;
    }
}

void RenderManager::renderObject(const RenderableObject& renderable)
{
    switch (renderable.type) {
        case RenderableType::SPHERE:
            renderSphere(renderable);
            break;
        case RenderableType::BBOX:
            renderBBox(renderable);
            break;
        case RenderableType::CUSTOM:
            renderCustom(renderable);
            break;
    }
}

void RenderManager::renderSphere(const RenderableObject& renderable)
{
    // Use geometry factory to get sphere geometry
    auto sphere = GeometryFactory::instance()->getSphere(renderable.geometryParams.sphereDetail);
    if (sphere) {
        sphere->draw();
    }
}

void RenderManager::renderBBox(const RenderableObject& renderable)
{
    // Use geometry factory to get bbox geometry
    auto bbox = GeometryFactory::instance()->getBBox();
    if (bbox) {
        bbox->draw();
    }
}

void RenderManager::renderCustom(const RenderableObject& renderable)
{
    // Call custom render function if provided
    if (renderable.customRenderFunc) {
        renderable.customRenderFunc(renderable);
    }
}

void RenderManager::useShader(const std::string& shaderName)
{
    ShaderLib* shaderLib = ShaderLib::instance();
    shaderLib->use(shaderName);
    m_currentShader = (*shaderLib)[shaderName];
}

void RenderManager::updateGlobalUBOs()
{
    // Update camera-dependent UBOs that are constant for the frame
    updateCameraUBO();
    updateLightUBO();
}

void RenderManager::updateCameraUBO()
{
    if (!m_camera) return;
    
    FlockingShaders::CameraBlock cameraData;
    cameraData.viewMatrix = m_viewMatrix;
    cameraData.projMatrix = m_projMatrix;
    cameraData.viewPos = m_camera->getEye();
    cameraData.nearPlane = 0.05f; // Should match camera settings
    cameraData.farPlane = 350.0f;
    
    ShaderLib::instance()->updateUBO("CameraUBO", &cameraData, sizeof(FlockingShaders::CameraBlock));
}

void RenderManager::updateLightUBO()
{
    // Use default lighting setup - can be extended for multiple lights
    FlockingShaders::LightBlock lightData;
    lightData.position = glm::vec4(120.0f, 120.0f, 120.0f, 1.0f);
    lightData.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    lightData.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    lightData.specular = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
    lightData.constantAttenuation = 1.0f;
    lightData.linearAttenuation = 0.0f;
    lightData.quadraticAttenuation = 0.0f;
    lightData.spotCosCutoff = -1.0f;
    
    ShaderLib::instance()->updateUBO("LightUBO", &lightData, sizeof(FlockingShaders::LightBlock));
}

void RenderManager::updateTransformUBO(const glm::mat4& modelMatrix)
{
    FlockingShaders::MatrixBlock matrices;
    matrices.M = modelMatrix;
    matrices.MV = m_viewMatrix * modelMatrix;
    matrices.MVP = m_projMatrix * m_viewMatrix * modelMatrix;
    matrices.normalMatrix = glm::mat3(glm::transpose(glm::inverse(m_viewMatrix * modelMatrix)));
    matrices.viewerPos = m_camera ? m_camera->getEye() : glm::vec3(0.0f);
    matrices.shouldNormalize = 1.0f;
    
    ShaderLib::instance()->updateUBO("MatrixUBO", &matrices, sizeof(FlockingShaders::MatrixBlock));
}

void RenderManager::updateMaterialUBO(const Material& material)
{
    FlockingShaders::MaterialBlock materialData;
    
    Colour ambient = material.getAmbient();
    Colour diffuse = material.getDiffuse();
    Colour specular = material.getSpecular();
    
    materialData.ambient = glm::vec4(ambient.m_r, ambient.m_g, ambient.m_b, ambient.m_a);
    materialData.diffuse = glm::vec4(diffuse.m_r, diffuse.m_g, diffuse.m_b, diffuse.m_a);
    materialData.specular = glm::vec4(specular.m_r, specular.m_g, specular.m_b, specular.m_a);
    materialData.shininess = material.getShininess();
    
    ShaderLib::instance()->updateUBO("MaterialUBO", &materialData, sizeof(FlockingShaders::MaterialBlock));
}

void RenderManager::setStatsEnabled(bool enabled)
{
    m_statsEnabled = enabled;
}

void RenderManager::printFrameStats()
{
    if (!m_statsEnabled) return;
    
    std::cout << "=== RENDER STATS (Frame " << m_frameNumber << ") ===" << std::endl;
    std::cout << "Total Renderables: " << m_frameStats.totalRenderables << std::endl;
    std::cout << "Draw Calls: " << m_frameStats.drawCalls << std::endl;
    std::cout << "Shader Switches: " << m_frameStats.shaderSwitches << std::endl;
    std::cout << "Material Switches: " << m_frameStats.materialSwitches << std::endl;
    
    std::cout << "Renderables by type:" << std::endl;
    for (const auto& pair : m_frameStats.renderablesByType) {
        std::cout << "  " << static_cast<int>(pair.first) << ": " << pair.second << std::endl;
    }
    std::cout << "================================" << std::endl;
}

void RenderManager::reset()
{
    m_batchedRenderables.clear();
    m_sortedRenderables.clear();
    m_frameStats.reset();
    m_frameNumber = 0;
}

void RenderManager::cleanup()
{
    reset();
}

// RenderStats implementation
void RenderManager::RenderStats::reset()
{
    totalRenderables = 0;
    drawCalls = 0;
    shaderSwitches = 0;
    materialSwitches = 0;
    renderablesByType.clear();
}
