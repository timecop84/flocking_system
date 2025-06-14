#include "obstacle.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Material.h>

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
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->use(_shaderName);
    ngl::Material m(ngl::PEWTER);
    m.setDiffuse(m_colour);
    m.loadToShader("material");
    // grab an instance of the primitives for drawing
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->createSphere("obstacle",_sphereRadius,20);

    if (m_wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    _transformStack.pushTransform();
    {

        _transformStack.setPosition(_spherePosition);
        _transformStack.setScale(_sphereRadius,_sphereRadius,_sphereRadius);
        loadMatricesToShader(_transformStack,_cam);
        prim->draw("obstacle");


    } // and before a pop
    _transformStack.popTransform();




}





