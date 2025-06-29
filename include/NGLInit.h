#ifndef NGLINIT_H
#define NGLINIT_H

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern NGLInit stub for compatibility
/// @details Provides interface compatibility for initialization calls
//----------------------------------------------------------------------------------------------------------------------
class NGLInit {
public:
    static NGLInit* instance();
    
    void initGlew();
    void NGLQuit();  // Add missing method
    
private:
    NGLInit() = default;
    static NGLInit* s_instance;
};

#endif // NGLINIT_H
