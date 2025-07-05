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

// Forward declarations for GPU flocking
namespace FlockingGraphics {
    class GPUFlockingManager;
}

// must be included after our stuff because GLEW needs to be first
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
/// @class GLWindow
/// @brief the main glwindow widget for our flock application using NGL.
/// put in this file
class GLWindow : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT        // must include this if you use Qt signals/slots
public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for GLWindow
    /// @param [in] _parent the parent window to create the GL context in
    //----------------------------------------------------------------------------------------------------------------------
    GLWindow(
            QWidget *_parent
            );

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor
    //----------------------------------------------------------------------------------------------------------------------
    ~GLWindow();
    /// @brief GUI releated methods
    //-----------------------------
    int  getCurrentBoidSize();
    void resetFlock();
    void applyFlock(int size);
    void addBoidsToFlock();
    void removeBoidsFromFlock();
    void setBoidSize(double size);
    void setBoidColor(QColor colour);
    void setFlockWireframe(bool value);
    void setFlockSpeedMultiplier(float multiplier);

    void setObstaclePosition(glm::vec3 position);
    void setObstacleSize(double size);
    void setObstacleColour(QColor colour);
    void setObstacleSpecular(double r, double g, double b);
    void setObstacleDiffuse(double r, double g, double b);
    void setObstacleWireframe(bool value);
    
    void setShowFPS(bool show);
    float getCurrentFPS() const;

    void setSimDistance(double distance);
    void setSimFlockDistance(double distance);
    void setSimCohesion(double cohesion);
    void setSimSeparation(double separation);
    void setSimAlignment(double alignment);

    void setBackgroundColour(Colour colour);
    void setBBoxSize(glm::vec3 size);
    
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set obstacle collision checking enabled/disabled
    //----------------------------------------------------------------------------------------------------------------------
    void setObstacleCollisionEnabled(bool enabled);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief print performance comparison between legacy and modern modes
    //----------------------------------------------------------------------------------------------------------------------
    void printPerformanceComparison();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief enable/disable performance monitoring
    //----------------------------------------------------------------------------------------------------------------------
    void setPerformanceMonitoring(bool enabled);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief validate behavior differences between legacy and modern systems
    //----------------------------------------------------------------------------------------------------------------------
    void validateBehaviorDifferences();

    void setObstacleAvoidanceRadiusScale(float scale);
    void setObstacleCollisionRadiusScale(float scale);
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

public slots:
    /// @brief slot to set obstacle enabled/disabled
    /// @param [in] _enabled the enabled state
    void setObstacleEnabled(bool _enabled);

};

#endif
