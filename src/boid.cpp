#include "boid.h"
#include "ngl_compat/VAOPrimitives.h"
#include "ngl_compat/Material.h"
#include "ngl_compat/ShaderLib.h"
#include "ngl_compat/Matrix.h"
#include <GL/gl.h>

Boid::Boid(

        ngl::Vector position,
        ngl::Vector direction

        )
{
    m_position = position;
    m_direction = direction;
    m_scale.set(1.0f, 1.0f, 1.0f);
    m_colour.set(1.0f, 0.0f, 0.5f, 1.0f);
    m_velocity.set(8.0f, 8.0f, 0.0f);
    m_maxVelocity = 0.9;
    m_minVelocity = 0.3;
    m_wireframe = false;
    m_size = 1;
}

//----------------------------------------------------------------------------------------------------------------------
void Boid::loadMatricesToShader(
        ngl::TransformStack &_tx,
        ngl::Camera *_cam
        )const
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Matrix MV;
    ngl::Matrix MVP;
    ngl::Mat3x3 normalMatrix;
    ngl::Matrix M;
    M=_tx.getCurrentTransform().getMatrix();
    MV=_tx.getCurrAndGlobal().getMatrix()*_cam->getViewMatrix() ;
    MVP=MV*_cam->getProjectionMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMatrix("MV",MV);
    shader->setShaderParamFromMatrix("MVP",MVP);
    shader->setShaderParamFromMat3x3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMatrix("M",M);
}

//----------------------------------------------------------------------------------------------------------------------
void Boid::draw(
        const std::string &_shaderName,
        ngl::TransformStack &_transformStack,
        ngl::Camera *_cam
        )const
{
    // Use immediate mode OpenGL instead of shader system for now
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set material color for boid
    float diffuse[] = {m_colour.m_r, m_colour.m_g, m_colour.m_b, m_colour.m_a};
    float ambient[] = {m_colour.m_r * 0.3f, m_colour.m_g * 0.3f, m_colour.m_b * 0.3f, m_colour.m_a};
    float specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    float shininess = 20.0f;
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    if (m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glPushMatrix();
    {
        // Apply transformations
        glTranslatef(m_position.m_x, m_position.m_y, m_position.m_z);
        glScalef(m_size * m_scale.m_x, m_size * m_scale.m_y, m_size * m_scale.m_z);
        
        // Draw sphere using VAOPrimitives
        ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
        prim->createSphere("sphere", 1.0f, 12);
        prim->draw("sphere");
    }
    glPopMatrix();
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::updateVelocity(ngl::Vector direction)
{
    m_velocity += direction;
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::boidDirection()
{

    m_newDirection = m_position - m_lastPosition;
    //ngl::Vector position (m_position);
    m_position += (m_velocity + m_newDirection) * 2.2;
    m_lastPosition.set(m_position);
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::velocityConstraint()
{
    if (m_velocity.length() > m_maxVelocity)
    {
        if(abs(m_velocity.length())>0)
        {
            m_velocity.normalize();
            m_velocity.lengthSquared();
        }
        //
        m_velocity =  m_velocity * m_maxVelocity;
    }

    if (m_velocity.length() < m_minVelocity)
    {
        if(abs(m_velocity.length() < 0))
        {
            m_velocity.lengthSquared();
            m_velocity.normalize();
        }
        //_velocity.lengthSquared();
        m_velocity =  m_velocity * m_minVelocity;
    }
}
Boid::~Boid(){}
