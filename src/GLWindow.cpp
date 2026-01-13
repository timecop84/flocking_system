/**
 * @file GLWindow.cpp
 * @brief Main OpenGL widget implementation for the flocking simulation UI.
 *
 * Handles rendering, user interaction, and simulation control. Integrates modern OpenGL, GPU compute, and advanced UI/UX features.
 *
 * @author Dennis Toufexis
 * @date 2025
 */

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Obstacle.h"
#include <GPUFlockingManager.h>
#include "GLWindow.h"
#include "Flock.h"
#include "Boid.h"
#include <iostream>
#include <cmath>
#include <limits>
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
#include <ShaderManager.h>
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
#include <BoidRenderer.h>



const static float INCREMENT = 1;
const static float ZOOM = 10.0;
GLWindow::GLWindow()
{
    obstacle = new Obstacle(Vector(0,0,0), 4.0);
    std::cout << "[GLWindow] Created obstacle at address: " << obstacle << std::endl;

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
    
    m_animate = true;
    m_backgroundColour.set(0.6f, 0.6f, 0.6f, 1.0f);
    
    m_boidRenderer = std::make_unique<BoidRenderer>();
    m_obstacleRenderer = std::make_unique<ObstacleRenderer>();
}

// ...existing code...
GLWindow::~GLWindow()
{
    delete m_light;
}

int GLWindow::getCurrentBoidSize()
{
    return flock->getFlockSize();
}

GLWindow::BoidStats GLWindow::getBoidStats() const
{
    BoidStats stats;
    if (!flock) {
        return stats;
    }

    const std::vector<Boid*>& boids = flock->getBoidList();
    stats.count = static_cast<int>(boids.size());
    if (boids.empty()) {
        return stats;
    }

    glm::vec3 minV(std::numeric_limits<float>::infinity());
    glm::vec3 maxV(-std::numeric_limits<float>::infinity());
    glm::vec3 sum(0.0f);
    int validCount = 0;

    for (const Boid* b : boids) {
        if (!b) continue;
        Vector p = b->getPosition();
        if (std::isnan(p.m_x) || std::isnan(p.m_y) || std::isnan(p.m_z)) {
            stats.nanCount++;
            continue;
        }
        glm::vec3 gp(p.m_x, p.m_y, p.m_z);
        minV = glm::min(minV, gp);
        maxV = glm::max(maxV, gp);
        sum += gp;
        validCount++;
    }

    if (validCount > 0) {
        stats.min = minV;
        stats.max = maxV;
        stats.avg = sum / static_cast<float>(validCount);
    }

    return stats;
}

bool GLWindow::isGPUModeEnabled() const
{
    return m_gpuFlockingManager && m_gpuFlockingManager->isEnabled();
}

