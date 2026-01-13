// GLWindow.cpp
// Drives the app lifetime: initializes GL/ImGui, wires input callbacks, boots the simulation state, and updates everything each frame.

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
    obstacle = std::make_unique<Obstacle>(Vector(0,0,0), 4.0);
    std::cout << "[GLWindow] Created obstacle at address: " << obstacle.get() << std::endl;

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
    // Unique pointers clean up owned resources.
}

int GLWindow::getCurrentBoidSize()
{
    return flock ? flock->getFlockSize() : 0;
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
    std::cout << "[GLWindow] setObstaclePosition: this->obstacle=" << obstacle.get() << std::endl;
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
    std::cout << "[GLWindow] setObstacleColour: this->obstacle=" << obstacle.get() << std::endl;
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
    bbox = std::make_unique<BBox>(Vector(0,0,0), size.x, size.y, size.z);
    bbox->setDrawMode(GL_LINE);
    if (flock) {
        flock->setBoundingBox(bbox.get());
    }
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
    m_shader->bindUniformBlockToBindingPoint("Phong", "LightingBlock", FlockingShaders::LIGHTING_BINDING_POINT);
    
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
    m_shader->bindUniformBlockToBindingPoint("boid_shader", "LightingBlock", FlockingShaders::LIGHTING_BINDING_POINT);
    
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
    m_shader->bindUniformBlockToBindingPoint("obstacle_shader", "LightingBlock", FlockingShaders::LIGHTING_BINDING_POINT);
    
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
    m_shader->bindUniformBlockToBindingPoint("PhongInstanced", "LightingBlock", FlockingShaders::LIGHTING_BINDING_POINT);
    
    Material m(GOLD);
    
    // Create camera using orbital controls
    Vector From(200,120,120);
    Vector To(0,0,0);
    Vector Up(0,1,0);
    m_cam = std::make_unique<Camera>(From,To,Up,PERSPECTIVE);
    float aspect = (height > 0) ? static_cast<float>(width) / static_cast<float>(height) : 16.0f / 9.0f;
    m_cam->setShape(45, aspect,0.05,2000,PERSPECTIVE);
    
    updateCameraPosition();
    
    Matrix iv=m_cam->getViewMatrix();
    iv.transpose();
    m_light = std::make_unique<Light>(Vector(150,200,120,1),Colour(0.8,0.8,0.75,1),Colour(1.0,1.0,0.9,1),POINTLIGHT);
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
    bbox = std::make_unique<BBox>(Vector(0,0,0),200,200,200);
    bbox->setDrawMode(GL_LINE);
    flock = std::make_unique<Flock>(bbox.get(), obstacle.get());
    
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

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
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
void GLWindow::setShowFPS(bool show) {
    m_showFPS = show;
}

//----------------------------------------------------------------------------------------------------------------------
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
