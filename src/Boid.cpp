// Boid.cpp
// Implements a single agent: owns its position/velocity, integrates motion, and draws a sphere via the modern pipeline.

#include "Boid.h"
#include "Material.h"
#include "ShaderLib.h"
#include "Matrix.h"
#include "ModernExample.h"
#include "MathUtils.h"
#include <glad/gl.h>
#include <cmath>

Boid::Boid(

        Vector position,
        Vector direction

        )
{
    m_position = position;
    m_lastPosition = position;
    m_direction = direction;
    m_scale.set(1.0f, 1.0f, 1.0f);
    m_colour.set(1.0f, 0.0f, 0.5f, 1.0f);
    m_newDirection.set(0.0f, 0.0f, 0.0f);
    // Note: despite the name, m_nextPosition is used as a "next movement" vector in collision code.
    m_nextPosition.set(0.0f, 0.0f, 0.0f);
    m_maxVelocity = 0.9;
    m_minVelocity = 0.3;

    // Initialize velocity from the provided spawn direction.
    // The old code used a constant velocity for every boid, which strongly encourages planar collapse.
    Vector v = direction;
    if (v.lengthSquared() < 0.0001f) {
        glm::vec3 rv = math::utils::randomUnitVector();
        v.set(rv.x, rv.y, rv.z);
    }
    v.normalizeIP();
    // Avoid near-planar seeds: ensure Z component has some magnitude
    if (std::abs(v.m_z) < 0.25f) {
        float sign = (math::utils::randomFloat(-1.0f, 1.0f) >= 0.0f) ? 1.0f : -1.0f;
        v.m_z = 0.25f * sign;
        v.normalizeIP();
    }
    const float speed = math::utils::randomFloat(m_minVelocity, m_maxVelocity);
    m_velocity = v * speed;

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
    // Previous-frame movement (delta position)
    m_newDirection = m_position - m_lastPosition;
    
    // Safety check for NaN values in velocity and direction
    if (std::isnan(m_velocity.m_x) || std::isnan(m_velocity.m_y) || std::isnan(m_velocity.m_z))
    {
        glm::vec3 rv = math::utils::randomUnitVector();
        m_velocity.set(rv.x * 0.15f, rv.y * 0.15f, rv.z * 0.15f);
    }
    if (std::isnan(m_newDirection.m_x) || std::isnan(m_newDirection.m_y) || std::isnan(m_newDirection.m_z))
    {
        m_newDirection.set(0.0f, 0.0f, 0.0f);
    }
    
    Vector nextMovement = (m_velocity + m_newDirection) * 0.5f;  // Smoothed integration
    
    // Safety check for NaN values in the final movement calculation
    if (std::isnan(nextMovement.m_x) || std::isnan(nextMovement.m_y) || std::isnan(nextMovement.m_z))
    {
        glm::vec3 rv = math::utils::randomUnitVector();
        nextMovement.set(rv.x * 0.15f, rv.y * 0.15f, rv.z * 0.15f);
    }
    
    // Store the *movement delta* for collision/response code (see validateBoundingBoxCollision / reverse())
    m_nextPosition = nextMovement;

    // CRITICAL: update last position BEFORE integrating movement
    m_lastPosition.set(m_position);
    m_position += nextMovement;
    
    // Final safety check for position
    if (std::isnan(m_position.m_x) || std::isnan(m_position.m_y) || std::isnan(m_position.m_z))
    {
        // Reset to a safe position near origin
        m_position.set(0.0f, 0.0f, 0.0f);
        glm::vec3 rv = math::utils::randomUnitVector();
        m_velocity.set(rv.x * 0.15f, rv.y * 0.15f, rv.z * 0.15f);
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
            m_velocity.normalizeIP();
            m_velocity = m_velocity * m_maxVelocity;
        }
    }
    else if (velocityLength > m_maxVelocity)
    {
        if(velocityLength > 0.0001f)  // Avoid division by zero
        {
            m_velocity.normalizeIP();
            m_velocity = m_velocity * m_maxVelocity;
        }
    }
    else if (velocityLength < m_minVelocity)
    {
        if(velocityLength > 0.0001f)  // Avoid division by zero
        {
            m_velocity.normalizeIP();
            m_velocity = m_velocity * m_minVelocity;
        }
        else
        {
            // If velocity is essentially zero, give it a small default velocity
            glm::vec3 rv = math::utils::randomUnitVector();
            m_velocity.set(rv.x * m_minVelocity, rv.y * m_minVelocity, rv.z * m_minVelocity);
        }
    }
    
    // Safety check for NaN values
    if (std::isnan(m_velocity.m_x) || std::isnan(m_velocity.m_y) || std::isnan(m_velocity.m_z))
    {
        // Reset to a small default velocity if NaN detected
        glm::vec3 rv = math::utils::randomUnitVector();
        m_velocity.set(rv.x * m_minVelocity, rv.y * m_minVelocity, rv.z * m_minVelocity);
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
