// shader_constants.h - Modernized, documented, and cleaned up for maintainability
#pragma once

/**
 * @file shader_constants.h
 * @brief Shader, camera, and light type constants for compatibility.
 *
 * Modernized for clarity and maintainability.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */
enum ShaderType {
    VERTEX = 0,
    FRAGMENT = 1,
    GEOMETRY = 2
};
enum ProjectionType {
    PERSPECTIVE = 0,
    ORTHOGRAPHIC = 1
};
enum LightType {
    POINTLIGHT = 0,
    DIRECTIONAL = 1,
    SPOTLIGHT = 2
};
