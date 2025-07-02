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

void Obstacle::loadMatricesToShader(TransformStack &_tx, Camera *_cam) const
{
    // Modern UBO-based approach: No need to set individual uniforms
    // The UBO data should be updated by the main rendering loop
    // This function is kept for compatibility but doesn't need to do anything
    // as the UBO system handles matrix updates automatically
    
    // TODO: Remove this function entirely once all rendering is verified to work with UBOs
}

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
    
    // Apply current transform from transform stack (already includes our position)
    // The transform matrix and material should already be in the UBOs
    
    // Simply render the geometry with the current transform state
    m_sphereGeometry->render();
}

void Obstacle::ObsDrawImmediate(const std::string &_shaderName, TransformStack &_transformStack, Camera *_cam) const
{
    // Legacy immediate mode OpenGL rendering (kept for reference/fallback)
    std::cout << "Drawing obstacle using legacy immediate mode" << std::endl;
    
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





