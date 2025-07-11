#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#ifdef vector
#error "vector macro is defined!"
#endif

static_assert(std::is_class<std::vector<int>>::value, "std::vector<int> is not a class!");
