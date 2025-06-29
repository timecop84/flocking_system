#pragma once

// Modern type aliases for the flocking system
// These replace the legacy ngl:: types with modern equivalents

#include "../modules/graphics/include/Camera.h"
#include "../modules/graphics/include/Material.h"
#include "../modules/utils/include/Random.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace flock {

// Core math types - using GLM directly for modernization
using Vector = glm::vec3;
using Vector4 = glm::vec4;
using Matrix = glm::mat4;
using Matrix3 = glm::mat3;

// Graphics types  
using Camera = graphics::Camera;
using Material = graphics::Material;

// Utility types
using Random = utils::Random;

// Color type (using GLM)
using Color = glm::vec4;

// Common color constants
namespace Colors {
    inline const Color WHITE{1.0f, 1.0f, 1.0f, 1.0f};
    inline const Color BLACK{0.0f, 0.0f, 0.0f, 1.0f};
    inline const Color RED{1.0f, 0.0f, 0.0f, 1.0f};
    inline const Color GREEN{0.0f, 1.0f, 0.0f, 1.0f};
    inline const Color BLUE{0.0f, 0.0f, 1.0f, 1.0f};
    inline const Color YELLOW{1.0f, 1.0f, 0.0f, 1.0f};
    inline const Color CYAN{0.0f, 1.0f, 1.0f, 1.0f};
    inline const Color MAGENTA{1.0f, 0.0f, 1.0f, 1.0f};
}

}
