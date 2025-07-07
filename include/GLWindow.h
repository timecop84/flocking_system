#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>
#include <glm/glm.hpp>
#include <memory>
#include <chrono>
#include <QString>

// Forward declaration for modular include
class ShaderLib;

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Vector.h"
#include "Colour.h"
#include "TransformStack.h"
#include "modules/graphics/include/BBox.h"
#include "BehaviorValidator.h"
// UBO support
#include "../modules/graphics/include/UBOStructures.h"
// High-performance instanced rendering
#include "modules/graphics/include/InstancedBoidRenderer.h"
#include "modules/graphics/include/BoidRenderer.h"
#include "modules/graphics/include/ObstacleRenderer.h"

// Forward declarations for GPU flocking
namespace FlockingGraphics {
    class GPUFlockingManager;
}

#include <QTime>
#include "boid.h"
#include "flock.h"
#include "obstacle.h"
#include "PerformanceMonitor.h"

/// @file GLWindow.h
/// @brief a GLWindow to visualize our flock.
/// @author Dionysios Toufexis
/// @brief modified from Jon Maceys BBox Collision. NGL Demos.
/// @date 13/06/12
/// @revision 8/07/12
/**
 * @class GLWindow
 * @brief Main OpenGL widget for visualizing and interacting with the flocking simulation.
 *
 * Provides rendering, user interaction, and simulation control for the flocking system.
 * Integrates modern OpenGL, GPU compute, and advanced UI/UX features.
 *
 * @author Dionysios Toufexis
 * @date 13/06/12 (modified 2025)
 * @details Modernized and extended for advanced GPU/CPU flocking, dynamic UI, and performance profiling.
 */
class GLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for GLWindow.
     * @param _parent The parent widget for this OpenGL context.
     */
    GLWindow(QWidget *_parent);

    /**
     * @brief Destructor for GLWindow.
     */
    ~GLWindow();

    /**
     * @brief Get the current number of boids in the flock.
     * @return The number of boids.
     */
    int getCurrentBoidSize();

    /**
     * @brief Reset the flock to its default state and size.
     */
    void resetFlock();

    /**
     * @brief Set the flock to a specific size and reset all boids.
     * @param size The desired number of boids.
     */
    void applyFlock(int size);

    /**
     * @brief Add a batch of boids to the flock.
     */
    void addBoidsToFlock();

    /**
     * @brief Remove a batch of boids from the flock.
     */
    void removeBoidsFromFlock();

    /**
     * @brief Set the size (scale) of each boid.
     * @param size The new boid size.
     */
    void setBoidSize(double size);

    /**
     * @brief Set the color of all boids.
     * @param colour The new color (QColor).
     */
    void setBoidColor(QColor colour);

    /**
     * @brief Enable or disable wireframe rendering for the flock.
     * @param value True for wireframe, false for solid.
     */
    void setFlockWireframe(bool value);

    /**
     * @brief Set the speed multiplier for the flock simulation.
     * @param multiplier The speed multiplier value.
     */
    void setFlockSpeedMultiplier(float multiplier);

    /**
     * @brief Set the position of the obstacle in the simulation.
     * @param position The new position (glm::vec3).
     */
    void setObstaclePosition(glm::vec3 position);

    /**
     * @brief Set the size (radius) of the obstacle.
     * @param size The new obstacle radius.
     */
    void setObstacleSize(double size);

    /**
     * @brief Set the color of the obstacle.
     * @param colour The new color (QColor).
     */
    void setObstacleColour(QColor colour);

    /**
     * @brief Set the specular color of the obstacle material.
     * @param r Red component (0-1)
     * @param g Green component (0-1)
     * @param b Blue component (0-1)
     */
    void setObstacleSpecular(double r, double g, double b);

    /**
     * @brief Set the diffuse color of the obstacle material.
     * @param r Red component (0-1)
     * @param g Green component (0-1)
     * @param b Blue component (0-1)
     */
    void setObstacleDiffuse(double r, double g, double b);

    /**
     * @brief Enable or disable wireframe rendering for the obstacle.
     * @param value True for wireframe, false for solid.
     */
    void setObstacleWireframe(bool value);

    /**
     * @brief Show or hide the FPS overlay.
     * @param show True to show FPS, false to hide.
     */
    void setShowFPS(bool show);

    /**
     * @brief Get the current frames per second (FPS).
     * @return The current FPS value.
     */
    float getCurrentFPS() const;

    /**
     * @brief Set the simulation distance parameter (for flocking behavior).
     * @param distance The new simulation distance.
     */
    void setSimDistance(double distance);

    /**
     * @brief Set the flock distance parameter (for flocking behavior).
     * @param distance The new flock distance.
     */
    void setSimFlockDistance(double distance);

    /**
     * @brief Set the cohesion parameter for flocking.
     * @param cohesion The new cohesion value.
     */
    void setSimCohesion(double cohesion);

    /**
     * @brief Set the separation parameter for flocking.
     * @param separation The new separation value.
     */
    void setSimSeparation(double separation);

    /**
     * @brief Set the alignment parameter for flocking.
     * @param alignment The new alignment value.
     */
    void setSimAlignment(double alignment);

    /**
     * @brief Set the background color of the OpenGL scene.
     * @param colour The new background color.
     */
    void setBackgroundColour(Colour colour);

    /**
     * @brief Set the size of the bounding box for the simulation.
     * @param size The new bounding box size (glm::vec3).
     */
    void setBBoxSize(glm::vec3 size);

    /**
     * @brief Enable or disable obstacle collision checking.
     * @param enabled True to enable, false to disable.
     */
    void setObstacleCollisionEnabled(bool enabled);

    /**
     * @brief Print a performance comparison between legacy and modern simulation modes.
     */
    void printPerformanceComparison();

    /**
     * @brief Enable or disable performance monitoring.
     * @param enabled True to enable, false to disable.
     */
    void setPerformanceMonitoring(bool enabled);

    /**
     * @brief Validate and log differences between legacy and modern flocking behaviors.
     */
    void validateBehaviorDifferences();

    /**
     * @brief Set the obstacle avoidance radius scale.
     * @param scale The new scale value.
     */
    void setObstacleAvoidanceRadiusScale(float scale);

    /**
     * @brief Set the obstacle collision radius scale.
     * @param scale The new scale value.
     */
    void setObstacleCollisionRadiusScale(float scale);

    /**
     * @brief Set the obstacle repulsion force.
     * @param force The new force value.
     */
    void setObstacleRepulsionForce(float force);

    //-----------------------------------
    /// @brief
    //void update();

