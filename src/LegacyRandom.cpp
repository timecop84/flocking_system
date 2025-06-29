#include "LegacyRandom.h"

Random* Random::s_instance = nullptr;

Random* Random::instance() {
    if (!s_instance) {
        s_instance = new Random();
    }
    return s_instance;
}

float Random::randomPositiveNumber(float range) {
    return m_modernRandom.randomFloat(0.0f, range);
}

float Random::randomNumber(float range) {
    return m_modernRandom.randomFloat(-range, range);
}

glm::vec3 Random::getRandomVec3() {
    return m_modernRandom.randomVec3();
}

glm::vec3 Random::getRandomNormalizedVec3() {
    return glm::normalize(m_modernRandom.randomVec3());
}

glm::vec3 Random::getRandomVec3(float range) {
    return m_modernRandom.randomVec3() * range;
}

glm::vec3 Random::getRandomVector() {
    return getRandomVec3();
}

glm::vec3 Random::getRandomPoint(float x, float y, float z) {
    return glm::vec3(
        m_modernRandom.randomFloat(-x, x),
        m_modernRandom.randomFloat(-y, y),
        m_modernRandom.randomFloat(-z, z)
    );
}