int GLWindow::getThreadCount() const
{
    return m_boidRenderer ? m_boidRenderer->getNumThreads() : 1;
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

void GLWindow::setBoidColor(const glm::vec3& colour)
{
    Colour colourToSet;
    colourToSet.set(colour.r, colour.g, colour.b);
    if (flock) {
        flock->setColour(colourToSet);
    }
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
    if (obstacle) {
        obstacle->setPositionModern(flock::Vec3(position.x, position.y, position.z));
    }
    m_needsRedraw = true;
}

void GLWindow::setObstacleSize(double size)
{
    if (obstacle) {
        obstacle->setRadiusModern(static_cast<float>(size));
    } else {
        m_pendingObstacleSize = size;
        m_hasPendingObstacleSize = true;
    }
    m_needsRedraw = true;
}

void GLWindow::setObstacleColour(const glm::vec3& colour)
{
    std::cout << "[GLWindow] setObstacleColour: this->obstacle=" << obstacle << std::endl;
    if (obstacle) {
        obstacle->setColorModern(flock::Color(colour.r, colour.g, colour.b, 1.0f));
    }
    m_needsRedraw = true;
}

void GLWindow::setObstacleWireframe(bool value)
{
    if (obstacle) {
        obstacle->setWireframe(value);
    }
    m_needsRedraw = true;
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
    m_needsRedraw = true;
}

void GLWindow::setBBoxSize(glm::vec3 size)
{
    delete bbox;
    bbox = new BBox(Vector(0,0,0), size.x, size.y, size.z);
    bbox->setDrawMode(GL_LINE);
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initialize(int width, int height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        return;
    }

    glViewport(0, 0, width, height);
    
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    glEnable(GL_DEPTH_TEST);
    
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "glad initialized successfully with OpenGL " << major << "." << minor << std::endl;
    
    const GLubyte* renderer = glGetString(GL_RENDERER);
    if (renderer)
        m_gpuName = reinterpret_cast<const char*>(renderer);
    else
        m_gpuName = "Unknown GPU";
    
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
    float aspect = (height > 0) ? static_cast<float>(width) / static_cast<float>(height) : 16.0f / 9.0f;
    m_cam->setShape(45, aspect,0.05,2000,PERSPECTIVE);
    
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
    } else {
        // Start in CPU mode; users can toggle GPU with 'G' if desired
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
}
//----------------------------------------------------------------------------------------------------------------------
// Handle viewport resize
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::resize(int _w, int _h)
{
    m_viewportWidth = _w;
    m_viewportHeight = _h;
    glViewport(0,0,_w,_h);
    
    if (m_cam) {
        m_cam->setShape(45,(float)_w/_h,0.05,2000,PERSPECTIVE);
    }
    m_needsRedraw = true;
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
void GLWindow::render()
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
    
    FlockingGraphics::FrameCoordinator::getInstance().endFrame();
}

void GLWindow::onMouseMove(double x, double y, bool leftDown, bool rightDown, bool middleDown, bool shift, bool ctrl)
{
    m_shiftDown = shift;
    m_ctrlDown = ctrl;
    m_leftMouseDown = leftDown;
    m_rightMouseDown = rightDown;
    m_middleMouseDown = middleDown;

    // Obstacle dragging has priority
    if (m_obstacleSelected && leftDown) {
        if (shift) {
            m_obstacleDragPlane = ObstacleDragPlane::XZ;
        } else if (ctrl) {
            m_obstacleDragPlane = ObstacleDragPlane::YZ;
        } else {
            m_obstacleDragPlane = ObstacleDragPlane::XY;
        }
        float nx = 2.0f * static_cast<float>(x) / static_cast<float>(m_viewportWidth) - 1.0f;
        float ny = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(m_viewportHeight);
        glm::vec4 rayClip(nx, ny, -1.0f, 1.0f);
        glm::mat4 proj = m_cam->getProjectionMatrix();
        glm::mat4 view = m_cam->getViewMatrix();
        glm::vec4 rayEye = glm::inverse(proj) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
        glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
        glm::vec3 camPos = m_cam->getEye();
        glm::vec3 hitPoint{std::numeric_limits<float>::quiet_NaN()};
        if (m_obstacleDragPlane == ObstacleDragPlane::XY) {
            float denom = rayWorld.z;
            if (fabs(denom) > 1e-6f) {
                float t = (m_obstacleDragPlaneZ - camPos.z) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        } else if (m_obstacleDragPlane == ObstacleDragPlane::XZ) {
            float denom = rayWorld.y;
            if (fabs(denom) > 1e-6f) {
                float t = (m_obstacleDragPlaneZ - camPos.y) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        } else if (m_obstacleDragPlane == ObstacleDragPlane::YZ) {
            float denom = rayWorld.x;
            if (fabs(denom) > 1e-6f) {
                float t = (m_obstacleDragPlaneZ - camPos.x) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        }
        if (!std::isnan(hitPoint.x) && !std::isnan(hitPoint.y) && !std::isnan(hitPoint.z)) {
            obstacle->setSpherePosition(Vector(hitPoint.x, hitPoint.y, hitPoint.z));
            m_needsRedraw = true;
        }
        m_lastMousePos = glm::vec2(x, y);
        return;
    }

    if(m_rotate && leftDown)
    {
        int diffx=static_cast<int>(x)-m_origX;
        int diffy=static_cast<int>(y)-m_origY;
        
        m_cameraAzimuth += diffx * 0.5f;
        m_cameraElevation += diffy * 0.5f;
        
        if (m_cameraElevation > 85.0f) m_cameraElevation = 85.0f;
        if (m_cameraElevation < -85.0f) m_cameraElevation = -85.0f;
        
        if (m_cameraAzimuth > 360.0f) m_cameraAzimuth -= 360.0f;
        if (m_cameraAzimuth < 0.0f) m_cameraAzimuth += 360.0f;
        
        updateCameraPosition();
        
        m_origX = static_cast<int>(x);
        m_origY = static_cast<int>(y);
        m_needsRedraw = true;
    }
    else if(m_translate && rightDown)
    {
        int diffX = static_cast<int>(x) - m_origXPos;
        int diffY = static_cast<int>(y) - m_origYPos;
        
        float panSpeed = 0.1f;
        m_cameraTarget.m_x += panSpeed * diffX;
        m_cameraTarget.m_y -= panSpeed * diffY;
        
        updateCameraPosition();
        
        m_origXPos=static_cast<int>(x);
        m_origYPos=static_cast<int>(y);
        m_needsRedraw = true;
    }
    else if(m_pan && middleDown)
    {
        int diffX = static_cast<int>(x) - m_origXPos;
        int diffY = static_cast<int>(y) - m_origYPos;
        
        float panSpeed = 0.1f;
        m_cameraTarget.m_x += panSpeed * diffX;
        m_cameraTarget.m_y -= panSpeed * diffY;
        
        updateCameraPosition();
        
        m_origXPos=static_cast<int>(x);
        m_origYPos=static_cast<int>(y);
        m_needsRedraw = true;
    }

    m_lastMousePos = glm::vec2(x, y);
}

void GLWindow::onMouseButton(double x, double y, int button, int action, int mods)
{
    m_shiftDown = (mods & GLFW_MOD_SHIFT) != 0;
    m_ctrlDown = (mods & GLFW_MOD_CONTROL) != 0;
    bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        m_leftMouseDown = pressed;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        m_rightMouseDown = pressed;
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        m_middleMouseDown = pressed;
    }

    if (pressed && button == GLFW_MOUSE_BUTTON_LEFT) {
        float nx = 2.0f * static_cast<float>(x) / static_cast<float>(m_viewportWidth) - 1.0f;
        float ny = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(m_viewportHeight);
        glm::vec4 rayClip(nx, ny, -1.0f, 1.0f);
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
            m_lastMousePos = glm::vec2(x, y);
            glm::vec3 hitPoint = camPos + tHit * rayWorld;
            m_obstacleDragStartWorld = Vector(hitPoint.x, hitPoint.y, hitPoint.z);
            if (m_shiftDown) {
                m_obstacleDragPlane = ObstacleDragPlane::XZ;
                m_obstacleDragPlaneZ = hitPoint.y;
            } else if (m_ctrlDown) {
                m_obstacleDragPlane = ObstacleDragPlane::YZ;
                m_obstacleDragPlaneZ = hitPoint.x;
            } else {
                m_obstacleDragPlane = ObstacleDragPlane::XY;
                m_obstacleDragPlaneZ = hitPoint.z;
            }
            return;
        }
        m_origX = static_cast<int>(x);
        m_origY = static_cast<int>(y);
        m_rotate = true;
    }
    else if(pressed && button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        m_origXPos = static_cast<int>(x);
        m_origYPos = static_cast<int>(y);
        m_translate = true;
    }
    else if(pressed && button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        m_origXPos = static_cast<int>(x);
        m_origYPos = static_cast<int>(y);
        m_pan = true;
    }

    if (!pressed) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            m_rotate = false;
            if (m_obstacleSelected) {
                m_obstacleSelected = false;
            }
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            m_translate = false;
        } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            m_pan = false;
        }
    }

    m_lastMousePos = glm::vec2(x, y);
}

