#ifndef NGL_BBOX_H
#define NGL_BBOX_H

#include "Vector.h"
#include <GL/gl.h>
#include <array>

namespace ngl {

class BBox {
public:
    BBox(const Vector& center, float width, float height, float depth)
        : m_center(center), m_width(width), m_height(height), m_depth(depth), m_drawMode(GL_FILL) {
        // Initialize normals for the 6 planes of the bounding box
        m_normals[0] = Vector(1, 0, 0);   // right
        m_normals[1] = Vector(-1, 0, 0);  // left
        m_normals[2] = Vector(0, 1, 0);   // top
        m_normals[3] = Vector(0, -1, 0);  // bottom
        m_normals[4] = Vector(0, 0, 1);   // front
        m_normals[5] = Vector(0, 0, -1);  // back
    }
    
    void setDrawMode(GLenum mode) { m_drawMode = mode; }
    
    void draw() { 
        // Simple wireframe cube drawing
        glPushMatrix();
        glTranslatef(m_center.m_x, m_center.m_y, m_center.m_z);
        
        if (m_drawMode == GL_LINE) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        
        // Draw a simple cube using immediate mode (for testing)
        float w = m_width / 2.0f;
        float h = m_height / 2.0f;
        float d = m_depth / 2.0f;
        
        glBegin(GL_QUADS);
        // Front face
        glVertex3f(-w, -h,  d);
        glVertex3f( w, -h,  d);
        glVertex3f( w,  h,  d);
        glVertex3f(-w,  h,  d);
        
        // Back face
        glVertex3f(-w, -h, -d);
        glVertex3f(-w,  h, -d);
        glVertex3f( w,  h, -d);
        glVertex3f( w, -h, -d);
        
        // Top face
        glVertex3f(-w,  h, -d);
        glVertex3f(-w,  h,  d);
        glVertex3f( w,  h,  d);
        glVertex3f( w,  h, -d);
        
        // Bottom face
        glVertex3f(-w, -h, -d);
        glVertex3f( w, -h, -d);
        glVertex3f( w, -h,  d);
        glVertex3f(-w, -h,  d);
        
        // Right face
        glVertex3f( w, -h, -d);
        glVertex3f( w,  h, -d);
        glVertex3f( w,  h,  d);
        glVertex3f( w, -h,  d);
        
        // Left face
        glVertex3f(-w, -h, -d);
        glVertex3f(-w, -h,  d);
        glVertex3f(-w,  h,  d);
        glVertex3f(-w,  h, -d);
        glEnd();
        
        glPopMatrix();
    }
    
    float width() const { return m_width; }
    float height() const { return m_height; }
    float depth() const { return m_depth; }
    
    const std::array<Vector, 6>& getNormalArray() const { return m_normals; }
    
private:
    Vector m_center;
    float m_width, m_height, m_depth;
    GLenum m_drawMode;
    std::array<Vector, 6> m_normals;
};

} // namespace ngl

#endif // NGL_BBOX_H
