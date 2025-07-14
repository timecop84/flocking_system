/**
 * @file GLWindow.cpp
 * @brief Main OpenGL widget implementation for the flocking simulation UI.
 *
 * Handles rendering, user interaction, and simulation control. Integrates modern OpenGL, GPU compute, and advanced UI/UX features.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */

#include <glad/gl.h>
#include "Obstacle.h"
#include <GPUFlockingManager.h>
#include "GLWindow.h"
#include <QMainWindow>
#include <QOpenGLContext>
#include "MainWindow.h"
#include "Flock.h"
#include "Boid.h"
#include <iostream>
#include <cmath>
#include <QSurfaceFormat>
#include "MathUtils.h"
#include <Camera.h>
#include <Colour.h>
#include <Material.h>
#include <TransformStack.h>
#include <Light.h>
#include <ShaderLib.h>
#include <UBOStructures.h>
#include <FrameCoordinator.h>
#include <GeometryFactory.h>
#include <SmartShaderManager.h>
#include <RenderManager.h>
#include <UBOCache.h>
#include <TransformBatcher.h>
#include "PerformanceMonitor.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BehaviorValidator.h"
#include "ShaderConstants.h"
#include "PerformanceProfiler.h"
#include <modules/graphics/include/BoidRenderer.h>



const static float INCREMENT = 1;
const static float ZOOM = 10.0;
GLWindow::GLWindow(
        QWidget *_parent
        )
    : QOpenGLWidget(_parent)
{
    // Configure OpenGL format
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4); // 4x MSAA
    setFormat(format);
    
    obstacle = new Obstacle(Vector(0,0,0), 4.0);
    std::cout << "[GLWindow] Created obstacle at address: " << obstacle << std::endl;

    // Set initial keyboard focus
    setFocus();
    setFocusPolicy(Qt::StrongFocus);
    this->resize(_parent->size());
    // Set initial GLWindow attributes
    m_rotate=false;
    m_translate=false;
    m_pan=false;
    m_spinXFace = 0;
    m_spinYFace = 0;
    
    m_cameraDistance = 200.0f;
    m_cameraAzimuth = 45.0f;
    m_cameraElevation = 30.0f;
    m_cameraTarget.set(0, 0, 0);
    
    m_lastTime = std::chrono::high_resolution_clock::now();
    m_frameCount = 0;
    m_currentFPS = 0.0f;
    m_showFPS = true;
    
    m_pendingBoidSize = 1.0;
    m_pendingObstacleSize = 4.0;
    m_hasPendingBoidSize = false;
    m_hasPendingObstacleSize = false;
    
    std::cout << "\n=== PERFORMANCE PROFILER CONTROLS ===" << std::endl;
    std::cout << "Press 'R' - Print detailed performance report" << std::endl;
    std::cout << "Press 'T' - Reset profiler statistics" << std::endl;
    std::cout << "Press 'G' - Toggle GPU/CPU flocking modes" << std::endl;
    std::cout << "Press 'Space' - Toggle animation" << std::endl;
    std::cout << "Press '+' or '-' - Add/Remove 50 boids" << std::endl;
    std::cout << "Press '0' - Reset to 200 boids (default)" << std::endl;
    std::cout << "Press '1' - Set 500 boids" << std::endl;
    std::cout << "Press '2' - Set 1000 boids" << std::endl;
    std::cout << "Press '3' - Set 2000 boids (TARGET)" << std::endl;
    std::cout << "====================================\n" << std::endl;
    
    m_sphereUpdateTimer = startTimer(1000 / 60);
    m_animate = true;
    m_backgroundColour.set(0.6f, 0.6f, 0.6f, 1.0f);
    
    m_boidRenderer = std::make_unique<BoidRenderer>();
    m_obstacleRenderer = std::make_unique<ObstacleRenderer>();
}

// ...existing code...
GLWindow::~GLWindow()
{
    delete m_light;
    // Qt handles OpenGL cleanup
}

int GLWindow::getCurrentBoidSize()
{
    return flock->getFlockSize();
}

void GLWindow::resetFlock()
{
    flock->setFlockSize(200);
    flock->resetBoids();
}

void GLWindow::applyFlock(int size)
{
    flock->setFlockSize(size);
    flock->resetBoids();
}

void GLWindow::addBoidsToFlock()
{
    flock->addBoids();
}

void GLWindow::removeBoidsFromFlock()
{
    flock->removeBoids();
}

void GLWindow::setBoidSize(double size)
{
    if (flock) {
        flock->setBoidSize(size);
    } else {
        m_pendingBoidSize = size;
        m_hasPendingBoidSize = true;
    }
}

void GLWindow::setBoidColor(QColor colour)
{
    Colour colourToSet;
    colourToSet.set(colour.redF(), colour.greenF(), colour.blueF());

    flock->setColour(colourToSet);
}

void GLWindow::setFlockWireframe(bool value)
{
    flock->setWireframe(value);
}

void GLWindow::setFlockSpeedMultiplier(float multiplier)
{
    flock->setSpeedMultiplier(multiplier);
}

void GLWindow::setObstaclePosition(glm::vec3 position)
{
    std::cout << "[GLWindow] setObstaclePosition: this->obstacle=" << obstacle << std::endl;
    // Only use modern representation
    if (obstacle) {
        obstacle->setPositionModern(flock::Vec3(position.x, position.y, position.z));
    }
    update();
}

void GLWindow::setObstacleSize(double size)
{
    if (obstacle) {
        obstacle->setRadiusModern(static_cast<float>(size));
    } else {
        m_pendingObstacleSize = size;
        m_hasPendingObstacleSize = true;
    }
    update();
}

void GLWindow::setObstacleColour(QColor colour)
{
    std::cout << "[GLWindow] setObstacleColour: this->obstacle=" << obstacle << std::endl;
    if (obstacle) {
        obstacle->setColorModern(flock::Color(colour.redF(), colour.greenF(), colour.blueF(), 1.0f));
    }
    update();
}

void GLWindow::setObstacleWireframe(bool value)
{
    if (obstacle) {
        obstacle->setWireframe(value);
    }
    update();
}

void GLWindow::setSimDistance(double distance)
{
    flock->setSimDistance(distance);
}

void GLWindow::setSimFlockDistance(double distance)
{
    flock->setSimFlockDistance(distance);
}

