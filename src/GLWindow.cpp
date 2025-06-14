#include "obstacle.h"

#include "GLWindow.h"
#include <iostream>
#include "ngl/Camera.h"
#include "ngl/Light.h"
#include "ngl/Transformation.h"
#include "ngl/TransformStack.h"
#include "ngl/Material.h"
#include "ngl/NGLInit.h"
#include "ngl/VAOPrimitives.h"
#include "ngl/ShaderLib.h"
#include "boost/foreach.hpp"
#include"ngl/Random.h"
#include "flock.h"
#include "ngl/BBox.h"
#include <ngl/Util.h>


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
    : QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent )
{
    obstacle = new Obstacle(ngl::Vector(12,30,0), 4.0);

    // set this widget to have the initial keyboard focus
    setFocus();
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    this->resize(_parent->size());
    // Now set the initial GLWindow attributes to default values
    // Roate is false
    m_rotate=false;
    // mouse rotation values set to 0
    m_spinXFace = 0;
    m_spinYFace = 0;
    m_sphereUpdateTimer = startTimer(1000 / 60); //run at 60FPS
    m_animate = true;
    m_backgroundColour.set(0.6f, 0.6f, 0.6f, 1.0f);
}

//----------------------------------------------------------------------------------------------------------------------
GLWindow::~GLWindow()
{
    ngl::NGLInit *Init = ngl::NGLInit::instance();
    std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
    delete m_light;
    Init->NGLQuit();
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
    ngl::Colour colourToSet;
    colourToSet.set(colour.redF(), colour.greenF(), colour.blueF());

    flock->setColour(colourToSet);
}

void GLWindow::setFlockWireframe(bool value)
{
    flock->setWireframe(value);
}

void GLWindow::setObstaclePosition(ngl::Vector position)
{
    obstacle->setSpherePosition(position);
}

void GLWindow::setObstacleSize(double size)
{
    obstacle->setSphereRadius(size);
}

void GLWindow::setObstacleColour(QColor colour)
{
    ngl::Colour colourToSet;
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

void GLWindow::setBackgroundColour(ngl::Colour colour)
{
    m_backgroundColour = colour;
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
}

void GLWindow::setBBoxSize(ngl::Vector size)
{
    delete bbox;
    bbox = new ngl::BBox(ngl::Vector(0,0,0), size.m_x, size.m_y, size.m_z);
}
//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining VBOs etc.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // we need to initialise the NGL lib, under windows and linux we also need to
    // initialise GLEW, under windows this needs to be done in the app as well
    // as the lib hence the WIN32 define
    ngl::NGLInit *Init = ngl::NGLInit::instance();
    Init->initGlew();
#ifdef WIN32
    glewInit(); // need a local glew init as well as lib one for windows
#endif
    // now to load the shader and set the values
    // grab an instance of shader manager
    m_shader = ngl::ShaderLib::instance();
    // we are creating a shader called Phong
    m_shader->createShaderProgram("Phong");
    // now we are going to create empty shaders for Frag and Vert
    m_shader->attachShader("PhongVertex",ngl::VERTEX);
    m_shader->attachShader("PhongFragment",ngl::FRAGMENT);
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
    ngl::Material m(ngl::GOLD);
    // load our material values to the shader into the structure material (see Vertex shader)
    m.loadToShader("material");
    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
    ngl::Vector From(200,120,120);
    ngl::Vector To(0,0,0);
    ngl::Vector Up(0,1,0);
    // now load to our new camera
    m_cam= new ngl::Camera(From,To,Up,ngl::PERSPECTIVE);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam->setShape(45,(float)720.0/576.0,0.05,350,ngl::PERSPECTIVE);
    m_shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);
    // now create our light this is done after the camera so we can pass the
    // transpose of the projection matrix to the light to do correct eye space
    // transformations
    ngl::Matrix iv=m_cam->getViewMatrix();
    iv.transpose();
    m_light = new ngl::Light(ngl::Vector(120,120,120,1),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
    m_light->setTransform(iv);
    // load these values to the shader as well
    m_light->loadToShader("light");
    m_shader->createShaderProgram("Colour");

    m_shader->attachShader("ColourVertex",ngl::VERTEX);
    m_shader->attachShader("ColourFragment",ngl::FRAGMENT);
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
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->createSphere("sphere",0.8,1);
    bbox = new ngl::BBox(ngl::Vector(0,0,0),120,120,120);
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
    // set the viewport for openGL
    glViewport(0,0,_w,_h);
    // now set the camera size values as the screen size has changed
    m_cam->setShape(45,(float)_w/_h,0.05,350,ngl::PERSPECTIVE);
}


