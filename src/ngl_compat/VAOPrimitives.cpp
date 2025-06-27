#include "ngl_compat/VAOPrimitives.h"
#include <GL/gl.h>
#include <cmath>

namespace ngl {
    VAOPrimitives* VAOPrimitives::s_instance = nullptr;
    
    void VAOPrimitives::createSphere(const std::string& /*name*/, float radius, int slices) {
        // Store sphere parameters for later use
        m_sphereRadius = radius;
        m_sphereSlices = slices;
    }
    
    void VAOPrimitives::draw(const std::string& name) {
        if (name == "sphere" || name == "obstacle") {
            drawSphere(m_sphereRadius > 0 ? m_sphereRadius : 1.0f, m_sphereSlices > 0 ? m_sphereSlices : 20);
        }
    }
    
    void VAOPrimitives::drawSphere(float radius, int slices) {
        int stacks = slices / 2;
        float pi = 3.14159265359f;
        
        for (int i = 0; i < stacks; ++i) {
            float lat0 = pi * (-0.5f + (float)(i) / stacks);
            float z0 = sin(lat0);
            float zr0 = cos(lat0);
            
            float lat1 = pi * (-0.5f + (float)(i + 1) / stacks);
            float z1 = sin(lat1);
            float zr1 = cos(lat1);
            
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; ++j) {
                float lng = 2 * pi * (float)(j) / slices;
                float x = cos(lng);
                float y = sin(lng);
                
                glNormal3f(x * zr0, y * zr0, z0);
                glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
                
                glNormal3f(x * zr1, y * zr1, z1);
                glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
            }
            glEnd();
        }
    }
}
