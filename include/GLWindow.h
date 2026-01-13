// ...existing code...
#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <memory>
#include <chrono>
#include <string>

#include "FlockTypes.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Vector.h"
#include "Colour.h"
#include "TransformStack.h"
#include "BBox.h"
#include "BehaviorValidator.h"
#include "UBOStructures.h"
#include "InstancedBoidRenderer.h"
#include "BoidRenderer.h"
#include "ObstacleRenderer.h"
#include "ShaderLib.h"

#include "Boid.h"
#include "Flock.h"
#include "Obstacle.h"
#include "PerformanceMonitor.h"

// Forward declarations for GPU flocking
namespace FlockingGraphics {
    class GPUFlockingManager;
}

/**
 * @file GLWindow.h
 * @brief Core rendering and simulation controller for the ImGui-based flocking system.
 */
class GLWindow
{
public:
    struct BoidStats {
        int count = 0;
        int nanCount = 0;
        glm::vec3 min{0.0f};
        glm::vec3 max{0.0f};
        glm::vec3 avg{0.0f};
    };

    GLWindow();
    ~GLWindow();

    void initialize(int width, int height);
    void resize(int width, int height);
    void render();
    void updateSimulation(float deltaTime);

    void onMouseMove(double x, double y, bool leftDown, bool rightDown, bool middleDown, bool shift, bool ctrl);
    void onMouseButton(double x, double y, int button, int action, int mods);
    void onScroll(double xoffset, double yoffset);
    void onKey(int key, int action);

    int getCurrentBoidSize();
    BoidStats getBoidStats() const;
    float getCurrentFPS() const { return m_currentFPS; }
    bool isGPUModeEnabled() const;
    int getThreadCount() const;
    const std::string& getGPUName() const { return m_gpuName; }

    void resetFlock();
    void applyFlock(int size);
    void addBoidsToFlock();
    void removeBoidsFromFlock();
    void setBoidSize(double size);
    void setBoidColor(const glm::vec3& colour);
    void setFlockWireframe(bool value);
    void setFlockSpeedMultiplier(float multiplier);
    void setObstaclePosition(glm::vec3 position);
    void setObstacleSize(double size);
    void setObstacleColour(const glm::vec3& colour);
    void setObstacleSpecular(double r, double g, double b);
    void setObstacleDiffuse(double r, double g, double b);
    void setObstacleWireframe(bool value);
    void setShowFPS(bool show);
    void setSimDistance(double distance);
    void setSimFlockDistance(double distance);
    void setSimCohesion(double cohesion);
    void setSimSeparation(double separation);
    void setSimAlignment(double alignment);
    void setBackgroundColour(Colour colour);
    void setBBoxSize(glm::vec3 size);
    void setObstacleCollisionEnabled(bool enabled);
    void printPerformanceComparison();
    void setPerformanceMonitoring(bool enabled);
    void validateBehaviorDifferences();
    void setObstacleAvoidanceRadiusScale(float scale);
    void setObstacleCollisionRadiusScale(float scale);
    void setObstacleRepulsionForce(float force);
    void setObstacleEnabled(bool enabled);

private:
    int m_spinXFace;
    int m_spinYFace;
    bool m_rotate;
    bool m_translate;
    bool m_pan;
    int m_origX;
    int m_origY;
    int m_origXPos;
    int m_origYPos;
    std::unique_ptr<Camera> m_cam;
    float m_cameraDistance;
    float m_cameraAzimuth;
    float m_cameraElevation;
    Vector m_cameraTarget;
    Vector m_modelPos;
    std::unique_ptr<Light> m_light;
    ShaderLib *m_shader;
    TransformStack m_transformStack;
    std::unique_ptr<BBox> bbox;
    std::unique_ptr<Obstacle> obstacle;
    std::unique_ptr<Flock> flock;
    std::unique_ptr<FlockingGraphics::InstancedBoidRenderer> m_instancedBoidRenderer;
    std::unique_ptr<FlockingGraphics::GPUFlockingManager> m_gpuFlockingManager;
    Colour m_backgroundColour;
    flock::PerformanceMonitor m_performanceMonitor;
    FlockingShaders::MatrixBlock m_matrixData;
    FlockingShaders::MaterialBlock m_materialData;
    FlockingShaders::LightingBlock m_lightData;
    std::chrono::high_resolution_clock::time_point m_lastTime;
    int m_frameCount;
    float m_currentFPS;
    bool m_showFPS;
    double m_pendingBoidSize;
    double m_pendingObstacleSize;
    bool m_hasPendingBoidSize;
    bool m_hasPendingObstacleSize;
    int m_viewportWidth = 1;
    int m_viewportHeight = 1;
    bool m_leftMouseDown = false;
    bool m_rightMouseDown = false;
    bool m_middleMouseDown = false;
    bool m_shiftDown = false;
    bool m_ctrlDown = false;
    glm::vec2 m_lastCursor{0.0f};
    bool m_needsRedraw = true;
    std::string m_gpuName;

    void initializeUBOs();
    void updateMatrixUBO(const TransformStack& transformStack);
    void updateMaterialUBO(const Material& material);
    void updateBoidMaterialUBO(const Boid& boid);
    void updateLightUBO();
    void updateLightingUBO();
    void setupLightingUBO();
    void setBoidMaterial(Material& material, const Boid& boid);

    GLuint m_matrixUBO;
    GLuint m_lightingUBO;
    
    void updateFPS();
    void loadMatricesToColourShader(TransformStack &_tx);
    void loadMatricesToShader(TransformStack &_tx);
    void updateCameraPosition();

    bool m_animate;

    double m_obstacleSpecularR = 1.0;
    double m_obstacleSpecularG = 1.0;
    double m_obstacleSpecularB = 1.0;
    double m_obstacleDiffuseR = 1.0;
    double m_obstacleDiffuseG = 0.8;
    double m_obstacleDiffuseB = 0.4;
    bool m_obstacleEnabled = false;  // Start with obstacle disabled to avoid collision issues

    std::unique_ptr<BoidRenderer> m_boidRenderer;
    std::unique_ptr<ObstacleRenderer> m_obstacleRenderer;

    bool m_obstacleSelected = false;
    glm::vec2 m_lastMousePos{0.0f};
    Vector m_obstacleDragOffset;
    Vector m_obstacleDragStartWorld;
    float m_obstacleDragPlaneZ = 0.0f;
    enum class ObstacleDragPlane { XY, XZ, YZ };
    ObstacleDragPlane m_obstacleDragPlane = ObstacleDragPlane::XY;
    bool intersectRaySphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius, float& tHit) const;
};