void GLWindow::onScroll(double /*xoffset*/, double yoffset)
{
    float zoomFactor = 10.0f;
    
    if(yoffset > 0)
    {
        m_cameraDistance -= zoomFactor;
        if (m_cameraDistance < 5.0f) m_cameraDistance = 5.0f;
    }
    else if(yoffset < 0 )
    {
        m_cameraDistance += zoomFactor;
        if (m_cameraDistance > 1500.0f) m_cameraDistance = 1500.0f;
    }
    
    updateCameraPosition();
    m_needsRedraw = true;
}

void GLWindow::updateSimulation(float deltaTime)
{
    if (!m_animate) {
        return;
    }

    PROFILE_SCOPE("Total Frame Update");
    {
        PROFILE_SCOPE("Flock Update");
        float dt = deltaTime > 0.0f ? deltaTime : 0.016f;
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
                gpuBoid.color = glm::vec4(colorModern.r, colorModern.g, colorModern.b, 1.0f);
                gpuBoidData.push_back(gpuBoid);
            }
            
            FlockingGraphics::FlockingParameters params;
            // Align GPU parameters to the current CPU integrator weights
            const float behaviourDist = static_cast<float>(flock->getBehaviours()->getBehaviourDistance());
            const float flockDist = static_cast<float>(flock->getBehaviours()->getFlockDistance());
            params.separationDistance = flockDist;
            params.alignmentDistance = behaviourDist;
            params.cohesionDistance = behaviourDist;
            params.separationForce = static_cast<float>(flock->getBehaviours()->getSeparationForce()) * 1.4f;
            params.cohesionForce = static_cast<float>(flock->getBehaviours()->getCohesionForce()) * 0.35f;
            params.alignmentForce = static_cast<float>(flock->getBehaviours()->getAlignment());
            // Match CPU speed clamp (Boid max velocity)
            float maxV = 0.9f;
            if (!boidList.empty()) {
                maxV = boidList.front()->getMaxVelocity();
            }
            params.maxSpeed = maxV;
            params.maxForce = 1.5f; // CPU accel clamp
            params.numBoids = static_cast<int>(boidList.size());
            params.deltaTime = dt;
            params.speedMultiplier = flock->getSpeedMultiplier();
            
            if (bbox) {
                Vector center = bbox->getCenter();
                float width = bbox->getWidth() / 2.0f;
                float height = bbox->getHeight() / 2.0f;
                float depth = bbox->getDepth() / 2.0f;
                params.boundingBoxMin = glm::vec3(center.m_x - width, center.m_y - height, center.m_z - depth);
                params.boundingBoxMax = glm::vec3(center.m_x + width, center.m_y + height, center.m_z + depth);
            }
            
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
        } else {
            flock->update();
        }
    }

    m_needsRedraw = true;
}

