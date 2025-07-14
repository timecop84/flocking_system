#pragma once

/**
 * @file FlockTypes.h
 * @brief Modern, portable types, math helpers, and color constants for the flocking system.
 *
 * This header centralizes all type aliases, color definitions, math constants, and utility functions
 * for the flocking simulation. It uses GLM for math, STL for containers, and is designed for
 * cross-platform, high-performance code.
 *
 * Usage: Include this header wherever you need flocking math types, color constants, or math helpers.
 *
 * @author Dennis Toufexis
 * @date 2025-07-11
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <memory>
#include <vector>
#include <string>

namespace flock {

// --- Core Math Types ---
using Vec3 = glm::vec3;   ///< 3D vector
using Vec4 = glm::vec4;   ///< 4D vector (often used for colors)
using Mat4 = glm::mat4;   ///< 4x4 matrix
using Mat3 = glm::mat3;   ///< 3x3 matrix

// --- Color Type and Constants ---
using Color = glm::vec4;  ///< RGBA color (float values in [0, 1])

namespace Colors {
    inline const Color WHITE   {1.0f, 1.0f, 1.0f, 1.0f}; ///< White
    inline const Color BLACK   {0.0f, 0.0f, 0.0f, 1.0f}; ///< Black
    inline const Color RED     {1.0f, 0.0f, 0.0f, 1.0f}; ///< Red
    inline const Color GREEN   {0.0f, 1.0f, 0.0f, 1.0f}; ///< Green
    inline const Color BLUE    {0.0f, 0.0f, 1.0f, 1.0f}; ///< Blue
    inline const Color YELLOW  {1.0f, 1.0f, 0.0f, 1.0f}; ///< Yellow
    inline const Color CYAN    {0.0f, 1.0f, 1.0f, 1.0f}; ///< Cyan
    inline const Color MAGENTA {1.0f, 0.0f, 1.0f, 1.0f}; ///< Magenta
    inline const Color GRAY    {0.5f, 0.5f, 0.5f, 1.0f}; ///< Gray
}

// --- Math Constants ---
namespace Math {
    inline constexpr float PI        = glm::pi<float>();      ///< Pi (π)
    inline constexpr float TWO_PI    = glm::two_pi<float>(); ///< 2π
    inline constexpr float HALF_PI   = glm::half_pi<float>();///< π/2
    inline constexpr float DEG_TO_RAD= PI / 180.0f;          ///< Degrees to radians
    inline constexpr float RAD_TO_DEG= 180.0f / PI;          ///< Radians to degrees
}

// --- Utility Functions ---
namespace Utils {
    /// Clamp value between min and max
    inline float clamp(float value, float min, float max) {
        return glm::clamp(value, min, max);
    }
    /// Linear interpolation
    inline float lerp(float a, float b, float t) {
        return glm::mix(a, b, t);
    }
    /// Vector length
    inline float length(const Vec3& v) { return glm::length(v); }
    /// Distance between two vectors
    inline float distance(const Vec3& a, const Vec3& b) { return glm::distance(a, b); }
    /// Normalize a vector
    inline Vec3 normalize(const Vec3& v) { return glm::normalize(v); }
    /// Dot product
    inline float dot(const Vec3& a, const Vec3& b) { return glm::dot(a, b); }
    /// Cross product
    inline Vec3 cross(const Vec3& a, const Vec3& b) { return glm::cross(a, b); }
    /// Matrix translation
    inline Mat4 translate(const Mat4& m, const Vec3& v) { return glm::translate(m, v); }
    /// Matrix rotation
    inline Mat4 rotate(const Mat4& m, float angle, const Vec3& axis) { return glm::rotate(m, angle, axis); }
    /// Matrix scale
    inline Mat4 scale(const Mat4& m, const Vec3& v) { return glm::scale(m, v); }
    /// Perspective projection matrix
    inline Mat4 perspective(float fovy, float aspect, float nearPlane, float farPlane) {
        return glm::perspective(fovy, aspect, nearPlane, farPlane);
    }
    /// Look-at view matrix
    inline Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
        return glm::lookAt(eye, center, up);
    }
}

} // namespace flock
