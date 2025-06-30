#pragma once

/**
 * Graphics3D Module - Generic 3D graphics utilities
 * 
 * This module provides graphics-related classes and utilities commonly used
 * in 3D rendering applications, particularly those using OpenGL.
 * 
 * Components:
 * - Camera: 3D camera class with projection and view matrices
 * - Colour: Color class with RGB/RGBA support
 * - Material: Material properties for rendering
 * - TransformStack: Transform matrix stack for hierarchical rendering
 * - Light: Lighting system for 3D scenes
 * - ShaderLib: Shader program management and uniforms
 */

// Core graphics classes
#include "Camera.h"
#include "Colour.h"
#include "Material.h"
#include "TransformStack.h"
#include "Light.h"
#include "ShaderLib.h"

// Namespace alias for convenience
namespace graphics3d {
    // Type aliases for easier access
    using Camera = ::Camera;
    using Colour = ::Colour;
    using Color = ::Colour;  // American spelling alias
    using Material = ::Material;
    using TransformStack = ::TransformStack;
    using Light = ::Light;
    using ShaderLib = ::ShaderLib;
}

// Legacy alias for backward compatibility
namespace flock {
    namespace graphics = ::graphics3d;
}
