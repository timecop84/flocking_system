#include "boid.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Random.h>
#include <ngl/Material.h>

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

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->use(_shaderName);
    ngl::Material m;
    m.set(ngl::BLACKPLASTIC);
    m.setDiffuse(m_colour);
    m.loadToShader("material");
    // grab an instance of the primitives for drawing
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    if (m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    _transformStack.pushTransform();
    {

        _transformStack.setPosition(m_position);
        _transformStack.setScale(m_size,m_size,m_size);
        _transformStack.setScale(m_scale);
        loadMatricesToShader(_transformStack,_cam);
        prim->draw("sphere");

    } // and before a pop
    _transformStack.popTransform();


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
