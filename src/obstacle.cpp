#include "obstacle.h"
#include "ngl_compat/VAOPrimitives.h"
#include "ngl_compat/Material.h"
#include "ngl_compat/ShaderLib.h"
#include "ngl_compat/Matrix.h"
#include <GL/gl.h>

Obstacle::Obstacle(ngl::Vector spherePosition, GLfloat sphereRadius)
{
    _spherePosition = spherePosition;
    _sphereRadius = sphereRadius;
    m_colour.set(0.5f, 1.0f, 0.6f, 1.0f);
    m_wireframe = false;

    _hit = false;
}

void Obstacle::loadMatricesToShader(ngl::TransformStack &_tx, ngl::Camera *_cam) const
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Matrix MV;
    ngl::Matrix MVP;
    ngl::Mat3x3 normalMatrix;
    ngl::Matrix M;
    M=_tx.getCurrentTransform().getMatrix();
    MV=  _tx.getCurrAndGlobal().getMatrix()*_cam->getViewMatrix();
    MVP=  MV*_cam->getProjectionMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMatrix("MV",MV);
    shader->setShaderParamFromMatrix("MVP",MVP);
    shader->setShaderParamFromMat3x3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMatrix("M",M);

}

void Obstacle::ObsDraw(const std::string &_shaderName, ngl::TransformStack &_transformStack, ngl::Camera *_cam) const
{
    // Use immediate mode OpenGL instead of shader system for now
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set material color
    float diffuse[] = {m_colour.m_r, m_colour.m_g, m_colour.m_b, m_colour.m_a};
    float ambient[] = {m_colour.m_r * 0.3f, m_colour.m_g * 0.3f, m_colour.m_b * 0.3f, m_colour.m_a};
    float specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    float shininess = 50.0f;
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // Set light position
    float lightPos[] = {100.0f, 100.0f, 100.0f, 1.0f};
    float lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    if (m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
    {
        // Apply transformations
        glTranslatef(_spherePosition.m_x, _spherePosition.m_y, _spherePosition.m_z);
        glScalef(_sphereRadius, _sphereRadius, _sphereRadius);
        
        // Draw sphere using VAOPrimitives
        ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
        prim->createSphere("obstacle", _sphereRadius, 20);
        prim->draw("obstacle");
    }
    glPopMatrix();
}





