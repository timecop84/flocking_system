#pragma once
#include <random>
#include <memory>
#include <mutex>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace utils {

/**
 * Modern random number generator using C++11+ standards
 * Thread-safe singleton implementation with smart pointers
 */
class Random {
private:
    static std::unique_ptr<Random> s_instance;
    static std::once_flag s_initialized;
    
    std::random_device m_rd;
    mutable std::mt19937 m_gen;  // mutable for const methods
    
public:
    Random() : m_gen(m_rd()) {}
    
    // Thread-safe singleton access
    static Random& instance() {
        std::call_once(s_initialized, []() {
            s_instance = std::make_unique<Random>();
        });
        return *s_instance;
    }
    
    // Generate random float in range [-1.0, 1.0]
    float randomNumber() const {
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
        return dist(m_gen);
    }
    
    // Generate random float in range [0.0, 1.0]
    float randomPositiveNumber() const {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(m_gen);
    }
    
    // Generate random float in custom range
    float randomRange(float min, float max) const {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(m_gen);
    }
    
    // Generate random integer in range [min, max]
    int randomInt(int min, int max) const {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(m_gen);
    }
    
    // Generate random 3D vector with components in [-1, 1]
    glm::vec3 randomVec3() const {
        return glm::vec3(randomNumber(), randomNumber(), randomNumber());
    }
    
    // Generate random point within specified bounds
    glm::vec3 randomPoint(float x, float y, float z) const {
        return glm::vec3(
            randomRange(-x, x),
            randomRange(-y, y), 
            randomRange(-z, z)
        );
    }
    
    // Generate random normalized 3D vector (unit sphere)
    glm::vec3 randomNormalizedVec3() const {
        glm::vec3 v = randomVec3();
        return glm::normalize(v);
    }
    
    // Generate random point on unit sphere surface
    glm::vec3 randomUnitSphere() const {
        float theta = randomRange(0.0f, 2.0f * glm::pi<float>());
        float phi = randomRange(0.0f, glm::pi<float>());
        float sinPhi = std::sin(phi);
        
        return glm::vec3(
            sinPhi * std::cos(theta),
            sinPhi * std::sin(theta),
            std::cos(phi)
        );
    }
    
    // Generate random point inside unit sphere
    glm::vec3 randomInsideUnitSphere() const {
        glm::vec3 v;
        do {
            v = randomVec3();
        } while (glm::dot(v, v) > 1.0f);
        return v;
    }
};

}
