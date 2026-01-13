#include "MathUtils.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

namespace math {
    namespace utils {
        
        // Random number generator with robust seeding
        // On Windows, std::random_device often has poor entropy, so we combine multiple sources
        static std::mt19937 createGenerator() {
            std::random_device rd;
            // Combine multiple entropy sources for better randomness
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = now.time_since_epoch();
            auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
            
            // XOR multiple sources together for better seed
            std::seed_seq seed{
                static_cast<unsigned int>(rd()),
                static_cast<unsigned int>(nanos & 0xFFFFFFFF),
                static_cast<unsigned int>((nanos >> 32) & 0xFFFFFFFF),
                static_cast<unsigned int>(reinterpret_cast<uintptr_t>(&rd) & 0xFFFFFFFF)
            };
            return std::mt19937(seed);
        }
        static std::mt19937 gen = createGenerator();
        
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
        
        // Additional random utilities used by flocking system
        glm::vec3 randomVec3() {
            // Return NORMALIZED random direction - ensures consistent behavior
            // Use rejection sampling to avoid bias at cube corners
            glm::vec3 v;
            float lenSq;
            do {
                v = glm::vec3(
                    randomFloat(-1.0f, 1.0f),
                    randomFloat(-1.0f, 1.0f),
                    randomFloat(-1.0f, 1.0f)
                );
                lenSq = glm::dot(v, v);
            } while (lenSq < 0.0001f || lenSq > 1.0f);  // Reject near-zero and outside unit sphere
            
            return glm::normalize(v);
        }
        
        glm::vec3 randomPoint(float x, float y, float z) {
            return glm::vec3(
                randomFloat(-x, x),
                randomFloat(-y, y), 
                randomFloat(-z, z)
            );
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

// ============================================================================
// Vector Class Implementation
// ============================================================================

#include <cmath>

float Vector::length() const {
    return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

float Vector::lengthSquared() const {
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

Vector Vector::normalize() const {
    float len = length();
    if (len > 0.0f) {
        return Vector(m_x / len, m_y / len, m_z / len);
    }
    return Vector(0, 0, 0);
}

void Vector::normalizeIP() {
    float len = length();
    if (len > 0.0f) {
        m_x /= len;
        m_y /= len;
        m_z /= len;
    }
}

float Vector::dot(const Vector& v) const {
    return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
}

Vector Vector::cross(const Vector& v) const {
    return Vector(
        m_y * v.m_z - m_z * v.m_y,
        m_z * v.m_x - m_x * v.m_z,
        m_x * v.m_y - m_y * v.m_x
    );
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << "(" << v.m_x << ", " << v.m_y << ", " << v.m_z << ")";
    return os;
}

Vector operator*(float scalar, const Vector& v) {
    return v * scalar;
}
