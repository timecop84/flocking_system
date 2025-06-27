#pragma once
#include <string>
#include <GL/gl.h>
#include <cmath>

namespace ngl {

class VAOPrimitives {
private:
    static VAOPrimitives* s_instance;
    float m_sphereRadius = 1.0f;
    int m_sphereSlices = 20;
    
public:
    static VAOPrimitives* instance() {
        if (!s_instance) {
            s_instance = new VAOPrimitives();
        }
        return s_instance;
    }
    
    void createSphere(const std::string& /*name*/, float radius, int precision);
    void draw(const std::string& name);
    void drawSphere(float radius, int slices);
};

}
