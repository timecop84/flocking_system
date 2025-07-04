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
// Legacy function removed - UBO-based rendering handles matrix updates automatically
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void Boid::draw(
        const std::string &_shaderName,
        TransformStack &_transformStack,
        Camera *_cam
        )const
{
    // Use modern UBO-based rendering by default
    drawModern(_shaderName, _transformStack, _cam);
}

//----------------------------------------------------------------------------------------------------------------------
void Boid::drawModern(
        const std::string &_shaderName,
        TransformStack &_transformStack,
        Camera *_cam
        )const
{
    // For debugging only - can be removed in production
    if (_shaderName != "Phong") {
        std::cerr << "Warning: Boid only supports Phong shader in modern mode, got: " << _shaderName << std::endl;
    }
    
    // Lazy initialization of sphere geometry
    if (!m_sphereGeometry) {
        // Create sphere at origin - position will be applied via model matrix
        float boidRadius = 0.5f; // Same as in immediate mode
        m_sphereGeometry = std::make_unique<FlockingGeometry::SphereGeometry>(boidRadius, 16, 16);
        m_sphereGeometry->initializeBuffers();
    }
    
    // The sphere geometry will be rendered at the current model transform position
    // All transformations should be handled by the transform stack
    // We just ensure the correct shader is bound and render using VAO
    
    ShaderLib *shader = ShaderLib::instance();
    shader->use(_shaderName);
    
    // Set polygon mode for wireframe if needed
    GLint prevPolygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, prevPolygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);

    // Apply current transform from transform stack (already includes our position and scale)
    // The transform matrix and material should already be in the UBOs
    
    // Simply render the geometry with the current transform state
    m_sphereGeometry->render();

    // Restore previous polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, prevPolygonMode[0]);
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
    
    // Check for extreme values that might cause instability
    if (velocityLength > m_maxVelocity * 10.0f)
    {
        // If velocity is extremely high, clamp it more aggressively
        if(velocityLength > 0.0001f)  // Avoid division by zero
        {
            m_velocity.normalize();
            m_velocity = m_velocity * m_maxVelocity;
        }
    }
    else if (velocityLength > m_maxVelocity)
    {
        if(velocityLength > 0.0001f)  // Avoid division by zero
        {
            m_velocity.normalize();
            m_velocity = m_velocity * m_maxVelocity;
        }
    }
    else if (velocityLength < m_minVelocity)
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
