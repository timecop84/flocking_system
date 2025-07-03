#include "obstacle.h"
#include "Material.h"
#include "ShaderLib.h"
#include "Matrix.h"
#include "SphereGeometry.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

Obstacle::Obstacle(Vector spherePosition, GLfloat sphereRadius)
{
    _spherePosition = spherePosition;
    _sphereRadius = sphereRadius;
    m_colour.set(0.5f, 1.0f, 0.6f, 1.0f);
    m_wireframe = false;

    _hit = false;
    
    // Initialize the sphere geometry (lazy initialization will happen in first draw)
    m_sphereGeometry = nullptr;
}

Obstacle::~Obstacle()
{
    // Cleanup will be handled automatically by the unique_ptr
}

//----------------------------------------------------------------------------------------------------------------------
// Legacy function removed - UBO-based rendering handles matrix updates automatically
//----------------------------------------------------------------------------------------------------------------------

void Obstacle::ObsDraw(const std::string &_shaderName, TransformStack &_transformStack, Camera *_cam) const
{
    // For debugging only - can be removed in production
    if (_shaderName != "Phong") {
        std::cerr << "Warning: Obstacle only supports Phong shader, got: " << _shaderName << std::endl;
    }
    
    // Lazy initialization of sphere geometry
    if (!m_sphereGeometry) {
        std::cout << "Initializing sphere geometry for obstacle..." << std::endl;
        // Create sphere at origin - position will be applied via model matrix
        m_sphereGeometry = std::make_unique<FlockingGeometry::SphereGeometry>(_sphereRadius, 32, 32);
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

    // Apply current transform from transform stack (already includes our position)
    // The transform matrix and material should already be in the UBOs
    
    // Simply render the geometry with the current transform state
    m_sphereGeometry->render();

    // Restore previous polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, prevPolygonMode[0]);
}

//----------------------------------------------------------------------------------------------------------------------
// Legacy function removed - UBO-based rendering handles matrix updates automatically
//----------------------------------------------------------------------------------------------------------------------