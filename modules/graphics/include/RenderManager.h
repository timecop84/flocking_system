#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <glm/glm.hpp>
#include "TransformStack.h"
#include "Camera.h"
#include "Material.h"
#include "Colour.h"
#include "UBOStructures.h"
#include "ShaderLib.h"

namespace FlockingGraphics {

//----------------------------------------------------------------------------------------------------------------------
/// @brief Advanced unified rendering manager for efficient, reusable rendering pipeline
/// @details Eliminates code duplication, provides batching, UBO caching, and performance tracking
//----------------------------------------------------------------------------------------------------------------------
class RenderManager {
public:
    // Rendering geometry types for efficient batching
    enum class GeometryType {
        SPHERE,
        BBOX,
        CUSTOM
    };
    
    // Rendering object interface
    struct RenderableObject {
        GeometryType type;
        std::function<void()> geometryRender;  // Geometry rendering function
        glm::mat4 transform;                   // Object transform
        Material material;                     // Object material
        bool wireframe = false;                // Wireframe mode
        float depth = 0.0f;                   // For depth sorting
        
        RenderableObject(GeometryType geoType, std::function<void()> render, 
                        const glm::mat4& trans, const Material& mat)
            : type(geoType), geometryRender(render), transform(trans), material(mat) {}
    };
    
    // Performance statistics
    struct RenderStats {
        int totalRenderables = 0;
        int drawCalls = 0;
        int shaderSwitches = 0;
        int materialSwitches = 0;
        std::unordered_map<GeometryType, int> renderablesByType;
        
        void reset();
    };
    
    // Singleton access
    static RenderManager& getInstance();
    
    // Frame lifecycle
    void initialize();
    void destroy();
    void beginFrame(Camera* camera, const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
    void endFrame();
    
    // Render submission and batching
    void submitRenderable(const RenderableObject& renderable);
    void flushBatches();
    
    // Efficient batch rendering (legacy support)
    void renderObjects(const std::vector<RenderableObject>& objects, 
                      const std::string& shaderName,
                      TransformStack& transformStack, 
                      Camera* camera);
    
    // Individual object rendering (for special cases)
    void renderObject(const RenderableObject& object,
                     const std::string& shaderName,
                     TransformStack& transformStack, 
                     Camera* camera);
    
    // UBO management (centralized and cached)
    void updateMatrixUBO(const TransformStack& transformStack, Camera* camera);
    void updateMaterialUBO(const Material& material);
    void updateLightUBO();
    
    // Shader state management
    void useShader(const std::string& shaderName);
    void setWireframeMode(bool enabled);
    
    // Performance tracking
    void setStatsEnabled(bool enabled);
    void printFrameStats();
    const RenderStats& getStats() const { return m_stats; }
    
    // System management
    void reset();
    void cleanup();
    
private:
    RenderManager() = default;
    ~RenderManager();
    
    // Internal rendering methods
    void sortRenderables();
    void renderBatches();
    void renderObject(const RenderableObject& renderable);
    void renderSphere(const RenderableObject& renderable);
    void renderBBox(const RenderableObject& renderable);
    void renderCustom(const RenderableObject& renderable);
    
    // UBO update methods
    void updateGlobalUBOs();
    void updateCameraUBO();
    void updateTransformUBO(const glm::mat4& modelMatrix);
    
    // State and data
    std::vector<RenderableObject> m_renderQueue;
    Camera* m_currentCamera = nullptr;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projMatrix;
    
    // Cache frequently used data
    FlockingShaders::MatrixBlock m_cachedMatrices;
    FlockingShaders::MaterialBlock m_cachedMaterial;
    FlockingShaders::CameraBlock m_cachedCamera;
    
    // Performance optimization
    std::string m_currentShader;
    bool m_wireframeState = false;
    bool m_statsEnabled = false;
    
    // Performance tracking
    RenderStats m_stats;
    std::chrono::high_resolution_clock::time_point m_frameStartTime;
};

} // namespace FlockingGraphics