void GLWindow::loadMatricesToShader(
        ngl::TransformStack &_tx
        )
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Matrix MV;
    ngl::Matrix MVP;
    ngl::Mat3x3 normalMatrix;
    ngl::Matrix M;
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
        ngl::TransformStack &_tx
        )

{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Colour"]->use();
    ngl::Matrix MV;
    ngl::Matrix MVP;
    ngl::Matrix M;
    ngl::Mat3x3 normalMatrix;
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
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // grab an instance of the shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    //Rotation based on the mouse position for our global transform
    ngl::Transformation trans;
    ngl::Matrix rotX;
    ngl::Matrix rotY;

    // create the rotation matrices
    rotX.rotateX(m_spinXFace);
    rotY.rotateY(m_spinYFace);
    // multiply the rotations
    ngl::Matrix final=rotY*rotX;
    // add the translations
    final.m_m[3][0] = m_modelPos.m_x;
    final.m_m[3][1] = m_modelPos.m_y;
    final.m_m[3][2] = m_modelPos.m_z;
    // set this in the TX stack
    trans.setMatrix(final);
    m_transformStack.setGlobal(trans);
    (*shader)["Colour"]->use();

    loadMatricesToShader(m_transformStack);

    bbox->draw();
    flock->draw("Phong",m_transformStack,m_cam);

    {
        m_transformStack.pushTransform();
        {
            //m_transformStack.setCurrent(0,10,0);
            loadMatricesToShader(m_transformStack);
            obstacle->ObsDraw("Phong",m_transformStack,m_cam);
        }
        m_transformStack.popTransform();
    }

    {
        m_transformStack.pushTransform();
        {
            loadMatricesToShader(m_transformStack);
        }
        m_transformStack.popTransform();
    }

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
        QMouseEvent * _event
        )
{
    // note the method buttons() is the button state when event was called
    // this is different from button() which is used to check which button was
    // pressed when the mousePress/Release event is generated
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
        int diffx=_event->x()-m_origX;
        int diffy=_event->y()-m_origY;
        m_spinXFace += (float) 0.5f * diffy;
        m_spinYFace += (float) 0.5f * diffx;
        m_origX = _event->x();
        m_origY = _event->y();
        updateGL();

    }
    // right mouse translate code
    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
        int diffX = (int)(_event->x() - m_origXPos);
        int diffY = (int)(_event->y() - m_origYPos);
        m_origXPos=_event->x();
        m_origYPos=_event->y();
        m_modelPos.m_x += INCREMENT * diffX;
        m_modelPos.m_y -= INCREMENT * diffY;
        updateGL();

    }

}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
        QMouseEvent * _event
        )
{
    // this method is called when the mouse button is pressed in this case we
    // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
    if(_event->button() == Qt::LeftButton)
    {
        m_origX = _event->x();
        m_origY = _event->y();
        m_rotate =true;
    }
    // right mouse translate mode
    else if(_event->button() == Qt::RightButton)
    {
        m_origXPos = _event->x();
        m_origYPos = _event->y();
        m_translate=true;
    }

}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
        QMouseEvent * _event
        )
{
    // this event is called when the mouse button is released
    // we then set Rotate to false
    if (_event->button() == Qt::LeftButton)
    {
        m_rotate=false;
    }
    // right mouse translate mode
    if (_event->button() == Qt::RightButton)
    {
        m_translate=false;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{

    // check the diff of the wheel position (0 means no change)
    if(_event->delta() > 0)
    {
        m_modelPos.m_z+=ZOOM;
    }
    else if(_event->delta() <0 )
    {
        m_modelPos.m_z-=ZOOM;
    }
    updateGL();

    ngl::Vector v;
    v.set(0, 0, 0);

    ngl::Vector direction;
    direction.set(0, 0.1, 0);

    v += direction;
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


        flock->update();
        updateGL();
    }

}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------


