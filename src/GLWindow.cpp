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
#include "PerformanceMonitor.h"
#include "../modules/graphics/include/BBox.h"
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
    
    obstacle = new Obstacle(Vector(12,30,0), 4.0);

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

    // the shader will use the currently active material and light0 so set them
    Material m(GOLD);
    // load our material values to the shader into the structure material (see Vertex shader)
    m.loadToShader("material");
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
    
    m_shader->setShaderParam3f("viewerPos",m_cam->getEye().x,m_cam->getEye().y,m_cam->getEye().z);
    // now create our light this is done after the camera so we can pass the
    // transpose of the projection matrix to the light to do correct eye space
    // transformations
    Matrix iv=m_cam->getViewMatrix();
    iv.transpose();
    m_light = new Light(Vector(120,120,120,1),Colour(1,1,1,1),Colour(1,1,1,1),POINTLIGHT);
    m_light->setTransform(iv);
    // load these values to the shader as well
    m_light->loadToShader("light");
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
    ShaderLib *shader=ShaderLib::instance();

    Matrix MV;
    Matrix MVP;
    Mat3x3 normalMatrix;
    Matrix M;
    M=_tx.getCurrAndGlobal().getMatrix();
    MV=  _tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix();
    MVP= M*m_cam->getVPMatrix(); //MV*m_cam->getProjectionMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    //  shader->setShaderParamFromMatrix("MV",MV);
    shader->setShaderParamFromMatrix("MVP",MVP);
    //  shader->setShaderParamFromMat3x3("normalMatrix",normalMatrix);
    //  shader->setShaderParamFromMatrix("M",M);
}
void GLWindow::loadMatricesToColourShader(
        TransformStack &_tx
        )

{
    ShaderLib *shader=ShaderLib::instance();
    (*shader)["Colour"]->use();
    Matrix MV;
    Matrix MVP;
    Matrix M;
    Mat3x3 normalMatrix;
    normalMatrix=MV;
    normalMatrix.inverse();

    MV= _tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix() ;
    MVP=MV*m_cam->getProjectionMatrix();
    shader->setShaderParamFromMatrix("MVP",MVP);
    shader->setShaderParamFromMatrix("M",M);


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
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

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

    // Debug: Add immediate mode test AFTER setting up matrices
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(project));
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));
    glMultMatrixf(glm::value_ptr(model.m_matrix));

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
        flock->draw("Phong", m_transformStack, m_cam);
    }

    // Draw the obstacle with Phong shader
    if (obstacle && frame_count % 60 == 1) {
        std::cout << "Drawing obstacle" << std::endl;
    }
    if (obstacle) {
        obstacle->ObsDraw("Phong", m_transformStack, m_cam);
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


