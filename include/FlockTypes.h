#pragma once

// Modern, portable types for the flocking system
// Uses standard libraries: GLM, STL, OpenGL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <vector>
#include <string>

namespace flock {

// Core math types - using GLM directly
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;
using Mat3 = glm::mat3;

// Color type
using Color = glm::vec4;

// Common colors
namespace Colors {
    inline const Color WHITE{1.0f, 1.0f, 1.0f, 1.0f};
    inline const Color BLACK{0.0f, 0.0f, 0.0f, 1.0f};
    inline const Color RED{1.0f, 0.0f, 0.0f, 1.0f};
    inline const Color GREEN{0.0f, 1.0f, 0.0f, 1.0f};
    inline const Color BLUE{0.0f, 0.0f, 1.0f, 1.0f};
    inline const Color YELLOW{1.0f, 1.0f, 0.0f, 1.0f};
    inline const Color CYAN{0.0f, 1.0f, 1.0f, 1.0f};
    inline const Color MAGENTA{1.0f, 0.0f, 1.0f, 1.0f};
    inline const Color GRAY{0.5f, 0.5f, 0.5f, 1.0f};
}

// Math constants
namespace Math {
    inline constexpr float PI = glm::pi<float>();
    inline constexpr float TWO_PI = glm::two_pi<float>();
    inline constexpr float HALF_PI = glm::half_pi<float>();
    inline constexpr float DEG_TO_RAD = PI / 180.0f;
    inline constexpr float RAD_TO_DEG = 180.0f / PI;
}

// Utility functions using GLM
namespace Utils {
    
    // Clamp value between min and max
    inline float clamp(float value, float min, float max) {
        return glm::clamp(value, min, max);
    }
    
    // Linear interpolation
    inline float lerp(float a, float b, float t) {
        return glm::mix(a, b, t);
    }
    
    // Vector operations
    inline float length(const Vec3& v) { return glm::length(v); }
    inline float distance(const Vec3& a, const Vec3& b) { return glm::distance(a, b); }
    inline Vec3 normalize(const Vec3& v) { return glm::normalize(v); }
    inline float dot(const Vec3& a, const Vec3& b) { return glm::dot(a, b); }
    inline Vec3 cross(const Vec3& a, const Vec3& b) { return glm::cross(a, b); }
    
    // Matrix operations
    inline Mat4 translate(const Mat4& m, const Vec3& v) { return glm::translate(m, v); }
    inline Mat4 rotate(const Mat4& m, float angle, const Vec3& axis) { return glm::rotate(m, angle, axis); }
    inline Mat4 scale(const Mat4& m, const Vec3& v) { return glm::scale(m, v); }
    
    // Projection matrices
    inline Mat4 perspective(float fovy, float aspect, float nearPlane, float farPlane) {
        return glm::perspective(fovy, aspect, nearPlane, farPlane);
    }
    
    inline Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
        return glm::lookAt(eye, center, up);
    }
}

}
