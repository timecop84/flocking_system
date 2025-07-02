#include "obstacle.h"
#include "GLWindow.h"
#include "flock.h"
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
#include "PerformanceMonitor.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BehaviorValidator.h"
#include "glew_compat.h"
#include "shader_constants.h"



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
    m_cameraDistance = 120.0f;  // Reduced from 250.0f for better view of the simulation
    m_cameraAzimuth = 45.0f;    // degrees
    m_cameraElevation = 30.0f;  // degrees  
    m_cameraTarget.set(0, 0, 0); // look at origin
    
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
    flock->setBoidSize(size);
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
    obstacle->setSphereRadius(size);
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
    std::cout << "initializeGL called" << std::endl;
    
    // Initialize OpenGL functions - required for QOpenGLWidget with QOpenGLFunctions
    initializeOpenGLFunctions();
    
    std::cout << "OpenGL functions initialized" << std::endl;
    
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    
    // Initialize OpenGL extensions
    std::cout << "OpenGL functions initialized" << std::endl;
#ifdef WIN32
    glewInit(); // Initialize GLEW on Windows
#endif
    // now to load the shader and set the values
    // grab an instance of shader manager
    m_shader = ShaderLib::instance();
    // we are creating a shader called Phong
    m_shader->createShaderProgram("Phong");
    // now we are going to create empty shaders for Frag and Vert
    m_shader->attachShader("PhongVertex",VERTEX);
    m_shader->attachShader("PhongFragment",FRAGMENT);
    // attach the source
    m_shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
    m_shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
    // compile the shaders
    m_shader->compileShader("PhongVertex");
    m_shader->compileShader("PhongFragment");
    // add them to the program
    m_shader->attachShaderToProgram("Phong","PhongVertex");
    m_shader->attachShaderToProgram("Phong","PhongFragment");
    // now bind the shader attributes for most NGL primitives we use the following
    // layout attribute 0 is the vertex data (x,y,z)
    m_shader->bindAttribute("Phong",0,"inVert");
    // attribute 1 is the UV data u,v (if present)
    m_shader->bindAttribute("Phong",1,"inUV");
    // attribute 2 are the normals x,y,z
    m_shader->bindAttribute("Phong",2,"inNormal");

    // now we have associated this data we can link the shader
    m_shader->linkProgramObject("Phong");
    // and make it active ready to load values
    (*m_shader)["Phong"]->use();
    
    // Bind uniform blocks to binding points for UBO compatibility
    m_shader->bindUniformBlockToBindingPoint("Phong", "MatrixBlock", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("Phong", "MaterialBlock", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUniformBlockToBindingPoint("Phong", "LightBlock", FlockingShaders::LIGHT_BINDING_POINT);
    
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
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam->setShape(45,(float)720.0/576.0,0.05,350,PERSPECTIVE);
    
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
    
    (*m_shader)["Colour"]->use();
    m_shader->setShaderParam4f("Colour",1,1,1,1);
    glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces
    
    // Initialize sphere primitive for boid rendering
    std::cout << "Initializing sphere primitive for boid rendering" << std::endl;
    bbox = new BBox(Vector(0,0,0),120,120,120);
    bbox->setDrawMode(GL_LINE);
    flock = new Flock(bbox, obstacle);

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
    std::cout << "resizeGL called: " << _w << "x" << _h << std::endl;
    
    // set the viewport for openGL
    glViewport(0,0,_w,_h);
    
    // Set up projection matrix using legacy OpenGL calls
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Simple perspective projection
    float aspect = (float)_w / (float)_h;
    float fovy = 45.0f;
    float near_plane = 0.1f;
    float far_plane = 1000.0f;
    
    // Use gluPerspective equivalent
    float fH = tan(fovy * 3.14159f / 360.0f) * near_plane;
    float fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, near_plane, far_plane);
    
    // now set the camera size values as the screen size has changed
    if (m_cam) {
        m_cam->setShape(45,(float)_w/_h,0.05,350,PERSPECTIVE);
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
    static int frame_count = 0;
    frame_count++;
    
    // Only print debug info every 60 frames to reduce spam
    if (frame_count % 60 == 1) {
        std::cout << "paintGL called (frame " << frame_count << ")" << std::endl;
        std::cout << "Camera pos: (" << m_cam->getEye().x << ", " << m_cam->getEye().y << ", " << m_cam->getEye().z << ")" << std::endl;
    }
    
    // Use the background color set by the user
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Enable depth testing and face culling for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Use the NGL-style camera and transform stack for consistent rendering
    // Set up the view matrix for the camera
    glm::mat4 view = m_cam->getViewMatrix();
    glm::mat4 project = m_cam->getProjectionMatrix();
    
    // Update lighting UBO with current camera position
    updateLightingUBO();
    
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

    // Debug: Add immediate mode test AFTER setting up matrices
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(project));
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));
    glMultMatrixf(glm::value_ptr(model.getGLMMat4()));

    // Set up lighting for immediate mode rendering
    // Position the light in world space
    GLfloat lightPos[] = {120.0f, 120.0f, 120.0f, 1.0f}; // Positional light
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};   // Ambient light
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};   // Diffuse light
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Specular light
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    // Enable lighting (will be disabled/enabled per object as needed)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // Normalize normals automatically
    glShadeModel(GL_SMOOTH); // Smooth shading
    
    // Temporarily disable lighting for axes
    glDisable(GL_LIGHTING);

    // Test: Draw coordinate axes for reference (smaller now that we know rendering works)
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        // X axis - Red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(30.0f, 0.0f, 0.0f);
        
        // Y axis - Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 30.0f, 0.0f);
        
        // Z axis - Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 30.0f);
    glEnd();
    glLineWidth(1.0f);

    // Draw the bounding box (wireframe) with white color
    if (bbox && frame_count % 60 == 1) {
        std::cout << "Drawing bbox" << std::endl;
    }
    if (bbox) {
        glColor3f(1.0f, 1.0f, 1.0f); // White color
        bbox->draw();
    }

    // Draw the flock with Phong shader
    if (flock && frame_count % 60 == 1) {
        std::cout << "Drawing flock" << std::endl;
    }
    if (flock) {
        // Update UBO matrices before drawing flock
        updateMatrixUBO(m_transformStack);
        flock->draw("Phong", m_transformStack, m_cam);
    }

    // Draw the obstacle with modern UBO-based Phong shader
    if (obstacle && frame_count % 60 == 1) {
        std::cout << "Drawing obstacle using modern UBO pipeline" << std::endl;
        flock::Vec3 obstaclePos = obstacle->getPositionModern();
        std::cout << "Obstacle position: (" << obstaclePos.x << ", " << obstaclePos.y << ", " << obstaclePos.z << ")" << std::endl;
        std::cout << "Obstacle radius: " << obstacle->getRadiusModern() << std::endl;
    }
    if (obstacle) {
        // Push a new transform level for the obstacle
        m_transformStack.pushTransform();
        {
            // Get the obstacle's absolute world position
            flock::Vec3 obstaclePos = obstacle->getPositionModern();
            
            // Create a fresh identity matrix for the model transform
            
            // Create a fresh world-space model matrix
            glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix
            
            // Apply translation to position the obstacle in world space
            // This positions the obstacle at its absolute world coordinates
            modelMatrix = glm::translate(
                modelMatrix,
                glm::vec3(obstaclePos.x, obstaclePos.y, obstaclePos.z)
            );
            
            // Debug output to verify the transformation
            std::cout << "Obstacle absolute world position: (" 
                     << obstaclePos.x << ", " << obstaclePos.y << ", " << obstaclePos.z << ")" << std::endl;
                     
            // Show the translation component of the matrix
            std::cout << "Model matrix translation components: [" 
                     << modelMatrix[3][0] << ", " << modelMatrix[3][1] << ", " << modelMatrix[3][2] << "]" << std::endl;
            
            // Apply the model matrix directly to the transform stack
            // This ensures object-to-world transformation is correct
            m_transformStack.setModel(modelMatrix);
            
            // Update UBO matrices with obstacle transform
            updateMatrixUBO(m_transformStack);
            
            // Set up obstacle material with balanced properties for smooth standard Phong lighting
            Material obstacleMaterial;
            
            // Match the boids' material properties exactly
            obstacleMaterial.setAmbient(glm::vec3(m_colour.m_r * 0.3f, m_colour.m_g * 0.3f, m_colour.m_b * 0.3f));  // 0.3 ambient factor
            obstacleMaterial.setDiffuse(glm::vec3(m_colour.m_r, m_colour.m_g, m_colour.m_b));     // Full color for diffuse
            obstacleMaterial.setSpecular(glm::vec3(0.8f, 0.8f, 0.8f));  // Match boids' specular
            obstacleMaterial.setShininess(64.0f);  // Match boids' shininess
            
            updateMaterialUBO(obstacleMaterial);
            
            // Use the Phong shader for obstacle rendering
            m_shader->use("Phong");
            
            // Render the obstacle using modern VBO/VAO approach
            obstacle->ObsDraw("Phong", m_transformStack, m_cam);
            
            // Note: No explicit shader unuse needed as next render call will use a different shader or disable shaders
        }
        m_transformStack.popTransform();
    }
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
        if (m_cameraDistance > 1000.0f) m_cameraDistance = 1000.0f; // Maximum distance
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
            FLOCK_TIMER(m_performanceMonitor, "Modern GLM Update");
            flock->update();
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
    
    // Calculate camera position in spherical coordinates
    // This ensures proper orbit around the target
    float x = m_cameraDistance * cos(radElevation) * cos(radAzimuth);
    float y = m_cameraDistance * sin(radElevation);
    float z = m_cameraDistance * cos(radElevation) * sin(radAzimuth);
    
    // Calculate the camera position in world space
    glm::vec3 cameraPos(
        m_cameraTarget.m_x + x, 
        m_cameraTarget.m_y + y, 
        m_cameraTarget.m_z + z
    );
    
    // Set up view vector (from camera to target)
    glm::vec3 target(m_cameraTarget.m_x, m_cameraTarget.m_y, m_cameraTarget.m_z);
    glm::vec3 up(0.0f, 1.0f, 0.0f); // World up vector
    
    // Update the camera with explicit lookAt
    m_cam->lookAt(cameraPos, target, up);
    
    // Debug output (every 60 frames)
    static int frameCount = 0;
    if (frameCount++ % 60 == 0) {
        std::cout << "Camera position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
        std::cout << "Camera target: (" << target.x << ", " << target.y << ", " << target.z << ")" << std::endl;
    }
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
    if (enabled) {
        std::cout << "Performance monitoring ENABLED" << std::endl;
    } else {
        std::cout << "Performance monitoring DISABLED" << std::endl;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::keyPressEvent(QKeyEvent *_event)
{
    switch (_event->key()) {
        case Qt::Key_P:
            // Print performance comparison
            printPerformanceComparison();
            break;
        case Qt::Key_M:
            // Toggle performance monitoring
            setPerformanceMonitoring(!m_performanceMonitor.isEnabled());
            break;
        case Qt::Key_C:
            // Clear performance data
            m_performanceMonitor.clear();
            std::cout << "Performance data cleared" << std::endl;
            break;
        case Qt::Key_Space:
            // Toggle animation
            m_animate = !m_animate;
            std::cout << "Animation " << (m_animate ? "ENABLED" : "DISABLED") << std::endl;
            break;
        case Qt::Key_V:
            // Validate behavior differences
            validateBehaviorDifferences();
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
        std::cout << "No flock available for validation" << std::endl;
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
    std::cout << "Initializing UBOs for modern shader pipeline..." << std::endl;
    
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
    
    std::cout << "UBOs initialized successfully" << std::endl;
}

void GLWindow::setupLightingUBO() {
    FlockingShaders::LightingBlock lighting;
    
    // Set default light position and color
    lighting.lightPos = glm::vec3(50.0f, 50.0f, 50.0f);
    lighting.viewPos = glm::vec3(0.0f, 0.0f, 0.0f);  // Will be updated in updateLightUBO
    lighting.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    lighting.shininess = 32.0f;
    lighting.pad1 = lighting.pad2 = 0.0f;  // Required padding for std140 layout
    
    // Create and initialize the UBO
    m_shader->createUBO("LightingUBO", sizeof(FlockingShaders::LightingBlock));
    m_shader->updateUBO("LightingUBO", &lighting, sizeof(FlockingShaders::LightingBlock));
    m_shader->bindUBOToBindingPoint("LightingUBO", FlockingShaders::LIGHTING_BINDING_POINT);
}

void GLWindow::updateLightingUBO() {
    if (m_cam && m_light) {
        FlockingShaders::LightingBlock lighting;
        
        // Get camera position for view direction calculations
        glm::vec3 cameraPos = m_cam->getEye();
        glm::vec3 lightPos = m_light->getPosition();
        glm::vec3 lightColor = m_light->getColor();
        
        // Set lighting data
        lighting.lightPos = lightPos;
        lighting.viewPos = cameraPos;
        lighting.lightColor = lightColor;
        lighting.shininess = 32.0f;
        lighting.pad1 = lighting.pad2 = 0.0f;
        
        // Update the UBO with current lighting data
        m_shader->updateUBO("LightingUBO", &lighting, sizeof(FlockingShaders::LightingBlock));
        
        // Print debug info occasionally
        static int debugCounter = 0;
        if (debugCounter++ % 180 == 0) { // Print every 3 seconds at 60fps
            std::cout << "Updated lighting UBO - Camera position: (" 
                      << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << std::endl;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update matrix UBO with current transform data
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateMatrixUBO(const TransformStack& transformStack) {
    FlockingShaders::MatrixBlock matrices;
    
    // Get matrices from transform stack and camera
    glm::mat4 modelMatrix = transformStack.getCurrentTransform();
    glm::mat4 viewMatrix = m_cam->getViewMatrix();
    glm::mat4 projMatrix = m_cam->getProjectionMatrix();
    
    // Debug matrices occasionally (every 60 frames)
    static int debugCounter = 0;
    if (debugCounter++ % 60 == 0) {
        // Print model matrix translation (world-space position)
        std::cout << "Model matrix translation (world): [" 
                  << modelMatrix[3][0] << ", " 
                  << modelMatrix[3][1] << ", " 
                  << modelMatrix[3][2] << "]" << std::endl;
        
        // Print camera position (world space)
        glm::vec3 eye = m_cam->getEye();
        std::cout << "Camera position (world): [" 
                  << eye.x << ", " << eye.y << ", " << eye.z << "]" << std::endl;
                  
        // Print view matrix
        std::cout << "View matrix first column: [" 
                  << viewMatrix[0][0] << ", " 
                  << viewMatrix[0][1] << ", " 
                  << viewMatrix[0][2] << "]" << std::endl;
    }
    
    // Set up matrices for UBO
    matrices.M = modelMatrix;                              // Model matrix (object → world)
    matrices.MV = viewMatrix * modelMatrix;                // Model-View matrix (object → camera)
    matrices.MVP = projMatrix * viewMatrix * modelMatrix;  // Model-View-Projection (object → clip space)
    
    // Calculate normal matrix (inverse transpose of model-view's 3x3 upper-left)
    // This corrects normals when there's non-uniform scaling
    glm::mat3 normalMat = glm::mat3(matrices.MV);
    normalMat = glm::transpose(glm::inverse(normalMat));
    matrices.normalMatrix = normalMat;
    
    // Set viewer position in world space for lighting calculations
    matrices.viewerPos = m_cam->getEye();
    matrices.shouldNormalize = 1.0f;

    // Update the UBO with new matrices
    m_shader->updateUBO("MatrixUBO", &matrices, sizeof(FlockingShaders::MatrixBlock));
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
        
        // Debug output for light position
        static int lightDebugCounter = 0;
        if (lightDebugCounter++ % 120 == 0) { // Print every 2 seconds at 60fps
            std::cout << "Light position (world): (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ")" << std::endl;
            std::cout << "Light color: (" << lightColor.x << ", " << lightColor.y << ", " << lightColor.z << ")" << std::endl;
        }
        
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


