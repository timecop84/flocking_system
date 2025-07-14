#pragma once

#include <glm/glm.hpp>
#include "FlockTypes.h"

namespace FlockingShaders {
/**
 * @file UBOStructures.h
 * @brief UBO data structures for modern shader pipeline (std140 layout).
 *
 * Modernized for clarity and maintainability. Uses FlockTypes.h for type aliases and helpers.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
struct MatrixBlock {
    glm::mat4 MVP;
    glm::mat4 MV;
    glm::mat4 M;
    glm::mat3 normalMatrix;
    glm::vec3 viewerPos;
    float shouldNormalize;
};
struct MaterialBlock {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
    float padding[3];
};
struct LightBlock {
    glm::vec4 position;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float spotCosCutoff;
};
struct LightingBlock {
    glm::vec3 lightPos;
    float pad1;
    glm::vec3 viewPos;
    float pad2;
    glm::vec3 lightColor;
    float shininess;
};
struct CameraBlock {
    glm::vec3 position;     // 12 bytes
    float nearPlane;        // 4 bytes
    glm::vec3 direction;    // 12 bytes
    float farPlane;         // 4 bytes
    glm::mat4 viewMatrix;   // 64 bytes
    glm::mat4 projMatrix;   // 64 bytes
};

// UBO binding points for shader uniform blocks
constexpr int MATRIX_BINDING_POINT   = 0;
constexpr int MATERIAL_BINDING_POINT = 1;
constexpr int LIGHT_BINDING_POINT    = 2;
constexpr int LIGHTING_BINDING_POINT = 3;
} // namespace FlockingShaders
