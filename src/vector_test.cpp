#include "vector_test.h"

int main() {
    std::vector<int> v = {1, 2, 3};
    for (int i : v) std::cout << i << std::endl;
    return 0;
}
