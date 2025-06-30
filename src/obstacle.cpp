#include "obstacle.h"
#include "Material.h"
#include "ShaderLib.h"
#include "Matrix.h"
#include <GL/gl.h>
#include <GL/glu.h>

Obstacle::Obstacle(Vector spherePosition, GLfloat sphereRadius)
{
    _spherePosition = spherePosition;
    _sphereRadius = sphereRadius;
    m_colour.set(0.5f, 1.0f, 0.6f, 1.0f);
    m_wireframe = false;

    _hit = false;
}

void Obstacle::loadMatricesToShader(TransformStack &_tx, Camera *_cam) const
{
    ShaderLib *shader=ShaderLib::instance();

    Matrix MV;
    Matrix MVP;
    Mat3x3 normalMatrix;
    Matrix M;
    M=_tx.getCurrentTransform();
    MV=  _tx.getCurrAndGlobal().getMatrix()*_cam->getViewMatrix();
    MVP=  MV*_cam->getProjectionMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMatrix("MV",MV);
    shader->setShaderParamFromMatrix("MVP",MVP);
    shader->setShaderParamFromMat3x3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMatrix("M",M);

}

void Obstacle::ObsDraw(const std::string &_shaderName, TransformStack &_transformStack, Camera *_cam) const
{
    // Use immediate mode OpenGL with lighting for better appearance
    glPushMatrix();
    {
        // Translate to obstacle position
        glTranslatef(_spherePosition.m_x, _spherePosition.m_y, _spherePosition.m_z);
        
        // Enable lighting for this object
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        // Set material properties for the obstacle (different from boids)
        GLfloat ambient[] = {m_colour.m_r * 0.4f, m_colour.m_g * 0.4f, m_colour.m_b * 0.4f, 1.0f};
        GLfloat diffuse[] = {m_colour.m_r, m_colour.m_g, m_colour.m_b, 1.0f};
        GLfloat specular[] = {0.9f, 0.9f, 0.9f, 1.0f};
        GLfloat shininess[] = {128.0f};
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        
        // Draw sphere using GLU quadrics for smooth appearance
        GLUquadric* quad = gluNewQuadric();
        if (m_wireframe) {
            gluQuadricDrawStyle(quad, GLU_LINE);
        } else {
            gluQuadricDrawStyle(quad, GLU_FILL);
        }
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluSphere(quad, _sphereRadius, 32, 32);  // Use actual radius, more detail
        gluDeleteQuadric(quad);
        
        glDisable(GL_LIGHTING);
    }
    glPopMatrix();
}





