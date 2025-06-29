#include "../include/Random.h"

namespace utils {

// Static member definitions
std::unique_ptr<Random> Random::s_instance = nullptr;
std::once_flag Random::s_initialized;

}
