#pragma once
#include <random>
#include "Vector.h"

namespace ngl {

class Random {
private:
    static Random* s_instance;
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_dist;
    
public:
    Random() : m_gen(m_rd()), m_dist(-1.0f, 1.0f) {}
    
    static Random* instance() {
        if (!s_instance) {
            s_instance = new Random();
        }
        return s_instance;
    }
    
    float randomNumber() {
        return m_dist(m_gen);
    }
    
    float randomPositiveNumber() {
        std::uniform_real_distribution<float> pos_dist(0.0f, 1.0f);
        return pos_dist(m_gen);
    }
    
    Vector getRandomVec3() {
        return Vector(randomNumber(), randomNumber(), randomNumber());
    }
    
    Vector getRandomVector() {
        return getRandomVec3();
    }
    
    Vector getRandomPoint(float x, float y, float z) {
        std::uniform_real_distribution<float> x_dist(-x, x);
        std::uniform_real_distribution<float> y_dist(-y, y);
        std::uniform_real_distribution<float> z_dist(-z, z);
        return Vector(x_dist(m_gen), y_dist(m_gen), z_dist(m_gen));
    }
    
    Vector getRandomNormalizedVec3() {
        Vector v = getRandomVec3();
        v.normalize();
        return v;
    }
};

}
