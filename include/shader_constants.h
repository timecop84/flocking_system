#ifndef SHADER_CONSTANTS_H
#define SHADER_CONSTANTS_H

// Shader type constants for compatibility
enum ShaderType {
    VERTEX = 0,
    FRAGMENT = 1,
    GEOMETRY = 2
};

// Camera projection type constants
enum ProjectionType {
    PERSPECTIVE = 0,
    ORTHOGRAPHIC = 1
};

// Light type constants
enum LightType {
    POINTLIGHT = 0,
    DIRECTIONAL = 1,
    SPOTLIGHT = 2
};

#endif // SHADER_CONSTANTS_H