private :

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the x rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinXFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the y rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinYFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_rotate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the Right mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_translate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the Middle mouse button is pressed when panning
    //----------------------------------------------------------------------------------------------------------------------
    bool m_pan;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Camera matrices and vectors
    //----------------------------------------------------------------------------------------------------------------------
    Camera *m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief camera orbital controls
    //----------------------------------------------------------------------------------------------------------------------
    float m_cameraDistance;
    float m_cameraAzimuth;
    float m_cameraElevation;
    Vector m_cameraTarget;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    Vector m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a simple light use to illuminate the screen
    //----------------------------------------------------------------------------------------------------------------------
    Light *m_light;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief shader lib instance
    //----------------------------------------------------------------------------------------------------------------------
    ShaderLib *m_shader;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief transform stack for transformations
    //----------------------------------------------------------------------------------------------------------------------
    TransformStack m_transformStack;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief bbox for the flock space
    //----------------------------------------------------------------------------------------------------------------------
    BBox *bbox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a sphere obstacle within the boid space
    //----------------------------------------------------------------------------------------------------------------------
    Obstacle *obstacle;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to the flock class to have access to the methods
    Flock *flock;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief high-performance instanced renderer for boids
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<FlockingGraphics::InstancedBoidRenderer> m_instancedBoidRenderer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief GPU-accelerated flocking simulation using compute shaders
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<FlockingGraphics::GPUFlockingManager> m_gpuFlockingManager;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the GL Depth Color
    //----------------------------------------------------------------------------------------------------------------------
    Colour m_backgroundColour;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief performance monitor for comparing legacy vs modern algorithms
    //----------------------------------------------------------------------------------------------------------------------
    flock::PerformanceMonitor m_performanceMonitor;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief UBO data structures for modern shader pipeline
    //----------------------------------------------------------------------------------------------------------------------
    FlockingShaders::MatrixBlock m_matrixData;
    FlockingShaders::MaterialBlock m_materialData;
    FlockingShaders::LightBlock m_lightData;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief FPS tracking variables
    //----------------------------------------------------------------------------------------------------------------------
    std::chrono::high_resolution_clock::time_point m_lastTime;
    int m_frameCount;
    float m_currentFPS;
    bool m_showFPS;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Pending size values for UI controls set before OpenGL initialization
    //----------------------------------------------------------------------------------------------------------------------
    double m_pendingBoidSize;
    double m_pendingObstacleSize;
    bool m_hasPendingBoidSize;
    bool m_hasPendingObstacleSize;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief UBO update helper methods
    //----------------------------------------------------------------------------------------------------------------------
    void initializeUBOs();
    void updateMatrixUBO(const TransformStack& transformStack);
    void updateMaterialUBO(const Material& material);
    void updateBoidMaterialUBO(const Boid& boid);
    void updateLightUBO();
    void updateLightingUBO();
    void setupLightingUBO();
    void setBoidMaterial(Material& material, const Boid& boid);

    // UBO handles
    GLuint m_matrixUBO;
    GLuint m_lightingUBO;
    
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief FPS calculation and rendering methods
    //----------------------------------------------------------------------------------------------------------------------
    void updateFPS();
    void renderFPSText();
    void renderFPSOverlay();