void GLWindow::setSimCohesion(double cohesion)
{
    flock->setSimCohesion(cohesion);
}

void GLWindow::setSimSeparation(double separation)
{
    flock->setSimSeparation(separation);
}

void GLWindow::setSimAlignment(double alignment)
{
    flock->setSimAlignment(alignment);
}

void GLWindow::setObstacleCollisionEnabled(bool enabled)
{
    flock->setObstacleCollisionEnabled(enabled);
}

void GLWindow::setBackgroundColour(Colour colour)
{
    m_backgroundColour = colour;
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    update(); // Force a repaint to show the new background color
}

void GLWindow::setBBoxSize(glm::vec3 size)
{
    delete bbox;
    bbox = new BBox(Vector(0,0,0), size.x, size.y, size.z);
    bbox->setDrawMode(GL_LINE); // Ensure wireframe mode is set
}
//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining VBOs etc.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{
    // Initialize OpenGL functions - required for QOpenGLWidget with QOpenGLFunctions
    initializeOpenGLFunctions();

    // Cache GPU renderer name for overlay
    const GLubyte* renderer = glGetString(GL_RENDERER);
    if (renderer)
        m_gpuName = QString::fromUtf8(reinterpret_cast<const char*>(renderer));
    else
        m_gpuName = "Unknown GPU";

    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    
    // Initialize glad for OpenGL function loading
    if (!gladLoadGL([](const char* name) { return QOpenGLContext::currentContext()->getProcAddress(name); })) {
        std::cerr << "Failed to initialize glad" << std::endl;
        return;
    }
    
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "glad initialized successfully with OpenGL " << major << "." << minor << std::endl;
    
    // Load shaders and set values
    m_shader = ShaderLib::instance();
    
    
    m_shader->createShaderProgram("Phong");
    m_shader->attachShader("PhongVertex",VERTEX);
    m_shader->attachShader("PhongFragment",FRAGMENT);
    m_shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
    m_shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
    m_shader->compileShader("PhongVertex");
    m_shader->compileShader("PhongFragment");
    m_shader->attachShaderToProgram("Phong","PhongVertex");
    m_shader->attachShaderToProgram("Phong","PhongFragment");
    
    m_shader->bindAttribute("Phong",0,"inVert");
    m_shader->bindAttribute("Phong",1,"inUV");
    m_shader->bindAttribute("Phong",2,"inNormal");
    
    m_shader->linkProgramObject("Phong");
    (*m_shader)["Phong"]->use();
    
    m_shader->bindUniformBlockToBindingPoint("Phong", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("Phong", "MaterialBlock", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("Phong", "LightBlock", FlockingShaders::LIGHT_BINDING_POINT);
    
    m_shader->createShaderProgram("boid_shader");
    m_shader->attachShader("BoidVertex",VERTEX);
    m_shader->attachShader("BoidFragment",FRAGMENT);
    m_shader->loadShaderSource("BoidVertex","shaders/Phong.vs");
    m_shader->loadShaderSource("BoidFragment","shaders/Phong.fs");
    m_shader->compileShader("BoidVertex");
    m_shader->compileShader("BoidFragment");
    m_shader->attachShaderToProgram("boid_shader","BoidVertex");
    m_shader->attachShaderToProgram("boid_shader","BoidFragment");
    m_shader->bindAttribute("boid_shader",0,"inVert");
    m_shader->bindAttribute("boid_shader",1,"inUV");
    m_shader->bindAttribute("boid_shader",2,"inNormal");
    m_shader->linkProgramObject("boid_shader");
    m_shader->bindUniformBlockToBindingPoint("boid_shader", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("boid_shader", "MaterialBlock", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("boid_shader", "LightBlock", FlockingShaders::LIGHT_BINDING_POINT);
    
    m_shader->createShaderProgram("obstacle_shader");
    m_shader->attachShader("ObstacleVertex",VERTEX);
    m_shader->attachShader("ObstacleFragment",FRAGMENT);
    m_shader->loadShaderSource("ObstacleVertex","shaders/Phong.vs");
    m_shader->loadShaderSource("ObstacleFragment","shaders/Phong.fs");
    m_shader->compileShader("ObstacleVertex");
    m_shader->compileShader("ObstacleFragment");
    m_shader->attachShaderToProgram("obstacle_shader","ObstacleVertex");
    m_shader->attachShaderToProgram("obstacle_shader","ObstacleFragment");
    m_shader->bindAttribute("obstacle_shader",0,"inVert");
    m_shader->bindAttribute("obstacle_shader",1,"inUV");
    m_shader->bindAttribute("obstacle_shader",2,"inNormal");
    m_shader->linkProgramObject("obstacle_shader");
    m_shader->bindUniformBlockToBindingPoint("obstacle_shader", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("obstacle_shader", "MaterialBlock", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("obstacle_shader", "LightBlock", FlockingShaders::LIGHT_BINDING_POINT);
    
    m_shader->createShaderProgram("PhongInstanced");
    m_shader->attachShader("PhongInstancedVertex",VERTEX);
    m_shader->attachShader("PhongInstancedFragment",FRAGMENT);
    m_shader->loadShaderSource("PhongInstancedVertex","shaders/PhongInstanced.vs");
    m_shader->loadShaderSource("PhongInstancedFragment","shaders/PhongInstanced.fs");
    m_shader->compileShader("PhongInstancedVertex");
    m_shader->compileShader("PhongInstancedFragment");
    m_shader->attachShaderToProgram("PhongInstanced","PhongInstancedVertex");
    m_shader->attachShaderToProgram("PhongInstanced","PhongInstancedFragment");
    m_shader->bindAttribute("PhongInstanced",0,"inVert");
    m_shader->bindAttribute("PhongInstanced",1,"inUV");
    m_shader->bindAttribute("PhongInstanced",2,"inNormal");
    m_shader->linkProgramObject("PhongInstanced");
    m_shader->bindUniformBlockToBindingPoint("PhongInstanced", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("PhongInstanced", "MaterialBlock", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("PhongInstanced", "LightingBlock", FlockingShaders::LIGHTING_BINDING_POINT);
    
    Material m(GOLD);
    
    // Create camera using orbital controls
    Vector From(200,120,120);
    Vector To(0,0,0);
    Vector Up(0,1,0);
    m_cam= new Camera(From,To,Up,PERSPECTIVE);
    m_cam->setShape(45,(float)720.0/576.0,0.05,2000,PERSPECTIVE);
    
    updateCameraPosition();
    
    Matrix iv=m_cam->getViewMatrix();
    iv.transpose();
    m_light = new Light(Vector(150,200,120,1),Colour(0.8,0.8,0.75,1),Colour(1.0,1.0,0.9,1),POINTLIGHT);
    m_light->setTransform(iv);
    
    initializeUBOs();
    
    updateMaterialUBO(m);
    updateLightUBO();
    
    
    FlockingGraphics::RenderManager::getInstance().initialize();
    
    FlockingGraphics::GeometryFactory::instance().createSphere(1.0f, 16); // Standard boid geometry
    FlockingGraphics::GeometryFactory::instance().createSphere(1.0f, 12); // Lower-quality sphere for obstacles
    FlockingGraphics::GeometryFactory::instance().createBoundingBox();    // Wireframe bounding box
    FlockingGraphics::GeometryFactory::instance().createCube(1.0f);       // Standard cube
    
    auto boidGeometry = FlockingGraphics::GeometryFactory::instance().createSphere(1.0f, 12);
    
    std::cout << "=== FLOCKING GRAPHICS OPTIMIZATION SYSTEM ENABLED ===" << std::endl;
    FlockingGraphics::GeometryFactory::instance().printStats();
    std::cout << "======================================================" << std::endl;
    
    m_shader->createShaderProgram("Colour");

    m_shader->attachShader("ColourVertex",VERTEX);
    m_shader->attachShader("ColourFragment",FRAGMENT);
    m_shader->loadShaderSource("ColourVertex","shaders/Colour.vs");
    m_shader->loadShaderSource("ColourFragment","shaders/Colour.fs");

    m_shader->compileShader("ColourVertex");
    m_shader->compileShader("ColourFragment");
    m_shader->attachShaderToProgram("Colour","ColourVertex");
    m_shader->attachShaderToProgram("Colour","ColourFragment");

    m_shader->bindAttribute("Colour",0,"inVert");

    m_shader->linkProgramObject("Colour");
    
    m_shader->bindUniformBlockToBindingPoint("Colour", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    
    m_shader->createShaderProgram("wireframe_shader");
    m_shader->attachShader("WireframeVertex",VERTEX);
    m_shader->attachShader("WireframeFragment",FRAGMENT);
    m_shader->loadShaderSource("WireframeVertex","shaders/Colour.vs");
    m_shader->loadShaderSource("WireframeFragment","shaders/Colour.fs");
    m_shader->compileShader("WireframeVertex");
    m_shader->compileShader("WireframeFragment");
    m_shader->attachShaderToProgram("wireframe_shader","WireframeVertex");
    m_shader->attachShaderToProgram("wireframe_shader","WireframeFragment");
    m_shader->bindAttribute("wireframe_shader",0,"inVert");
    m_shader->linkProgramObject("wireframe_shader");
    m_shader->bindUniformBlockToBindingPoint("wireframe_shader", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    
    (*m_shader)["Colour"]->use();
    m_shader->setShaderParam4f("Colour",1,1,1,1);
    glEnable(GL_DEPTH_TEST);
    
    // Initialize sphere primitive for boid rendering
    bbox = new BBox(Vector(0,0,0),200,200,200);
    bbox->setDrawMode(GL_LINE);
    flock = new Flock(bbox, obstacle);
    
    // Initialize high-performance instanced boid renderer
    m_instancedBoidRenderer = std::make_unique<FlockingGraphics::InstancedBoidRenderer>();
    m_instancedBoidRenderer->initialize(0.5f, 16);
    m_boidRenderer->setInstancedBoidRenderer(m_instancedBoidRenderer.get());
    
    m_gpuFlockingManager = std::make_unique<FlockingGraphics::GPUFlockingManager>();
    if (!m_gpuFlockingManager->initialize()) {
        std::cerr << "Warning: GPU flocking acceleration failed to initialize, falling back to CPU" << std::endl;
        m_gpuFlockingManager->setEnabled(false);
    }
    
    if (m_hasPendingBoidSize) {
        flock->setBoidSize(m_pendingBoidSize);
        m_hasPendingBoidSize = false;
    }
    if (m_hasPendingObstacleSize) {
        obstacle->setSphereRadius(m_pendingObstacleSize);
        m_hasPendingObstacleSize = false;
    }
    
    if (auto* mainWindow = qobject_cast<MainWindow*>(parent()->parent())) {
        mainWindow->initializeUIValues();
    }
}
//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been updateVelocityresized.
// The new size is passed in width and height.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::resizeGL(
        int _w,
        int _h
        )
{
    // set the viewport for openGL
    glViewport(0,0,_w,_h);
    
    // Update camera with new aspect ratio (modern approach)
    if (m_cam) {
        m_cam->setShape(45,(float)_w/_h,0.05,2000,PERSPECTIVE);
    }
}


void GLWindow::loadMatricesToShader(
        TransformStack &_tx
        )
{
    // Use UBO-based approach for modern shader pipeline
    updateMatrixUBO(_tx);
}
void GLWindow::loadMatricesToColourShader(
        TransformStack &_tx
        )

{
    // Use UBO-based approach for modern shader pipeline
    updateMatrixUBO(_tx);
    
    ShaderLib *shader=ShaderLib::instance();
    (*shader)["Colour"]->use();
}
//----------------------------------------------------------------------------------------------------------------------
// ...existing code...
void GLWindow::paintGL()
{
    PROFILE_SCOPE("Total Frame Render");
    
    static int frame_count = 0;
    frame_count++;
    
    FlockingGraphics::FrameCoordinator::getInstance().beginFrame();
    
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    
    {
        PROFILE_SCOPE("Clear Buffers");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glm::mat4 view = m_cam->getViewMatrix();
    glm::mat4 project = m_cam->getProjectionMatrix();
    
    Matrix nglProject(project);
    Matrix nglView(view);
    m_transformStack.setProjection(nglProject);
    m_transformStack.setView(nglView);
    
    Matrix model;
    model.identity();
    model.translate(m_modelPos.m_x, m_modelPos.m_y, m_modelPos.m_z);
    m_transformStack.setModel(model);
    
    updateLightingUBO();

    if (bbox) {
        PROFILE_SCOPE("Draw Bounding Box");
        m_transformStack.pushTransform();
        {
            Matrix bboxTransform;
            bboxTransform.identity();
            m_transformStack.setModel(bboxTransform.getGLMMat4());
            
            updateMatrixUBO(m_transformStack);
            
            ShaderLib *shader = ShaderLib::instance();
            shader->use("Colour");
            shader->setShaderParam4f("Colour", 1.0f, 1.0f, 1.0f, 1.0f);
            bbox->draw();
        }
        m_transformStack.popTransform();
    }

    if (flock && m_instancedBoidRenderer) {
        m_boidRenderer->render(flock->getBoidList(), m_transformStack, m_cam);
    }

    if (obstacle && m_obstacleEnabled) {
        m_obstacleRenderer->render(obstacle, m_transformStack, m_cam);
    }
    
    updateFPS();
    renderFPSText();
    
    FlockingGraphics::FrameCoordinator::getInstance().endFrame();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
        QMouseEvent * _event
        )
{
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
        int diffx=_event->position().x()-m_origX;
        int diffy=_event->position().y()-m_origY;
        
        m_cameraAzimuth += diffx * 0.5f;
        m_cameraElevation += diffy * 0.5f;
        
        if (m_cameraElevation > 85.0f) m_cameraElevation = 85.0f;
        if (m_cameraElevation < -85.0f) m_cameraElevation = -85.0f;
        
        if (m_cameraAzimuth > 360.0f) m_cameraAzimuth -= 360.0f;
        if (m_cameraAzimuth < 0.0f) m_cameraAzimuth += 360.0f;
        
        updateCameraPosition();
        
        m_origX = _event->position().x();
        m_origY = _event->position().y();
        update();
    }
    // Right mouse button - translate/pan target
    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
        int diffX = (int)(_event->position().x() - m_origXPos);
        int diffY = (int)(_event->position().y() - m_origYPos);
        
        float panSpeed = 0.1f;
        m_cameraTarget.m_x += panSpeed * diffX;
        m_cameraTarget.m_y -= panSpeed * diffY;
        
        updateCameraPosition();
        
        m_origXPos=_event->position().x();
        m_origYPos=_event->position().y();
        update();
    }
    // Middle mouse button - pan camera
    else if(m_pan && _event->buttons() == Qt::MiddleButton)
    {
        int diffX = (int)(_event->position().x() - m_origXPos);
        int diffY = (int)(_event->position().y() - m_origYPos);
        
        // Pan the camera target
        float panSpeed = 0.1f;
        m_cameraTarget.m_x += panSpeed * diffX;
        m_cameraTarget.m_y -= panSpeed * diffY;
        
        updateCameraPosition();
        
        m_origXPos=_event->position().x();
        m_origYPos=_event->position().y();
        update();
    }
    // Obstacle dragging
    if (m_obstacleSelected && _event->buttons() == Qt::LeftButton) {
        if (_event->modifiers() & Qt::ShiftModifier) {
            m_obstacleDragPlane = ObstacleDragPlane::XZ;
        } else if (_event->modifiers() & Qt::ControlModifier) {
            m_obstacleDragPlane = ObstacleDragPlane::YZ;
        } else {
            m_obstacleDragPlane = ObstacleDragPlane::XY;
        }
        float x = 2.0f * _event->position().x() / width() - 1.0f;
        float y = 1.0f - 2.0f * _event->position().y() / height();
        glm::vec4 rayClip(x, y, -1.0f, 1.0f);
        glm::mat4 proj = m_cam->getProjectionMatrix();
        glm::mat4 view = m_cam->getViewMatrix();
        glm::vec4 rayEye = glm::inverse(proj) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
        glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
        glm::vec3 camPos = m_cam->getEye();
        glm::vec3 hitPoint;
        if (m_obstacleDragPlane == ObstacleDragPlane::XY) {
            float denom = rayWorld.z;
            if (fabs(denom) > 1e-6) {
                float t = (m_obstacleDragPlaneZ - camPos.z) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        } else if (m_obstacleDragPlane == ObstacleDragPlane::XZ) {
            float denom = rayWorld.y;
            if (fabs(denom) > 1e-6) {
                float t = (m_obstacleDragPlaneZ - camPos.y) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        } else if (m_obstacleDragPlane == ObstacleDragPlane::YZ) {
            float denom = rayWorld.x;
            if (fabs(denom) > 1e-6) {
                float t = (m_obstacleDragPlaneZ - camPos.x) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        }
        if (!std::isnan(hitPoint.x) && !std::isnan(hitPoint.y) && !std::isnan(hitPoint.z)) {
            obstacle->setSpherePosition(Vector(hitPoint.x, hitPoint.y, hitPoint.z));
            update();
        }
        m_lastMousePos = _event->pos();
        return;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
        QMouseEvent * _event
        )
{
    if (_event->button() == Qt::LeftButton) {
        // Convert mouse position to normalized device coordinates
        float x = 2.0f * _event->position().x() / width() - 1.0f;
        float y = 1.0f - 2.0f * _event->position().y() / height();
        glm::vec4 rayClip(x, y, -1.0f, 1.0f);
        glm::mat4 proj = m_cam->getProjectionMatrix();
        glm::mat4 view = m_cam->getViewMatrix();
        glm::vec4 rayEye = glm::inverse(proj) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
        glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
        glm::vec3 camPos = m_cam->getEye();
        glm::vec3 sphereCenter = glm::vec3(obstacle->getPositionModern().x, obstacle->getPositionModern().y, obstacle->getPositionModern().z);
        float sphereRadius = obstacle->getSphereRadius();
        float tHit;
        if (intersectRaySphere(camPos, rayWorld, sphereCenter, sphereRadius, tHit)) {
            m_obstacleSelected = true;
            m_lastMousePos = _event->pos();
            glm::vec3 hitPoint = camPos + tHit * rayWorld;
            m_obstacleDragStartWorld = Vector(hitPoint.x, hitPoint.y, hitPoint.z);
            if (_event->modifiers() & Qt::ShiftModifier) {
                m_obstacleDragPlane = ObstacleDragPlane::XZ;
                m_obstacleDragPlaneZ = hitPoint.y; // Y fixed
            } else if (_event->modifiers() & Qt::ControlModifier) {
                m_obstacleDragPlane = ObstacleDragPlane::YZ;
                m_obstacleDragPlaneZ = hitPoint.x; // X fixed
            } else {
                m_obstacleDragPlane = ObstacleDragPlane::XY;
                m_obstacleDragPlaneZ = hitPoint.z; // Z fixed
            }
            return;
        }
        // Left mouse button - orbital rotation
        m_origX = _event->position().x();
        m_origY = _event->position().y();
        m_rotate = true;
    }
    else if(_event->button() == Qt::RightButton)
    {
        m_origXPos = _event->position().x();
        m_origYPos = _event->position().y();
        m_translate = true;
    }
    else if(_event->button() == Qt::MiddleButton)
    {
        m_origXPos = _event->position().x();
        m_origYPos = _event->position().y();
        m_pan = true;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
        QMouseEvent * _event
        )
{
    // Left mouse button
    if (_event->button() == Qt::LeftButton)
    {
        m_rotate = false;
    }
    // Right mouse button
    else if (_event->button() == Qt::RightButton)
    {
        m_translate = false;
    }
    // Middle mouse button
    else if (_event->button() == Qt::MiddleButton)
    {
        m_pan = false;
    }

    if (_event->button() == Qt::LeftButton && m_obstacleSelected) {
        m_obstacleSelected = false;
        return;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{
    float zoomFactor = 10.0f;
    
    if(_event->angleDelta().y() > 0)
    {
        m_cameraDistance -= zoomFactor;
        if (m_cameraDistance < 5.0f) m_cameraDistance = 5.0f;
    }
    else if(_event->angleDelta().y() < 0 )
    {
        m_cameraDistance += zoomFactor;
        if (m_cameraDistance > 1500.0f) m_cameraDistance = 1500.0f;
    }
    
    updateCameraPosition();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::timerEvent(
        QTimerEvent *_event
        )
{

    if(_event->timerId() == m_sphereUpdateTimer)
    {
        if (m_animate !=true)
        {


            return;
        }

        {
            PROFILE_SCOPE("Total Frame Update");
            {
                PROFILE_SCOPE("Flock Update");
                if (m_gpuFlockingManager && m_gpuFlockingManager->isEnabled()) {
                    PROFILE_SCOPE("GPU Flocking Update");
                    const std::vector<Boid*>& boidList = flock->getBoidList();
                    std::vector<FlockingGraphics::GPUBoidData> gpuBoidData;
                    gpuBoidData.reserve(boidList.size());
                    for (Boid* boid : boidList) {
                        FlockingGraphics::GPUBoidData gpuBoid;
                        Vector pos = boid->getPosition();
                        gpuBoid.position = glm::vec3(pos.m_x, pos.m_y, pos.m_z);
                        Vector vel = boid->getVelocity();
                        gpuBoid.velocity = glm::vec3(vel.m_x, vel.m_y, vel.m_z);
                        Vector lastPos = boid->getLastPosition();
                        gpuBoid.lastPosition = glm::vec3(lastPos.m_x, lastPos.m_y, lastPos.m_z);
                        flock::Color colorModern = boid->getColorModern();
                        gpuBoid.color = glm::vec4(colorModern.r, colorModern.g, colorModern.b, colorModern.a);
                        gpuBoidData.push_back(gpuBoid);
                    }
                    
                    FlockingGraphics::FlockingParameters params;
                    params.separationDistance = static_cast<float>(flock->getBehaviours()->getFlockDistance());
                    params.alignmentDistance = static_cast<float>(flock->getBehaviours()->getBehaviourDistance());
                    params.cohesionDistance = static_cast<float>(flock->getBehaviours()->getBehaviourDistance());
                    params.separationForce = static_cast<float>(flock->getBehaviours()->getSeparationForce());
                    params.alignmentForce = static_cast<float>(flock->getBehaviours()->getAlignment());
                    params.cohesionForce = static_cast<float>(flock->getBehaviours()->getCohesionForce());
                    params.maxSpeed = 2.0f;
                    params.maxForce = 0.5f;
                    params.numBoids = static_cast<int>(boidList.size());
                    params.deltaTime = 0.016f;
                    params.speedMultiplier = flock->getSpeedMultiplier();
                    
                    static bool debugPrinted = false;
                    if (!debugPrinted) {
                        std::cout << "GPU Flocking Parameters:" << std::endl;
                        std::cout << "  separationDistance: " << params.separationDistance << std::endl;
                        std::cout << "  alignmentDistance: " << params.alignmentDistance << std::endl;
                        std::cout << "  cohesionDistance: " << params.cohesionDistance << std::endl;
                        std::cout << "  separationForce: " << params.separationForce << std::endl;
                        std::cout << "  alignmentForce: " << params.alignmentForce << std::endl;
                        std::cout << "  cohesionForce: " << params.cohesionForce << std::endl;
                        std::cout << "  maxSpeed: " << params.maxSpeed << std::endl;
                        std::cout << "  maxForce: " << params.maxForce << std::endl;
                        std::cout << "  deltaTime: " << params.deltaTime << std::endl;
                        std::cout << "  speedMultiplier: " << params.speedMultiplier << std::endl;
                        debugPrinted = true;
                    }
                    
                    if (bbox) {
                        Vector center = bbox->getCenter();
                        float width = bbox->getWidth() / 2.0f;
                        float height = bbox->getHeight() / 2.0f;
                        float depth = bbox->getDepth() / 2.0f;
                        params.boundingBoxMin = glm::vec3(center.m_x - width, center.m_y - height, center.m_z - depth);
                        params.boundingBoxMax = glm::vec3(center.m_x + width, center.m_y + height, center.m_z + depth);
                    }
                    
                    // if (obstacle) {
                    //     Vector obsPos = obstacle->getSpherePosition();
                    //     params.obstaclePosition = glm::vec3(obsPos.m_x, obsPos.m_y, obsPos.m_z);
                    //     params.obstacleRadius = static_cast<float>(obstacle->getSphereRadius());
                    // }
                    
                    m_gpuFlockingManager->updateParameters(params);
                    m_gpuFlockingManager->uploadBoidData(gpuBoidData);
                    m_gpuFlockingManager->computeFlocking();
                    
                    std::vector<FlockingGraphics::GPUBoidData> results = m_gpuFlockingManager->downloadBoidData();
                    
                    for (size_t i = 0; i < boidList.size() && i < results.size(); i++) {
                        const FlockingGraphics::GPUBoidData& result = results[i];
                        
                        Vector newPos(result.position.x, result.position.y, result.position.z);
                        boidList[i]->setPosition(newPos);
                        
                        Vector newVel(result.velocity.x, result.velocity.y, result.velocity.z);
                        boidList[i]->setVelocity(newVel);
                        
                        boidList[i]->boidDirection();
                    }
                    
                    flock->checkCollisions();
                    
                    static int frameCounter = 0;
                    if (++frameCounter % 120 == 0) { // Every 2 seconds at 60 FPS
                        std::cout << "GPU Flocking: " << m_gpuFlockingManager->getLastComputeTime() << "ms for " 
                                  << boidList.size() << " boids" << std::endl;
                    }
                } else {
                    flock->update();
                }
            }
        }
        update();
    }

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateCameraPosition()
{
    float radAzimuth = glm::radians(m_cameraAzimuth);
    float radElevation = glm::radians(m_cameraElevation);
    
    float x = m_cameraDistance * cos(radElevation) * cos(radAzimuth);
    float y = m_cameraDistance * sin(radElevation);
    float z = m_cameraDistance * cos(radElevation) * sin(radAzimuth);
    
    Vector cameraPos(m_cameraTarget.m_x + x, m_cameraTarget.m_y + y, m_cameraTarget.m_z + z);
    Vector up(0, 1, 0);
    
    m_cam->lookAt(cameraPos, m_cameraTarget, up);
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::printPerformanceComparison()
{
    m_performanceMonitor.printComparison();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setPerformanceMonitoring(bool enabled)
{
    m_performanceMonitor.setEnabled(enabled);
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::keyPressEvent(QKeyEvent *_event)
{
    switch (_event->key()) {
        case Qt::Key_P:
            printPerformanceComparison();
            break;
        case Qt::Key_R:
            PerformanceProfiler::getInstance().printReport();
            break;
        case Qt::Key_T:
            PerformanceProfiler::getInstance().reset();
            std::cout << "Performance profiler statistics reset." << std::endl;
            break;
        case Qt::Key_M:
            setPerformanceMonitoring(!m_performanceMonitor.isEnabled());
            break;
        case Qt::Key_C:
            m_performanceMonitor.clear();
            break;
        case Qt::Key_Space:
            m_animate = !m_animate;
            break;
        case Qt::Key_V:
            validateBehaviorDifferences();
            break;
        case Qt::Key_G:
            if (m_gpuFlockingManager) {
                bool wasEnabled = m_gpuFlockingManager->isEnabled();
                m_gpuFlockingManager->toggleGPUMode();
                std::cout << "Switched from " << (wasEnabled ? "GPU" : "CPU") 
                          << " to " << (m_gpuFlockingManager->isEnabled() ? "GPU" : "CPU") 
                          << " flocking mode" << std::endl;
            } else {
                std::cout << "GPU flocking manager not available" << std::endl;
            }
            break;
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            if (flock) {
                for (int i = 0; i < 5; i++) {  // Add 5 sets of 10 = 50 boids
                    flock->addBoids();
                }
                std::cout << "Added boids. Total: " << flock->getFlockSize() << std::endl;
            }
            break;
        case Qt::Key_Minus:
            if (flock) {
                for (int i = 0; i < 5 && flock->getFlockSize() > 10; i++) {  // Remove 5 sets of 10 = 50 boids
                    flock->removeBoids();
                }
                std::cout << "Removed boids. Total: " << flock->getFlockSize() << std::endl;
            }
            break;
        case Qt::Key_1:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 500;
                std::cout << "Setting flock size from " << currentSize << " to " << target << std::endl;
                
                if (currentSize < target) {
                    // Add boids
                    while (flock->getFlockSize() < target) {
                        flock->addBoids();
                    }
                } else if (currentSize > target) {
                    // Remove boids
                    while (flock->getFlockSize() > target) {
                        flock->removeBoids();
                    }
                }
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case Qt::Key_2:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 1000;
                std::cout << "Setting flock size from " << currentSize << " to " << target << std::endl;
                
                if (currentSize < target) {
                    // Add boids
                    while (flock->getFlockSize() < target) {
                        flock->addBoids();
                    }
                } else if (currentSize > target) {
                    // Remove boids
                    while (flock->getFlockSize() > target) {
                        flock->removeBoids();
                    }
                }
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case Qt::Key_3:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 2000;
                std::cout << "Setting flock size from " << currentSize << " to " << target << " - TARGET!" << std::endl;
                flock->setFlockSize(target);
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case Qt::Key_4:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 4000;
                std::cout << "Setting flock size from " << currentSize << " to " << target << " - MASSIVE!" << std::endl;
                flock->setFlockSize(target);
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case Qt::Key_0:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 200;
                std::cout << "Resetting flock size from " << currentSize << " to " << target << " (default)" << std::endl;
                
                if (currentSize < target) {
                    // Add boids
                    while (flock->getFlockSize() < target) {
                        flock->addBoids();
                    }
                } else if (currentSize > target) {
                    // Remove boids
                    while (flock->getFlockSize() > target) {
                        flock->removeBoids();
                    }
                }
                std::cout << "Flock size reset to " << flock->getFlockSize() << std::endl;
            }
            break;
        default:
            QOpenGLWidget::keyPressEvent(_event);
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::validateBehaviorDifferences()
{
    if (!flock) {
        return;
    }
    
    std::cout << "\n=== BEHAVIOR VALIDATION STARTED ===" << std::endl;
    std::cout << "Current mode: Modern GLM" << std::endl;
    
    // Validate the first few boids to get a sample
    int boidsToValidate = std::min(5, flock->getFlockSize());
    
    for (int i = 0; i < boidsToValidate; i++) {
        BehaviorValidator::logDetailedComparison(i, flock->getBoidList(), flock->getBehaviours());
    }
    
    std::cout << "Validation completed for " << boidsToValidate << " boids" << std::endl;
    std::cout << "===================================\n" << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Initialize UBOs for modern shader pipeline
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeUBOs()
{
    // Create UBOs for matrix, material, and light data
    m_shader->createUBO("MatrixUBO", sizeof(FlockingShaders::MatrixBlock));
    m_shader->createUBO("MaterialUBO", sizeof(FlockingShaders::MaterialBlock));
    m_shader->createUBO("LightUBO", sizeof(FlockingShaders::LightBlock));
    
    // Bind UBOs to their respective binding points
    m_shader->bindUBOToBindingPoint("MatrixUBO", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUBOToBindingPoint("MaterialUBO", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUBOToBindingPoint("LightUBO", FlockingShaders::LIGHT_BINDING_POINT);
    
    // Setup lighting UBO
    setupLightingUBO();
}

void GLWindow::setupLightingUBO() {
    FlockingShaders::LightingBlock lighting;
    
    // Set default light position and color
    lighting.lightPos = glm::vec3(50.0f, 50.0f, 50.0f);
    lighting.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    lighting.shininess = 32.0f;
    lighting.pad1 = lighting.pad2 = 0.0f;  // Required padding for std140 layout
    
    // Create and initialize the UBO
    ShaderLib::instance()->createUBO("LightingUBO", sizeof(FlockingShaders::LightingBlock));
    ShaderLib::instance()->updateUBO("LightingUBO", &lighting, sizeof(FlockingShaders::LightingBlock));
    ShaderLib::instance()->bindUBOToBindingPoint("LightingUBO", FlockingShaders::LIGHTING_BINDING_POINT);
}

void GLWindow::updateLightingUBO() {
    FlockingShaders::LightingBlock lighting;
    // In view space, the camera is always at the origin
    lighting.viewPos = glm::vec3(0.0f, 0.0f, 0.0f);
    
    // Transform light position to view space
    // Use the same light source as the obstacle for consistency
    glm::vec3 lightWorldPos = m_light ? glm::vec3(m_light->getPosition()) : glm::vec3(150.0f, 200.0f, 120.0f);
    glm::mat4 viewMatrix = m_cam->getViewMatrix();
    glm::vec4 lightViewPos = viewMatrix * glm::vec4(lightWorldPos, 1.0f);
    lighting.lightPos = glm::vec3(lightViewPos); // Light position in view space
    
    lighting.lightColor = m_light ? glm::vec3(m_light->getColor()) * 0.6f : glm::vec3(0.6f, 0.6f, 0.55f);  // Further reduced intensity for darker, more natural appearance
    lighting.shininess = 32.0f;
    lighting.pad1 = lighting.pad2 = 0.0f;
    
    // Update the UBO
    ShaderLib::instance()->updateUBO("LightingUBO", &lighting, sizeof(FlockingShaders::LightingBlock));
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update matrix UBO with current transform data
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateMatrixUBO(const TransformStack& transformStack) {
    FlockingShaders::MatrixBlock matrices;
    
    // Compose matrices for UBO
    glm::mat4 model = transformStack.getCurrentTransform();
    glm::mat4 view = m_cam->getViewMatrix();
    glm::mat4 proj = m_cam->getProjectionMatrix();
    matrices.M = model;
    matrices.MV = view * model;
    matrices.MVP = proj * view * model;
    matrices.normalMatrix = glm::mat3(glm::transpose(glm::inverse(view * model)));
    matrices.viewerPos = m_cam->getEye();
    matrices.shouldNormalize = 1.0f;
    ShaderLib::instance()->updateUBO("MatrixUBO", &matrices, sizeof(FlockingShaders::MatrixBlock));
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update material UBO with material properties
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateMaterialUBO(const Material& material)
{
    // Convert NGL Material to UBO format
    Colour ambient = material.getAmbient();
    Colour diffuse = material.getDiffuse();
    Colour specular = material.getSpecular();
    
    m_materialData.ambient = glm::vec4(ambient.m_r, ambient.m_g, ambient.m_b, ambient.m_a);
    m_materialData.diffuse = glm::vec4(diffuse.m_r, diffuse.m_g, diffuse.m_b, diffuse.m_a);
    m_materialData.specular = glm::vec4(specular.m_r, specular.m_g, specular.m_b, specular.m_a);
    m_materialData.shininess = material.getShininess();
    
    // Update the UBO
    m_shader->updateUBO("MaterialUBO", &m_materialData, sizeof(FlockingShaders::MaterialBlock));
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update light UBO with light properties
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateLightUBO()
{
    if (m_light) {
        // Convert NGL Light to UBO format
        glm::vec3 lightPos = m_light->getPosition();
        glm::vec3 lightColor = m_light->getColor();
        
        // Debug output disabled for performance
        // static int lightDebugCounter = 0;
        // if (lightDebugCounter++ % 120 == 0) { // Print every 2 seconds at 60fps
        //     std::cout << "Light position (world): (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ")" << std::endl;
        //     std::cout << "Light color: (" << lightColor.x << ", " << lightColor.y << ", " << lightColor.z << ")" << std::endl;
        // }
        
        m_lightData.position = glm::vec4(lightPos, 1.0f); // Keep light in world space
        // Enhanced lighting components for smoother appearance
        m_lightData.ambient = glm::vec4(lightColor * 0.5f, 1.0f);  // Moderate ambient for smooth gradient support
        m_lightData.diffuse = glm::vec4(lightColor, 1.0f);         // Full diffuse
        m_lightData.specular = glm::vec4(lightColor * 2.0f, 1.0f); // High specular light for visible highlights
        
        // Set attenuation values
        m_lightData.constantAttenuation = m_light->getConstantAttenuation();
        m_lightData.linearAttenuation = m_light->getLinearAttenuation();
        m_lightData.quadraticAttenuation = m_light->getQuadraticAttenuation();
        m_lightData.spotCosCutoff = -1.0f; // No spotlight (directional/point light)
        
        // Update the UBO
        m_shader->updateUBO("LightUBO", &m_lightData, sizeof(FlockingShaders::LightBlock));
    }
}

void GLWindow::setObstacleSpecular(double r, double g, double b)
{
    m_obstacleSpecularR = r;
    m_obstacleSpecularG = g;
    m_obstacleSpecularB = b;
    update(); // Trigger a redraw
}

void GLWindow::setObstacleDiffuse(double r, double g, double b)
{
    m_obstacleDiffuseR = r;
    m_obstacleDiffuseG = g;
    m_obstacleDiffuseB = b;
    if (obstacle) {
        Colour colourToSet;
        colourToSet.set(r, g, b);
        obstacle->setColour(colourToSet);
        obstacle->setColorModern(flock::Color(r, g, b, 1.0f));
    }
    update(); // Trigger a redraw
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update material UBO with boid-specific material properties
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateBoidMaterialUBO(const Boid& boid)
{
    // Get boid color
    flock::Color boidColor = boid.getColorModern();
    
    // Set up boid material properties similar to immediate mode
    m_materialData.ambient = glm::vec4(boidColor.r * 0.3f, boidColor.g * 0.3f, boidColor.b * 0.3f, 1.0f);
    m_materialData.diffuse = glm::vec4(boidColor.r, boidColor.g, boidColor.b, 1.0f);
    m_materialData.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    m_materialData.shininess = 64.0f;
    
    // Update the UBO
    m_shader->updateUBO("MaterialUBO", &m_materialData, sizeof(FlockingShaders::MaterialBlock));
}

void GLWindow::setBoidMaterial(Material& material, const Boid& boid)
{
    // Get boid color
    flock::Color boidColor = boid.getColorModern();
    
    // Set up boid material properties similar to immediate mode
    material.setAmbient(Colour(boidColor.r * 0.3f, boidColor.g * 0.3f, boidColor.b * 0.3f, 1.0f));
    material.setDiffuse(Colour(boidColor.r, boidColor.g, boidColor.b, 1.0f));
    material.setSpecular(Colour(0.8f, 0.8f, 0.8f, 1.0f));
    material.setShininess(64.0f);
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::setShowFPS(bool show) {
    m_showFPS = show;
}

//----------------------------------------------------------------------------------------------------------------------
float GLWindow::getCurrentFPS() const {
    return m_currentFPS;
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateFPS() {
    m_frameCount++;
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_lastTime);
    
    // Update FPS every 500ms for smooth display
    if (timeDiff.count() >= 500) {
        m_currentFPS = (m_frameCount * 1000.0f) / timeDiff.count();
        m_frameCount = 0;
        m_lastTime = currentTime;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::renderFPSText() {
    if (!m_showFPS) return;
    // Terminal FPS/boid count debug output and automatic profiling report removed.
    // Profiling report is now only printed on demand (e.g., when pressing 'T').
}

void GLWindow::renderFPSOverlay() {
    if (!m_showFPS) return;
    
    // This method will be called from paintEvent using QPainter
    // The actual text rendering will be done there
}

void GLWindow::paintEvent(QPaintEvent *event) {
    // First, perform the standard OpenGL rendering
    QOpenGLWidget::paintEvent(event);

    // Then, draw the FPS overlay using QPainter
    if (m_showFPS) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Set up the font for the FPS text
        QFont font = painter.font();
        font.setPointSize(12);
        font.setBold(true);
        painter.setFont(font);

        // Prepare the FPS text
        QString fpsText = QString("FPS: %1").arg(m_currentFPS, 0, 'f', 1);
        QString boidText = QString("Boids: %1").arg(flock ? flock->getFlockSize() : 0);
        QString modeText;
        if (m_gpuFlockingManager && m_gpuFlockingManager->isEnabled()) {
            modeText = QString("Mode: GPU - %1").arg(m_gpuName);
        } else {
            modeText = "Mode: CPU";
        }

        // Calculate text metrics
        QFontMetrics fm(font);
        int textWidth = qMax(qMax(fm.horizontalAdvance(fpsText), fm.horizontalAdvance(boidText)), fm.horizontalAdvance(modeText));
        int textHeight = fm.height();

        // Draw semi-transparent background (dark theme, more modern look)
        int padding = 8;
        int bgWidth = textWidth + 2 * padding;
        int bgHeight = 3 * textHeight + 4 * padding;
        QRect bgRect(10, 10, bgWidth, bgHeight);
        // Use a deep blue-gray for the background
        painter.fillRect(bgRect, QColor(24, 28, 40, 220));

        // Draw border with a subtle blue accent
        painter.setPen(QPen(QColor(80, 160, 255), 2));
        painter.drawRect(bgRect);

        // Draw FPS text
        QColor textColor = Qt::white;
        if (m_currentFPS >= 50.0f) {
            textColor = QColor(120, 255, 180); // soft green
        } else if (m_currentFPS >= 30.0f) {
            textColor = QColor(255, 220, 120); // soft yellow
        } else {
            textColor = QColor(255, 120, 120); // soft red
        }
        painter.setPen(textColor);
        painter.drawText(10 + padding, 10 + padding + textHeight, fpsText);

        // Draw boid count in a cool blue
        painter.setPen(QColor(120, 180, 255));
        painter.drawText(10 + padding, 10 + 2 * padding + 2 * textHeight, boidText);

        // Draw GPU/CPU mode and GPU name in a subtle gray
        painter.setPen(QColor(180, 200, 220));
        painter.drawText(10 + padding, 10 + 3 * padding + 3 * textHeight, modeText);

        // Draw CPU thread count if in CPU mode
        if (!m_gpuFlockingManager || !m_gpuFlockingManager->isEnabled()) {
            QString threadText = QString("CPU Threads: %1").arg(m_boidRenderer ? m_boidRenderer->getNumThreads() : 1);
            painter.setPen(QColor(180, 200, 220));
            painter.drawText(10 + padding, 10 + 4 * padding + 4 * textHeight, threadText);
        }

        painter.end();
    }
}

void GLWindow::setObstacleEnabled(bool _enabled)
{
    m_obstacleEnabled = _enabled;
    // Also control the collision checking in the flock
    setObstacleCollisionEnabled(_enabled);
    update(); // Force a repaint to show/hide the obstacle
}

void GLWindow::setObstacleAvoidanceRadiusScale(float scale)
{
    if (flock) flock->setObstacleAvoidanceRadiusScale(scale);
}

void GLWindow::setObstacleCollisionRadiusScale(float scale)
{
    if (flock) flock->setObstacleCollisionRadiusScale(scale);
}

void GLWindow::setObstacleRepulsionForce(float force)
{
    if (flock) flock->setObstacleRepulsionForce(force);
}

bool GLWindow::intersectRaySphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius, float& tHit) const {
    glm::vec3 oc = rayOrigin - sphereCenter;
    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(oc, rayDir);
    float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return false;
    tHit = (-b - sqrt(discriminant)) / (2.0f * a);
    return tHit >= 0;
}

