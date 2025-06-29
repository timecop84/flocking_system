#pragma once

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace flock {

/**
 * Modern random number generator using standard C++ and GLM
 * Thread-safe, stateless design - no singletons needed
 */
class Random {
private:
    mutable std::random_device m_rd;
    mutable std::mt19937 m_gen;
    
public:
    Random() : m_gen(m_rd()) {}
    
    // Basic random numbers
    float randomFloat(float min = -1.0f, float max = 1.0f) const {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(m_gen);
    }
    
    float randomPositive(float max = 1.0f) const {
        return randomFloat(0.0f, max);
    }
    
    int randomInt(int min, int max) const {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(m_gen);
    }
    
    // Vector generation using GLM
    glm::vec3 randomVec3(float min = -1.0f, float max = 1.0f) const {
        return glm::vec3(
            randomFloat(min, max),
            randomFloat(min, max),
            randomFloat(min, max)
        );
    }
    
    glm::vec3 randomPointInBox(const glm::vec3& size) const {
        return glm::vec3(
            randomFloat(-size.x, size.x),
            randomFloat(-size.y, size.y),
            randomFloat(-size.z, size.z)
        );
    }
    
    glm::vec3 randomNormalizedVec3() const {
        return glm::normalize(randomVec3());
    }
    
    // Random point on unit sphere surface
    glm::vec3 randomOnUnitSphere() const {
        float theta = randomFloat(0.0f, glm::two_pi<float>());
        float phi = randomFloat(0.0f, glm::pi<float>());
        float sinPhi = std::sin(phi);
        
        return glm::vec3(
            sinPhi * std::cos(theta),
            sinPhi * std::sin(theta),
            std::cos(phi)
        );
    }
    
    // Random point inside unit sphere
    glm::vec3 randomInUnitSphere() const {
        glm::vec3 v;
        do {
            v = randomVec3();
        } while (glm::dot(v, v) > 1.0f);
        return v;
    }
    
    // Random color
    glm::vec4 randomColor(float alpha = 1.0f) const {
        return glm::vec4(randomPositive(), randomPositive(), randomPositive(), alpha);
    }
};

// Global random instance for convenience (thread_local for thread safety)
thread_local static Random g_random;

// Convenience functions
inline float randomFloat(float min = -1.0f, float max = 1.0f) {
    return g_random.randomFloat(min, max);
}

inline glm::vec3 randomVec3(float min = -1.0f, float max = 1.0f) {
    return g_random.randomVec3(min, max);
}

inline glm::vec3 randomNormalizedVec3() {
    return g_random.randomNormalizedVec3();
}

}
