#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif

int main() {
    int nthreads = 1;
    #pragma omp parallel
    {
        #ifdef _OPENMP
        nthreads = omp_get_num_threads();
        #endif
    }
    std::cout << "OpenMP threads: " << nthreads << std::endl;
    return 0;
}