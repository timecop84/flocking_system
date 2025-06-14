#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__


#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/TransformStack.h>
#include <ngl/Text.h>


// must be included after our stuff becuase GLEW needs to be first
#include <QtOpenGL>
#include <QTime>
#include "boid.h"
#include "flock.h"
#include "ngl/BBox.h"
#include "obstacle.h"

/// @file GLWindow.h
/// @brief a GLWindow to visualize our flock.
/// @author Dionysios Toufexis
/// @brief modified from Jon Maceys BBox Collision. NGL Demos.
/// @date 13/06/12
/// @revision 8/07/12
/// @class GLWindow
/// @brief the main glwindow widget for our flock application using NGL.
/// put in this file
class GLWindow : public QGLWidget
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

    void setObstaclePosition(ngl::Vector position);
    void setObstacleSize(double size);
    void setObstacleColour(QColor colour);
    void setObstacleWireframe(bool value);

    void setSimDistance(double distance);
    void setSimFlockDistance(double distance);
    void setSimCohesion(double cohesion);
    void setSimSeparation(double separation);
    void setSimAlignment(double alignment);

    void setBackgroundColour(ngl::Colour colour);
    void setBBoxSize(ngl::Vector size);
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
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Camera *m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief transformation stack for the gl transformations etc
    //----------------------------------------------------------------------------------------------------------------------
    ngl::TransformStack m_transformStack;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a simple light use to illuminate the screen
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Light *m_light;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a bounding box to have our boids collide and confine them within a designated space
    //----------------------------------------------------------------------------------------------------------------------
    ngl::BBox *bbox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a sphere obstacle within the boid space
    //----------------------------------------------------------------------------------------------------------------------
    Obstacle *obstacle;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to the flock class to have access to the methods
    Flock *flock;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store the GL Depth Color
    ngl::Colour m_backgroundColour;
    //----------------------------------------------------------------------------------------------------------------------

protected:

    void loadMatricesToColourShader(
            ngl::TransformStack &_tx
            );

    void loadMatricesToShader(
            ngl::TransformStack &_tx
            );

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
    int m_sphereUpdateTimer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if animation is active or not
    //----------------------------------------------------------------------------------------------------------------------
    bool m_animate;
    //----------------------------------------------------------------------------------------------------------------------
    ngl::ShaderLib *m_shader;
    //----------------------------------------------------------------------------------------------------------------------


public slots:

};

#endif
