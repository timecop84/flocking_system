#pragma once
#include <iostream>

namespace ngl {

class NGLInit {
private:
    static NGLInit* s_instance;
    
public:
    static NGLInit* instance() {
        if (!s_instance) {
            s_instance = new NGLInit();
        }
        return s_instance;
    }
    
    void initGlew() {
        // Stub implementation - would normally initialize GLEW
        std::cout << "NGLInit: initGlew() called (stub)\n";
    }
    
    void NGLQuit() {
        // Stub implementation - would normally clean up NGL resources
        std::cout << "NGLInit: NGLQuit() called (stub)\n";
    }
};

}
