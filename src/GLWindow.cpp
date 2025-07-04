#include "obstacle.h"
#include "GLWindow.h"
#include "mainwindow.h"
#include "flock.h"
#include "boid.h"
#include <iostream>
#include <cmath>
#include <QSurfaceFormat>
#include "MathUtils.h"
#include "../modules/graphics/include/Camera.h"
#include "../modules/graphics/include/Colour.h"
#include "../modules/graphics/include/Material.h"
#include "../modules/graphics/include/TransformStack.h"
#include "../modules/graphics/include/Light.h"
#include "../modules/graphics/include/ShaderLib.h"
#include "../modules/graphics/include/UBOStructures.h"
#include "../modules/graphics/include/FrameCoordinator.h"
#include "../modules/graphics/include/GeometryFactory.h"
#include "../modules/graphics/include/SmartShaderManager.h"
#include "../modules/graphics/include/RenderManager.h"
#include "../modules/graphics/include/UBOCache.h"
#include "../modules/graphics/include/TransformBatcher.h"
#include "PerformanceMonitor.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BehaviorValidator.h"
#include "glew_compat.h"
#include "shader_constants.h"
#include "PerformanceProfiler.h"



//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT = 1;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM = 10.0;
//----------------------------------------------------------------------------------------------------------------------
// in this ctor we need to call the CreateCoreGLContext class, this is mainly for the MacOS Lion version as
// we need to init the OpenGL 3.2 sub-system which is different than other platforms
//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(
        QWidget *_parent
        )
    : QOpenGLWidget(_parent)
{
    // Configure OpenGL format to use compatibility profile
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4); // 4x MSAA
    setFormat(format);
    
    obstacle = new Obstacle(Vector(50,0,0), 15.0);  // Move the obstacle far to the right for clear testing

    // set this widget to have the initial keyboard focus
    setFocus();
    // Make sure this widget can accept keyboard focus
    setFocusPolicy(Qt::StrongFocus);
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    this->resize(_parent->size());
    // Now set the initial GLWindow attributes to default values
    // Roate is false
    m_rotate=false;
    m_translate=false;
    m_pan=false;
    // mouse rotation values set to 0
    m_spinXFace = 0;
    m_spinYFace = 0;
    
    // Initialize orbital camera controls
    m_cameraDistance = 200.0f;  // Increased from 120.0f for better view of larger simulation space
    m_cameraAzimuth = 45.0f;    // degrees
    m_cameraElevation = 30.0f;  // degrees  
    m_cameraTarget.set(0, 0, 0); // look at origin
    
    // Initialize FPS tracking
    m_lastTime = std::chrono::high_resolution_clock::now();
    m_frameCount = 0;
    m_currentFPS = 0.0f;
    m_showFPS = true; // Show FPS by default
    
    // Initialize pending size values
    m_pendingBoidSize = 1.0;
    m_pendingObstacleSize = 4.0;
    m_hasPendingBoidSize = false;
    m_hasPendingObstacleSize = false;
    
    // Print profiling instructions
    std::cout << "\n=== PERFORMANCE PROFILER CONTROLS ===" << std::endl;
    std::cout << "Press 'R' - Print detailed performance report" << std::endl;
    std::cout << "Press 'T' - Reset profiler statistics" << std::endl;
    std::cout << "Press 'Space' - Toggle animation" << std::endl;
    std::cout << "Press '+' or '-' - Add/Remove 50 boids" << std::endl;
    std::cout << "Press '0' - Reset to 200 boids (default)" << std::endl;
    std::cout << "Press '1' - Set 500 boids" << std::endl;
    std::cout << "Press '2' - Set 1000 boids" << std::endl;
    std::cout << "Press '3' - Set 2000 boids (TARGET)" << std::endl;
    std::cout << "====================================\n" << std::endl;
    
    m_sphereUpdateTimer = startTimer(1000 / 60); //run at 60FPS
    m_animate = true;
    m_backgroundColour.set(0.6f, 0.6f, 0.6f, 1.0f);
}

//----------------------------------------------------------------------------------------------------------------------
GLWindow::~GLWindow()
{
    delete m_light;
    // Qt handles OpenGL cleanup automatically
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
    Vector nglPos(position.x, position.y, position.z);
    obstacle->setSpherePosition(nglPos);
}