protected:

    void loadMatricesToColourShader(TransformStack &_tx);

    void loadMatricesToShader(TransformStack &_tx);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief update camera position based on orbital controls
    //----------------------------------------------------------------------------------------------------------------------
    void updateCameraPosition();

    /// @brief  The following methods must be implimented in the sub class
    /// this is called when the window is created
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called whenever the window is re-sized
    /// @param[in] _w the width of the resized window
    /// @param[in] _h the height of the resized window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(
            const int _w,
            const int _h
            );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is the main gl drawing routine which is called whenever the window needs to
    /// be re-drawn
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL();
    
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief paint event override to draw FPS overlay using QPainter
    //----------------------------------------------------------------------------------------------------------------------
    void paintEvent(QPaintEvent *event) override;

private :

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (
            QMouseEvent * _event
            );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent (
            QMouseEvent *_event
            );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent (
            QMouseEvent *_event
            );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent(
            QWheelEvent *_event
            );
    /// @brief timer to change the sphere position by calling update()
    //----------------------------------------------------------------------------------------------------------------------
    void timerEvent(
            QTimerEvent *_event
            );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief key press event handler for performance monitoring and other controls
    /// @param _event the Qt Key Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    int m_sphereUpdateTimer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if animation is active or not
    //----------------------------------------------------------------------------------------------------------------------
    bool m_animate;
    //----------------------------------------------------------------------------------------------------------------------

    double m_obstacleSpecularR = 1.0;
    double m_obstacleSpecularG = 1.0;
    double m_obstacleSpecularB = 1.0;
    double m_obstacleDiffuseR = 1.0;  // Maximum brightness orange-red
    double m_obstacleDiffuseG = 0.8;  // Maximum brightness orange
    double m_obstacleDiffuseB = 0.4;  // Maximum brightness orange
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if obstacle is enabled or not
    //----------------------------------------------------------------------------------------------------------------------
    bool m_obstacleEnabled = true;

    QString m_gpuName; // Stores the GPU renderer name for overlay display

    // Renderer for all boids in the simulation (modularized).
    std::unique_ptr<BoidRenderer> m_boidRenderer;
    // Renderer for the obstacle (modularized).
    std::unique_ptr<ObstacleRenderer> m_obstacleRenderer;

    // --- Obstacle selection and dragging ---
    bool m_obstacleSelected = false;
    QPoint m_lastMousePos;
    Vector m_obstacleDragOffset; // Offset from obstacle center to intersection point
    Vector m_obstacleDragStartWorld; // The world-space point where the drag started
    float m_obstacleDragPlaneZ = 0.0f; // The Z value of the drag plane
    enum class ObstacleDragPlane { XY, XZ, YZ };
    ObstacleDragPlane m_obstacleDragPlane = ObstacleDragPlane::XY;
    // Helper for ray-sphere intersection
    bool intersectRaySphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius, float& tHit) const;

public slots:
    /// @brief slot to set obstacle enabled/disabled
    /// @param [in] _enabled the enabled state
    void setObstacleEnabled(bool _enabled);

};

#endif
