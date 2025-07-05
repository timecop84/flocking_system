#pragma once

#include <glm/glm.hpp>

namespace FlockingShaders {

// UBO data structures for modern shader pipeline
// Note: std140 layout requires specific alignment rules

// Matrix and view data UBO (Binding point 0)
struct MatrixBlock {
    glm::mat4 MVP;          // 64 bytes
    glm::mat4 MV;           // 64 bytes  
    glm::mat4 M;            // 64 bytes
    glm::mat3 normalMatrix; // 48 bytes (3x4 in std140)
    glm::vec3 viewerPos;    // 12 bytes
    float shouldNormalize;  // 4 bytes (bool as float)
    // Total: 256 bytes (aligned to 16-byte boundary)
};

// Material properties UBO (Binding point 1)
struct MaterialBlock {
    glm::vec4 ambient;      // 16 bytes
    glm::vec4 diffuse;      // 16 bytes
    glm::vec4 specular;     // 16 bytes
    float shininess;        // 4 bytes
    float padding[3];       // 12 bytes padding for alignment
    // Total: 64 bytes (aligned to 16-byte boundary)
};

// Light properties UBO (Binding point 2)
struct LightBlock {
    glm::vec4 position;             // 16 bytes
    glm::vec4 ambient;              // 16 bytes
    glm::vec4 diffuse;              // 16 bytes
    glm::vec4 specular;             // 16 bytes
    float constantAttenuation;      // 4 bytes
    float linearAttenuation;        // 4 bytes
    float quadraticAttenuation;     // 4 bytes
    float spotCosCutoff;            // 4 bytes
    // Total: 80 bytes (aligned to 16-byte boundary)
};

// Lighting block for fragment shader (Binding point 3)
struct LightingBlock {
    glm::vec3 lightPos;    // 12 bytes
    float pad1;            // 4 bytes padding
    glm::vec3 viewPos;     // 12 bytes
    float pad2;            // 4 bytes padding
    glm::vec3 lightColor;  // 12 bytes
    float shininess;       // 4 bytes
    // Total: 48 bytes (aligned to 16-byte boundary)
};

// Camera properties UBO (Binding point 4)
struct CameraBlock {
    glm::vec3 position;     // 12 bytes
    float nearPlane;        // 4 bytes
    glm::vec3 direction;    // 12 bytes  
    float farPlane;         // 4 bytes
    glm::mat4 viewMatrix;   // 64 bytes
    glm::mat4 projMatrix;   // 64 bytes
    // Total: 160 bytes (aligned to 16-byte boundary)
};

// Binding points for UBOs
const unsigned int MATRIX_BINDING_POINT = 0;
const unsigned int MATERIAL_BINDING_POINT = 1;
const unsigned int LIGHT_BINDING_POINT = 2;
const unsigned int LIGHTING_BINDING_POINT = 3;
const unsigned int CAMERA_BINDING_POINT = 4;

} // namespace FlockingShaders
