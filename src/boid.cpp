#include "boid.h"
#include "Material.h"
#include "ShaderLib.h"
#include "Matrix.h"
#include "ModernExample.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

Boid::Boid(

        Vector position,
        Vector direction

        )
{
    m_position = position;
    m_direction = direction;
    m_scale.set(1.0f, 1.0f, 1.0f);
    m_colour.set(1.0f, 0.0f, 0.5f, 1.0f);
    m_velocity.set(0.3f, 0.3f, 0.1f);  // Initialize at min velocity level
    m_maxVelocity = 0.9;
    m_minVelocity = 0.3;
    m_wireframe = false;
    m_size = 1;
}

//----------------------------------------------------------------------------------------------------------------------
void Boid::loadMatricesToShader(
        TransformStack &_tx,
        Camera *_cam
        )const
{
    ShaderLib *shader=ShaderLib::instance();

    Matrix MV;
    Matrix MVP;
    Mat3x3 normalMatrix;
    Matrix M;
    M=_tx.getCurrentTransform();
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
        TransformStack &_transformStack,
        Camera *_cam
        )const
{
    // Use immediate mode OpenGL with lighting for better visual appearance
    
    glPushMatrix();
    {
        // Translate to boid position
        glTranslatef(m_position.m_x, m_position.m_y, m_position.m_z);
        
        // Scale the boid 
        float boidSize = 2.0f;
        glScalef(boidSize, boidSize, boidSize);
        
        // Enable lighting for this object
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        // Set material properties for the boid
        GLfloat ambient[] = {m_colour.m_r * 0.3f, m_colour.m_g * 0.3f, m_colour.m_b * 0.3f, 1.0f};
        GLfloat diffuse[] = {m_colour.m_r, m_colour.m_g, m_colour.m_b, 1.0f};
        GLfloat specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat shininess[] = {64.0f};
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        
        // Draw a sphere using GLU quadrics for smooth appearance
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluSphere(quad, 0.5, 16, 16);  // radius, slices, stacks
        gluDeleteQuadric(quad);
        
        glDisable(GL_LIGHTING);
    }
    glPopMatrix();
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::updateVelocity(Vector direction)
{
    m_velocity += direction;
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::boidDirection()
{
    m_newDirection = m_position - m_lastPosition;
    
    // Safety check for NaN values in velocity and direction
    if (std::isnan(m_velocity.m_x) || std::isnan(m_velocity.m_y) || std::isnan(m_velocity.m_z))
    {
        m_velocity.set(0.1f, 0.1f, 0.0f);
    }
    if (std::isnan(m_newDirection.m_x) || std::isnan(m_newDirection.m_y) || std::isnan(m_newDirection.m_z))
    {
        m_newDirection.set(0.0f, 0.0f, 0.0f);
    }
    
    Vector nextMovement = (m_velocity + m_newDirection) * 0.5f;  // More reasonable multiplier
    
    // Safety check for NaN values in the final movement calculation
    if (std::isnan(nextMovement.m_x) || std::isnan(nextMovement.m_y) || std::isnan(nextMovement.m_z))
    {
        nextMovement.set(0.1f, 0.1f, 0.0f);
    }
    
    m_position += nextMovement;
    m_lastPosition.set(m_position);
    
    // Final safety check for position
    if (std::isnan(m_position.m_x) || std::isnan(m_position.m_y) || std::isnan(m_position.m_z))
    {
        // Reset to a safe position near origin
        m_position.set(0.0f, 0.0f, 0.0f);
        m_velocity.set(0.1f, 0.1f, 0.0f);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Boid::velocityConstraint()
{
    float velocityLength = m_velocity.length();
    
    if (velocityLength > m_maxVelocity)
    {
        if(velocityLength > 0.0001f)  // Avoid division by zero
        {
            m_velocity.normalize();
            m_velocity = m_velocity * m_maxVelocity;
        }
    }
    else if (velocityLength < m_minVelocity)  // Remove the extra condition
    {
        if(velocityLength > 0.0001f)  // Avoid division by zero
        {
            m_velocity.normalize();
            m_velocity = m_velocity * m_minVelocity;
        }
        else
        {
            // If velocity is essentially zero, give it a small default velocity
            m_velocity.set(0.1f, 0.1f, 0.0f);
        }
    }
    
    // Safety check for NaN values
    if (std::isnan(m_velocity.m_x) || std::isnan(m_velocity.m_y) || std::isnan(m_velocity.m_z))
    {
        // Reset to a small default velocity if NaN detected
        m_velocity.set(0.1f, 0.1f, 0.0f);
    }
}
Boid::~Boid(){}

//----------------------------------------------------------------------------------------------------------------------
// Modern flocking calculation using GLM
//----------------------------------------------------------------------------------------------------------------------
flock::Vec3 Boid::calculateModernFlocking(const std::vector<flock::Vec3>& neighborPositions,
                                          const std::vector<flock::Vec3>& neighborVelocities) const
{
    // Use the modern flocking calculation from ModernExample.h
    auto result = flock::examples::ModernFlocking::calculateFlocking(
        getPositionModern(),
        getVelocityModern(),
        neighborPositions,
        neighborVelocities
    );
    
    return result.combined;
}
