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

// Binding points for UBOs
const unsigned int MATRIX_BINDING_POINT = 0;
const unsigned int MATERIAL_BINDING_POINT = 1;
const unsigned int LIGHT_BINDING_POINT = 2;

} // namespace FlockingShaders
