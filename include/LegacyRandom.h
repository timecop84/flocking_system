#ifndef LEGACY_RANDOM_H
#define LEGACY_RANDOM_H

#include "Random.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief Legacy Random compatibility wrapper
/// @details Provides singleton interface to maintain compatibility with old code
//----------------------------------------------------------------------------------------------------------------------
class Random {
public:
    static Random* instance();
    
    // Legacy interface methods
    float randomPositiveNumber(float range);
    float randomNumber(float range);
    glm::vec3 getRandomVec3();
    glm::vec3 getRandomNormalizedVec3();
    glm::vec3 getRandomVec3(float range);
    glm::vec3 getRandomVector(); // Alias for getRandomVec3
    glm::vec3 getRandomPoint(float x, float y, float z);
    
private:
    Random() = default;
    static Random* s_instance;
    flock::Random m_modernRandom;
};

#endif // LEGACY_RANDOM_H
