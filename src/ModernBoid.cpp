#include "../include/ModernBoid.h"
#include "../include/ModernRandom.h"
#include <algorithm>
#include <GL/gl.h>

Boid::Boid() {
    // Initialize with random position and direction
    flock::Random random;
    m_position = random.randomVec3() * 5.0f;  // Random position in a 5x5x5 cube
    m_direction = random.randomNormalizedVec3();
    m_velocity = m_direction * random.randomFloat(MIN_VELOCITY, MAX_VELOCITY);
    m_color = flock::Colors::WHITE;
    m_scale = flock::Vec3(1.0f);
}

Boid::Boid(const flock::Vec3& position, const flock::Vec3& direction) 
    : m_position(position), m_direction(flock::Utils::normalize(direction)) {
    flock::Random random;
    m_velocity = m_direction * random.randomFloat(MIN_VELOCITY, MAX_VELOCITY);
    m_color = flock::Colors::WHITE;
    m_scale = flock::Vec3(1.0f);
}

void Boid::updateVelocity(const flock::Vec3& direction) {
    if (flock::Utils::length(direction) > 0.0001f) {
        m_newDirection = flock::Utils::normalize(direction);
        
        // Update velocity based on new direction
        flock::Vec3 newVelocity = m_newDirection * flock::Utils::length(m_velocity);
        
        // Clamp velocity to limits
        float speed = flock::Utils::length(newVelocity);
        speed = flock::Utils::clamp(speed, MIN_VELOCITY, MAX_VELOCITY);
        
        if (speed > 0.0001f) {
            m_velocity = flock::Utils::normalize(newVelocity) * speed;
        }
        
        // Apply damping
        m_velocity *= DAMPING;
        
        m_direction = m_newDirection;
    }
}

void Boid::update() {
    // Store last position
    m_lastPosition = m_position;
    
    // Update position based on velocity
    m_nextPosition = m_position + m_velocity * TIME_STEP;
    m_position = m_nextPosition;
}

flock::Vec3 Boid::checkBoundaryDirection(const flock::Vec3& min, const flock::Vec3& max) {
    flock::Vec3 newDirection(0.0f);
    
    // Check each axis and apply steering force if near boundary
    const float margin = 1.0f;  // Distance from boundary to start steering
    const float strength = 2.0f; // Steering force strength
    
    // X boundaries
    if (m_position.x < min.x + margin) {
        newDirection.x += strength * (min.x + margin - m_position.x);
    } else if (m_position.x > max.x - margin) {
        newDirection.x -= strength * (m_position.x - (max.x - margin));
    }
    
    // Y boundaries  
    if (m_position.y < min.y + margin) {
        newDirection.y += strength * (min.y + margin - m_position.y);
    } else if (m_position.y > max.y - margin) {
        newDirection.y -= strength * (m_position.y - (max.y - margin));
    }
    
    // Z boundaries
    if (m_position.z < min.z + margin) {
        newDirection.z += strength * (min.z + margin - m_position.z);
    } else if (m_position.z > max.z - margin) {
        newDirection.z -= strength * (m_position.z - (max.z - margin));
    }
    
    return newDirection;
}

void Boid::draw() const {
    glPushMatrix();
    
    // Transform to boid position
    glTranslatef(m_position.x, m_position.y, m_position.z);
    
    // Scale the boid
    glScalef(m_scale.x, m_scale.y, m_scale.z);
    
    // Set color
    glColor4f(m_color.r, m_color.g, m_color.b, m_color.a);
    
    // Set rendering mode
    if (m_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    // Enable lighting for better visual quality
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set material properties
    float ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float diffuse[] = {m_color.r, m_color.g, m_color.b, m_color.a};
    float specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    float shininess = 64.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // Draw a simple tetrahedron pointing in the direction of movement
    float size = 0.3f;
    
    glBegin(GL_TRIANGLES);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(-size * 0.5f, -size * 0.5f, size * 0.5f);
    glVertex3f(size * 0.5f, -size * 0.5f, size * 0.5f);
    
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(size * 0.5f, -size * 0.5f, size * 0.5f);
    glVertex3f(size * 0.5f, -size * 0.5f, -size * 0.5f);
    
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(size * 0.5f, -size * 0.5f, -size * 0.5f);
    glVertex3f(-size * 0.5f, -size * 0.5f, -size * 0.5f);
    
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, size, 0.0f);
    glVertex3f(-size * 0.5f, -size * 0.5f, -size * 0.5f);
    glVertex3f(-size * 0.5f, -size * 0.5f, size * 0.5f);
    
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-size * 0.5f, -size * 0.5f, size * 0.5f);
    glVertex3f(-size * 0.5f, -size * 0.5f, -size * 0.5f);
    glVertex3f(size * 0.5f, -size * 0.5f, -size * 0.5f);
    
    glVertex3f(-size * 0.5f, -size * 0.5f, size * 0.5f);
    glVertex3f(size * 0.5f, -size * 0.5f, -size * 0.5f);
    glVertex3f(size * 0.5f, -size * 0.5f, size * 0.5f);
    
    glEnd();
    
    glPopMatrix();
}
