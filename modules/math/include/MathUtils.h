#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Vector.h"
#include "Matrix.h"

namespace math {
    // Mathematical constants
    const float PI = 3.14159265359f;
    const float TWO_PI = 2.0f * PI;
    const float HALF_PI = PI / 2.0f;
    const float DEG_TO_RAD = PI / 180.0f;
    const float RAD_TO_DEG = 180.0f / PI;
    
    // Type aliases for convenience
    using Vec3 = Vector;
    using Mat4 = Matrix;
    
    namespace utils {
        
        // Basic math utilities
        float clamp(float value, float min, float max);
        float lerp(float a, float b, float t);
        float smoothstep(float edge0, float edge1, float x);
        float map(float value, float inMin, float inMax, float outMin, float outMax);
        
        // Vector utilities
        glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t);
        float distance(const glm::vec3& a, const glm::vec3& b);
        float distanceSquared(const glm::vec3& a, const glm::vec3& b);
        glm::vec3 normalize(const glm::vec3& v);
        bool isNormalized(const glm::vec3& v, float tolerance = 0.001f);
        
        // Angle utilities
        float normalizeAngle(float angle);
        float angleBetween(const glm::vec3& a, const glm::vec3& b);
        glm::vec3 rotateVector(const glm::vec3& v, float angle, const glm::vec3& axis);
        
        // Random utilities
        float randomFloat(float min = -1.0f, float max = 1.0f);
        int randomInt(int min, int max);
        glm::vec3 randomVector(float minLength = 0.0f, float maxLength = 1.0f);
        glm::vec3 randomUnitVector();
        
        // Additional random utilities used by flocking system
        glm::vec3 randomVec3();  // Random vector with components in [-1, 1]
        glm::vec3 randomPoint(float x, float y, float z);  // Random point within bounds
        
        // Easing functions
        float easeInQuad(float t);
        float easeOutQuad(float t);
        float easeInOutQuad(float t);
        float easeInCubic(float t);
        float easeOutCubic(float t);
        float easeInOutCubic(float t);
        
        // Geometric utilities
        bool pointInSphere(const glm::vec3& point, const glm::vec3& center, float radius);
        bool sphereIntersect(const glm::vec3& center1, float radius1, 
                           const glm::vec3& center2, float radius2);
        glm::vec3 closestPointOnLine(const glm::vec3& point, 
                                   const glm::vec3& lineStart, 
                                   const glm::vec3& lineEnd);
        
        // Statistics
        float average(const std::vector<float>& values);
        float variance(const std::vector<float>& values);
        float standardDeviation(const std::vector<float>& values);
    }
}
