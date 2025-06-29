#include "MathUtils.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <glm/gtc/matrix_transform.hpp>

namespace math {
    namespace utils {
        
        // Random number generator
        static std::random_device rd;
        static std::mt19937 gen(rd());
        
        // Basic math utilities
        float clamp(float value, float min, float max) {
            return std::max(min, std::min(max, value));
        }
        
        float lerp(float a, float b, float t) {
            return a + t * (b - a);
        }
        
        float smoothstep(float edge0, float edge1, float x) {
            float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
            return t * t * (3.0f - 2.0f * t);
        }
        
        float map(float value, float inMin, float inMax, float outMin, float outMax) {
            return outMin + (outMax - outMin) * ((value - inMin) / (inMax - inMin));
        }
        
        // Vector utilities
        glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t) {
            return a + t * (b - a);
        }
        
        float distance(const glm::vec3& a, const glm::vec3& b) {
            return glm::length(b - a);
        }
        
        float distanceSquared(const glm::vec3& a, const glm::vec3& b) {
            glm::vec3 diff = b - a;
            return glm::dot(diff, diff);
        }
        
        glm::vec3 normalize(const glm::vec3& v) {
            float len = glm::length(v);
            return (len > 0.001f) ? v / len : glm::vec3(0.0f);
        }
        
        bool isNormalized(const glm::vec3& v, float tolerance) {
            float len = glm::length(v);
            return std::abs(len - 1.0f) < tolerance;
        }
        
        // Angle utilities
        float normalizeAngle(float angle) {
            const float PI = 3.14159265359f;
            const float TWO_PI = 2.0f * PI;
            while (angle > PI) angle -= TWO_PI;
            while (angle < -PI) angle += TWO_PI;
            return angle;
        }
        
        float angleBetween(const glm::vec3& a, const glm::vec3& b) {
            float dot = glm::dot(normalize(a), normalize(b));
            return std::acos(clamp(dot, -1.0f, 1.0f));
        }
        
        glm::vec3 rotateVector(const glm::vec3& v, float angle, const glm::vec3& axis) {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, normalize(axis));
            return glm::vec3(rotation * glm::vec4(v, 0.0f));
        }
        
        // Random utilities
        float randomFloat(float min, float max) {
            std::uniform_real_distribution<float> dist(min, max);
            return dist(gen);
        }
        
        int randomInt(int min, int max) {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(gen);
        }
        
        glm::vec3 randomVector(float minLength, float maxLength) {
            const float PI = 3.14159265359f;
            const float TWO_PI = 2.0f * PI;
            float theta = randomFloat(0.0f, TWO_PI);
            float phi = randomFloat(0.0f, PI);
            float length = randomFloat(minLength, maxLength);
            
            float x = length * std::sin(phi) * std::cos(theta);
            float y = length * std::sin(phi) * std::sin(theta);
            float z = length * std::cos(phi);
            
            return glm::vec3(x, y, z);
        }
        
        glm::vec3 randomUnitVector() {
            return normalize(randomVector(0.1f, 1.0f));
        }
        
        // Easing functions
        float easeInQuad(float t) {
            return t * t;
        }
        
        float easeOutQuad(float t) {
            return 1.0f - (1.0f - t) * (1.0f - t);
        }
        
        float easeInOutQuad(float t) {
            return t < 0.5f ? 2.0f * t * t : 1.0f - 2.0f * (1.0f - t) * (1.0f - t);
        }
        
        float easeInCubic(float t) {
            return t * t * t;
        }
        
        float easeOutCubic(float t) {
            float temp = 1.0f - t;
            return 1.0f - temp * temp * temp;
        }
        
        float easeInOutCubic(float t) {
            return t < 0.5f ? 4.0f * t * t * t : 1.0f - 4.0f * (1.0f - t) * (1.0f - t) * (1.0f - t);
        }
        
        // Geometric utilities
        bool pointInSphere(const glm::vec3& point, const glm::vec3& center, float radius) {
            return distanceSquared(point, center) <= radius * radius;
        }
        
        bool sphereIntersect(const glm::vec3& center1, float radius1, 
                           const glm::vec3& center2, float radius2) {
            float totalRadius = radius1 + radius2;
            return distanceSquared(center1, center2) <= totalRadius * totalRadius;
        }
        
        glm::vec3 closestPointOnLine(const glm::vec3& point, 
                                   const glm::vec3& lineStart, 
                                   const glm::vec3& lineEnd) {
            glm::vec3 line = lineEnd - lineStart;
            float lineLength = glm::length(line);
            
            if (lineLength < 0.001f) {
                return lineStart;
            }
            
            glm::vec3 lineDir = line / lineLength;
            glm::vec3 toPoint = point - lineStart;
            float projection = glm::dot(toPoint, lineDir);
            
            projection = clamp(projection, 0.0f, lineLength);
            return lineStart + projection * lineDir;
        }
        
        // Statistics
        float average(const std::vector<float>& values) {
            if (values.empty()) return 0.0f;
            return std::accumulate(values.begin(), values.end(), 0.0f) / values.size();
        }
        
        float variance(const std::vector<float>& values) {
            if (values.size() < 2) return 0.0f;
            
            float mean = average(values);
            float sum = 0.0f;
            
            for (float value : values) {
                float diff = value - mean;
                sum += diff * diff;
            }
            
            return sum / (values.size() - 1);
        }
        
        float standardDeviation(const std::vector<float>& values) {
            return std::sqrt(variance(values));
        }
    }
}