void GLWindow::onKey(int key, int action)
{
    if (action != GLFW_PRESS) {
        return;
    }

    switch (key) {
        case GLFW_KEY_P:
            printPerformanceComparison();
            break;
        case GLFW_KEY_R:
            PerformanceProfiler::getInstance().printReport();
            break;
        case GLFW_KEY_T:
            PerformanceProfiler::getInstance().reset();
            std::cout << "Performance profiler statistics reset." << std::endl;
            break;
        case GLFW_KEY_M:
            setPerformanceMonitoring(!m_performanceMonitor.isEnabled());
            break;
        case GLFW_KEY_C:
            m_performanceMonitor.clear();
            break;
        case GLFW_KEY_SPACE:
            m_animate = !m_animate;
            break;
        case GLFW_KEY_V:
            validateBehaviorDifferences();
            break;
        case GLFW_KEY_G:
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
        case GLFW_KEY_KP_ADD:
        case GLFW_KEY_EQUAL:
            if (flock) {
                for (int i = 0; i < 5; i++) {
                    flock->addBoids();
                }
                std::cout << "Added boids. Total: " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_KP_SUBTRACT:
        case GLFW_KEY_MINUS:
            if (flock) {
                for (int i = 0; i < 5 && flock->getFlockSize() > 10; i++) {
                    flock->removeBoids();
                }
                std::cout << "Removed boids. Total: " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_1:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 500;
                std::cout << "Setting flock size from " << currentSize << " to " << target << std::endl;
                
                if (currentSize < target) {
                    while (flock->getFlockSize() < target) {
                        flock->addBoids();
                    }
                } else if (currentSize > target) {
                    while (flock->getFlockSize() > target) {
                        flock->removeBoids();
                    }
                }
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_2:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 1000;
                std::cout << "Setting flock size from " << currentSize << " to " << target << std::endl;
                
                if (currentSize < target) {
                    while (flock->getFlockSize() < target) {
                        flock->addBoids();
                    }
                } else if (currentSize > target) {
                    while (flock->getFlockSize() > target) {
                        flock->removeBoids();
                    }
                }
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_3:
            if (flock) {
                int target = 2000;
                std::cout << "Setting flock size to " << target << " - TARGET!" << std::endl;
                flock->setFlockSize(target);
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_4:
            if (flock) {
                int target = 4000;
                std::cout << "Setting flock size to " << target << " - MASSIVE!" << std::endl;
                flock->setFlockSize(target);
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_0:
            if (flock) {
                int target = 200;
                std::cout << "Resetting flock size to " << target << " (default)" << std::endl;
                
                while (flock->getFlockSize() < target) {
                    flock->addBoids();
                }
                while (flock->getFlockSize() > target) {
                    flock->removeBoids();
                }
                std::cout << "Flock size reset to " << flock->getFlockSize() << std::endl;
            }
            break;
        default:
            break;
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
    m_needsRedraw = true;
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
    m_needsRedraw = true;
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

void GLWindow::setObstacleEnabled(bool _enabled)
{
    m_obstacleEnabled = _enabled;
    // Also control the collision checking in the flock
    setObstacleCollisionEnabled(_enabled);
    m_needsRedraw = true;
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