void GLWindow::setObstacleSize(double size)
{
    if (obstacle) {
        obstacle->setSphereRadius(size);
    } else {
        m_pendingObstacleSize = size;
        m_hasPendingObstacleSize = true;
    }
}

void GLWindow::setObstacleColour(QColor colour)
{
    Colour colourToSet;
    colourToSet.set(colour.redF(), colour.greenF(), colour.blueF());

    obstacle->setColour(colourToSet);
}

void GLWindow::setObstacleWireframe(bool value)
{
    obstacle->setWireframe(value);
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
    
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    
    // Initialize OpenGL extensions
#ifdef WIN32
    glewInit(); // Initialize GLEW on Windows
#endif
    // now to load the shader and set the values
    // grab an instance of shader manager
    m_shader = ShaderLib::instance();
    
    // Create shaders for optimized rendering
    
    // Main Phong shader for boids and obstacles
    m_shader->createShaderProgram("Phong");
    m_shader->attachShader("PhongVertex",VERTEX);
    m_shader->attachShader("PhongFragment",FRAGMENT);
    m_shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
    m_shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
    m_shader->compileShader("PhongVertex");
    m_shader->compileShader("PhongFragment");
    m_shader->attachShaderToProgram("Phong","PhongVertex");
    m_shader->attachShaderToProgram("Phong","PhongFragment");
    
    // Bind vertex attributes
    m_shader->bindAttribute("Phong",0,"inVert");
    m_shader->bindAttribute("Phong",1,"inUV");
    m_shader->bindAttribute("Phong",2,"inNormal");
    
    // Link and activate
    m_shader->linkProgramObject("Phong");
    (*m_shader)["Phong"]->use();
    
    // Bind uniform blocks to binding points for UBO compatibility
    m_shader->bindUniformBlockToBindingPoint("Phong", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("Phong", "MaterialBlock", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("Phong", "LightBlock", FlockingShaders::LIGHT_BINDING_POINT);
    
    // Create additional shaders for optimization
    // Instanced boid shader
    m_shader->createShaderProgram("boid_shader");
    m_shader->attachShader("BoidVertex",VERTEX);
    m_shader->attachShader("BoidFragment",FRAGMENT);
    // For now, use the same Phong shaders - can be optimized later with instancing
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
    
    // Obstacle shader  
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
    
    // Instanced Phong shader for high-performance boid rendering
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
    // Instance attributes bound by InstancedBoidRenderer
    m_shader->linkProgramObject("PhongInstanced");
    m_shader->bindUniformBlockToBindingPoint("PhongInstanced", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("PhongInstanced", "MaterialBlock", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("PhongInstanced", "LightingBlock", FlockingShaders::LIGHTING_BINDING_POINT);
    
    // the shader will use the currently active material and light0 so set them
    Material m(GOLD);
    
    // Now we will create a basic Camera from the graphics library
    // Create camera using orbital controls
    Vector From(200,120,120);  // Initial position (will be updated by orbital controls)
    Vector To(0,0,0);
    Vector Up(0,1,0);
    // now load to our new camera
    m_cam= new Camera(From,To,Up,PERSPECTIVE);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.05 and 2000 (increased for large simulations)
    m_cam->setShape(45,(float)720.0/576.0,0.05,2000,PERSPECTIVE);
    
    // Initialize camera position using orbital controls
    updateCameraPosition();
    
    // now create our light this is done after the camera so we can pass the
    // transpose of the projection matrix to the light to do correct eye space
    // transformations
    Matrix iv=m_cam->getViewMatrix();
    iv.transpose();
    m_light = new Light(Vector(120,120,120,1),Colour(1,1,1,1),Colour(1,1,1,1),POINTLIGHT);
    m_light->setTransform(iv);
    
    // Initialize UBOs for modern shader pipeline
    initializeUBOs();
    
    // Load initial material and light data to UBOs
    updateMaterialUBO(m);
    updateLightUBO();
    
    // Initialize optimization systems and standard geometries
    
    // TODO: Initialize optimization systems (temporarily commented for build stability)
    // FlockingGraphics::RenderManager::getInstance().initialize();
    
    // Create standard geometries for optimized rendering (stubbed for now)
    // FlockingGraphics::GeometryFactory::instance().createSphere(1.0f, 16); // Standard boid geometry
    // FlockingGraphics::GeometryFactory::instance().createSphere(1.0f, 12); // Lower-quality sphere for obstacles
    // FlockingGraphics::GeometryFactory::instance().createBoundingBox();    // Wireframe bounding box
    // FlockingGraphics::GeometryFactory::instance().createCube(1.0f);       // Standard cube
    
    // Create named geometries for the rendering system
    // auto boidGeometry = FlockingGraphics::GeometryFactory::instance().createSphere(1.0f, 12);
    // The geometry factory automatically names spheres with the pattern: "sphere_<radius>_<segments>"
    // So our boid geometry will be named "sphere_1.0_12"
    // We can create an alias for easier access
    
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
    
    // Bind UBO blocks to the Colour shader (same as Phong shader)
    m_shader->bindUniformBlockToBindingPoint("Colour", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    
    // Create wireframe shader for optimized bounding box rendering
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
    glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces
    
    // Initialize sphere primitive for boid rendering
    bbox = new BBox(Vector(0,0,0),200,200,200);  // Increased from 120x120x120 for larger simulation space
    bbox->setDrawMode(GL_LINE);
    flock = new Flock(bbox, obstacle);
    
    // Initialize high-performance instanced boid renderer
    m_instancedBoidRenderer = std::make_unique<FlockingGraphics::InstancedBoidRenderer>();
    m_instancedBoidRenderer->initialize(0.5f, 16); // 0.5f radius, 16 segments
    
    // Apply any pending size values that were set before OpenGL initialization
    if (m_hasPendingBoidSize) {
        flock->setBoidSize(m_pendingBoidSize);
        m_hasPendingBoidSize = false;
    }
    if (m_hasPendingObstacleSize) {
        obstacle->setSphereRadius(m_pendingObstacleSize);
        m_hasPendingObstacleSize = false;
    }
    
    // Initialize UI values now that OpenGL is ready
    // Get the parent MainWindow and initialize UI values
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
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::paintGL()
{
    PROFILE_SCOPE("Total Frame Render");
    
    static int frame_count = 0;
    frame_count++;
    
    // Begin frame coordination for all optimization systems
    FlockingGraphics::FrameCoordinator::getInstance().beginFrame();
    
    // Use the background color set by the user
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    
    // clear the screen and depth buffer
    {
        PROFILE_SCOPE("Clear Buffers");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    // Enable depth testing and face culling for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Use the NGL-style camera and transform stack for consistent rendering
    // Set up the view matrix for the camera
    glm::mat4 view = m_cam->getViewMatrix();
    glm::mat4 project = m_cam->getProjectionMatrix();
    
    // Load the matrices into the transform stack (for NGL compatibility)
    Matrix nglProject(project);
    Matrix nglView(view);
    m_transformStack.setProjection(nglProject);
    m_transformStack.setView(nglView);
    
    // Set up basic model matrix (no rotation - camera handles all movement now)
    Matrix model;
    model.identity();
    model.translate(m_modelPos.m_x, m_modelPos.m_y, m_modelPos.m_z);
    m_transformStack.setModel(model);
    
    // Update lighting UBO every frame (light position needs to be in view space)
    updateLightingUBO();

    // Draw the bounding box (wireframe) with white color
    if (bbox) {
        PROFILE_SCOPE("Draw Bounding Box");
        // Push a clean transform for the bounding box to ensure it's drawn at origin
        m_transformStack.pushTransform();
        {
            // Reset to identity transform for bounding box (no translation)
            Matrix bboxTransform;
            bboxTransform.identity();
            m_transformStack.setModel(bboxTransform.getGLMMat4());
            
            // Update UBO with clean transform
            updateMatrixUBO(m_transformStack);
            
            // Use modern color shader for wireframe rendering
            ShaderLib *shader = ShaderLib::instance();
            shader->use("Colour");
            shader->setShaderParam4f("Colour", 1.0f, 1.0f, 1.0f, 1.0f); // White color
            bbox->draw();
        }
        m_transformStack.popTransform();
    }

    // Draw the flock using high-performance instanced rendering
    if (flock && m_instancedBoidRenderer) {
        PROFILE_SCOPE("Draw Instanced Boids");
        // Clear previous instances
        m_instancedBoidRenderer->clearInstances();
        
        // Prepare view and projection matrices for instancing
        m_transformStack.pushTransform();
        
        // Get all boids and add them as instances
        const std::vector<Boid*>& boidList = flock->getBoidList();
        
        // Update base matrices (view/projection only, no model transform)
        Matrix identityMatrix;
        identityMatrix.identity();
        m_transformStack.setModel(identityMatrix.getGLMMat4());
        updateMatrixUBO(m_transformStack);
        
        // Update material UBO once for all boids with a brighter material
        Material boidMaterial;
        boidMaterial.setAmbient(Colour(0.3f, 0.3f, 0.3f, 1.0f));     // Brighter ambient
        boidMaterial.setDiffuse(Colour(0.8f, 0.6f, 0.4f, 1.0f));     // Bright gold diffuse
        boidMaterial.setSpecular(Colour(1.0f, 1.0f, 1.0f, 1.0f));    // Bright specular
        boidMaterial.setShininess(32.0f);                              // Moderate shininess
        updateMaterialUBO(boidMaterial);
        
        // Add each boid as an instance
        for(const Boid* boid : boidList)
        {
            // Calculate model matrix for this boid
            flock::Vec3 boidPos = boid->getPositionModern();
            Vector boidScale = boid->getScale();
            Matrix boidTransform;
            boidTransform.identity();
            boidTransform.translate(boidPos.x, boidPos.y, boidPos.z);
            
            // Use the actual boid scale from the flock (controlled by UI)
            boidTransform.scale(boidScale.m_x, boidScale.m_y, boidScale.m_z);
            
            // Convert to GLM matrix
            glm::mat4 modelMatrix = boidTransform.getGLMMat4();
            
            // Use a brighter color for boids that won't darken the material too much
            glm::vec4 boidColor(1.0f, 1.0f, 1.0f, 1.0f);  // White to preserve material color
            
            // Add this boid as an instance
            m_instancedBoidRenderer->addInstance(modelMatrix, boidColor);
        }
        
        // Render all boids in a single instanced draw call
        m_instancedBoidRenderer->renderInstances("PhongInstanced");
        
        m_transformStack.popTransform();
    }

    // Draw the obstacle with modern UBO-based Phong shader
    if (obstacle && m_obstacleEnabled) {
        PROFILE_SCOPE("Draw Obstacle");
        // Push a new transform level for the obstacle
        m_transformStack.pushTransform();
        {
            // Set up the obstacle's transform (translate to obstacle position and scale by radius)
            flock::Vec3 obstaclePos = obstacle->getPositionModern();
            float obstacleRadius = obstacle->getSphereRadius();
            
            // Debug output for obstacle size
            static int obstacleDebugCount = 0;
            if (obstacleDebugCount++ % 60 == 0) { // Every 60 frames (once per second at 60fps)
                std::cout << "Obstacle radius: " << obstacleRadius << std::endl;
            }
            
            Matrix obstacleTransform;
            obstacleTransform.identity();
            
            obstacleTransform.translate(obstaclePos.x, obstaclePos.y, obstaclePos.z);
            obstacleTransform.scale(obstacleRadius, obstacleRadius, obstacleRadius);
            
            m_transformStack.setModel(obstacleTransform.getGLMMat4());
            
            // Update UBO matrices with obstacle transform
            updateMatrixUBO(m_transformStack);
            
            // Set up obstacle material with balanced properties for smooth standard Phong lighting
            Material obstacleMaterial;
            
            // Enhanced material properties for better specular visibility
            obstacleMaterial.setAmbient(Colour(0.3f, 0.25f, 0.2f, 1.0f));  // Moderate ambient
            obstacleMaterial.setDiffuse(Colour(m_obstacleDiffuseR, m_obstacleDiffuseG, m_obstacleDiffuseB, 1.0f));   // Warm diffuse color
            obstacleMaterial.setSpecular(Colour(m_obstacleSpecularR, m_obstacleSpecularG, m_obstacleSpecularB, 1.0f));  // Bright specular for visibility
            obstacleMaterial.setShininess(8.0f);  // Low shininess for broad, visible highlights
            
            updateMaterialUBO(obstacleMaterial);
            
            // Use the Phong shader for obstacle rendering
            m_shader->use("Phong");
            
            // Render the obstacle using modern VBO/VAO approach
            obstacle->ObsDraw("Phong", m_transformStack, m_cam);
        }
        m_transformStack.popTransform();
    }
    
    // Update and render FPS counter
    updateFPS();
    renderFPSText();
    
    // End frame coordination to finalize rendering
    FlockingGraphics::FrameCoordinator::getInstance().endFrame();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
        QMouseEvent * _event
        )
{
    // Left mouse button - orbital rotation
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
        int diffx=_event->position().x()-m_origX;
        int diffy=_event->position().y()-m_origY;
        
        // Update azimuth and elevation for orbital camera
        m_cameraAzimuth += diffx * 0.5f;
        m_cameraElevation += diffy * 0.5f;
        
        // Clamp elevation to prevent flipping
        if (m_cameraElevation > 85.0f) m_cameraElevation = 85.0f;
        if (m_cameraElevation < -85.0f) m_cameraElevation = -85.0f;
        
        // Wrap azimuth
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
        
        // Pan the camera target
        float panSpeed = 0.1f;
        m_cameraTarget.m_x += panSpeed * diffX;
        m_cameraTarget.m_y -= panSpeed * diffY; // Invert Y for natural panning
        
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
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
        QMouseEvent * _event
        )
{
    // Left mouse button - rotation
    if(_event->button() == Qt::LeftButton)
    {
        m_origX = _event->position().x();
        m_origY = _event->position().y();
        m_rotate = true;
    }
    // Right mouse button - translate mode
    else if(_event->button() == Qt::RightButton)
    {
        m_origXPos = _event->position().x();
        m_origYPos = _event->position().y();
        m_translate = true;
    }
    // Middle mouse button - pan mode
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
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{
    // Zoom by adjusting camera distance
    float zoomFactor = 10.0f;
    
    if(_event->angleDelta().y() > 0)
    {
        // Zoom in - decrease camera distance
        m_cameraDistance -= zoomFactor;
        if (m_cameraDistance < 5.0f) m_cameraDistance = 5.0f; // Minimum distance
    }
    else if(_event->angleDelta().y() < 0 )
    {
        // Zoom out - increase camera distance
        m_cameraDistance += zoomFactor;
        if (m_cameraDistance > 1500.0f) m_cameraDistance = 1500.0f; // Maximum distance (increased for large simulations)
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

        // Use modern GLM-based update with performance monitoring
        {
            PROFILE_SCOPE("Total Frame Update");
            {
                PROFILE_SCOPE("Flock Update");
                flock->update();
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
    // Convert spherical coordinates to Cartesian coordinates
    float radAzimuth = glm::radians(m_cameraAzimuth);
    float radElevation = glm::radians(m_cameraElevation);
    
    // Calculate camera position relative to target
    float x = m_cameraDistance * cos(radElevation) * cos(radAzimuth);
    float y = m_cameraDistance * sin(radElevation);
    float z = m_cameraDistance * cos(radElevation) * sin(radAzimuth);
    
    Vector cameraPos(m_cameraTarget.m_x + x, m_cameraTarget.m_y + y, m_cameraTarget.m_z + z);
    Vector up(0, 1, 0);
    
    // Update camera
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
            // Print performance comparison
            printPerformanceComparison();
            break;
        case Qt::Key_R:
            // Print performance profiler report
            PerformanceProfiler::getInstance().printReport();
            break;
        case Qt::Key_T:
            // Reset performance profiler statistics
            PerformanceProfiler::getInstance().reset();
            std::cout << "Performance profiler statistics reset." << std::endl;
            break;
        case Qt::Key_M:
            // Toggle performance monitoring
            setPerformanceMonitoring(!m_performanceMonitor.isEnabled());
            break;
        case Qt::Key_C:
            // Clear performance data
            m_performanceMonitor.clear();
            break;
        case Qt::Key_Space:
            // Toggle animation
            m_animate = !m_animate;
            break;
        case Qt::Key_V:
            // Validate behavior differences
            validateBehaviorDifferences();
            break;
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            // Add more boids for testing
            if (flock) {
                for (int i = 0; i < 5; i++) {  // Add 5 sets of 10 = 50 boids
                    flock->addBoids();
                }
                std::cout << "Added boids. Total: " << flock->getFlockSize() << std::endl;
            }
            break;
        case Qt::Key_Minus:
            // Remove boids for testing
            if (flock) {
                for (int i = 0; i < 5 && flock->getFlockSize() > 10; i++) {  // Remove 5 sets of 10 = 50 boids
                    flock->removeBoids();
                }
                std::cout << "Removed boids. Total: " << flock->getFlockSize() << std::endl;
            }
            break;
        case Qt::Key_1:
            // Set to 500 boids
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
            // Set to 1000 boids
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
            // Set to 2000 boids
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 2000;
                std::cout << "Setting flock size from " << currentSize << " to " << target << " - TARGET!" << std::endl;
                
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
        case Qt::Key_0:
            // Reset to default 200 boids
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
    // Position light above and to the side of the flock for good visibility
    glm::vec3 lightWorldPos = glm::vec3(80.0f, 80.0f, 80.0f);
    glm::mat4 viewMatrix = m_cam->getViewMatrix();
    glm::vec4 lightViewPos = viewMatrix * glm::vec4(lightWorldPos, 1.0f);
    lighting.lightPos = glm::vec3(lightViewPos); // Light position in view space
    
    lighting.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
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
    
    // Save current OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    // Disable depth testing for 2D overlay
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    // Switch to 2D rendering mode
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width(), 0, height(), -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Enable blending for semi-transparent background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // For now, we'll print to console more frequently for debugging
    static int printCounter = 0;
    if (printCounter++ % 30 == 0) { // Print every 30 frames (every 0.5 seconds at 60fps)
        qDebug() << "FPS:" << m_currentFPS << "| Boids:" << (flock ? flock->getFlockSize() : 0);
    }
    
    // Print profiling report every 5 seconds (300 frames at 60fps)
    static int profilingCounter = 0;
    if (profilingCounter++ % 300 == 0) {
        std::cout << "\n=== AUTOMATIC PROFILING REPORT ===" << std::endl;
        PerformanceProfiler::getInstance().printReport();
        std::cout << "===================================\n" << std::endl;
    }
    
    // Draw a large, prominent FPS indicator bar
    float barWidth = (m_currentFPS / 60.0f) * 150.0f; // Normalize to 60 FPS, make it wider
    if (barWidth > 150.0f) barWidth = 150.0f;
    
    // Large FPS bar background
    glColor4f(0.0f, 0.0f, 0.0f, 0.9f); // More opaque background
    glBegin(GL_QUADS);
        glVertex2f(10, height() - 40);
        glVertex2f(170, height() - 40);
        glVertex2f(170, height() - 20);
        glVertex2f(10, height() - 20);
    glEnd();
    
    // FPS bar foreground (color-coded) - larger and brighter
    if (m_currentFPS >= 50.0f) {
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // Bright green for good FPS
    } else if (m_currentFPS >= 30.0f) {
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f); // Bright yellow for medium FPS
    } else {
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // Bright red for low FPS
    }
    
    glBegin(GL_QUADS);
        glVertex2f(12, height() - 38);
        glVertex2f(12 + barWidth, height() - 38);
        glVertex2f(12 + barWidth, height() - 22);
        glVertex2f(12, height() - 22);
    glEnd();
    
    // Add a white border for better visibility
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(10, height() - 40);
        glVertex2f(170, height() - 40);
        glVertex2f(170, height() - 20);
        glVertex2f(10, height() - 20);
    glEnd();
    
    // Restore OpenGL state
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glPopAttrib();
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
        
        // Calculate text metrics
        QFontMetrics fm(font);
        int textWidth = qMax(fm.horizontalAdvance(fpsText), fm.horizontalAdvance(boidText));
        int textHeight = fm.height();
        
        // Draw semi-transparent background
        int padding = 8;
        int bgWidth = textWidth + 2 * padding;
        int bgHeight = 2 * textHeight + 3 * padding;
        
        QRect bgRect(10, 10, bgWidth, bgHeight);
        painter.fillRect(bgRect, QColor(0, 0, 0, 180));
        
        // Draw border
        painter.setPen(QPen(Qt::white, 2));
        painter.drawRect(bgRect);
        
        // Draw FPS text
        QColor textColor = Qt::white;
        if (m_currentFPS >= 50.0f) {
            textColor = Qt::green;
        } else if (m_currentFPS >= 30.0f) {
            textColor = Qt::yellow;
        } else {
            textColor = Qt::red;
        }
        
        painter.setPen(textColor);
        painter.drawText(10 + padding, 10 + padding + textHeight, fpsText);
        
        // Draw boid count
        painter.setPen(Qt::white);
        painter.drawText(10 + padding, 10 + 2 * padding + 2 * textHeight, boidText);
        
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

