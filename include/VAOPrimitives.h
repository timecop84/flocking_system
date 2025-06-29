#ifndef VAOPRIMITIVES_H
#define VAOPRIMITIVES_H

#include <string>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern VAOPrimitives stub for compatibility
/// @details Provides interface compatibility for primitive drawing calls
//----------------------------------------------------------------------------------------------------------------------
class VAOPrimitives {
public:
    static VAOPrimitives* instance();
    
    void createSphere(const std::string& name, float radius, int precision = 20);
    void draw(const std::string& name);
    
private:
    VAOPrimitives() = default;
    static VAOPrimitives* s_instance;
};

#endif // VAOPRIMITIVES_H
