#ifndef PICMDK_OPENMPWRAPPER_H
#define PICMDK_OPENMPWRAPPER_H


/* This file should be included everywhere instead of <omp.h>.
If the code is build with OpenMP it just includes <omp.h>, otherwise it
provides wrappers for OpenMP functions and #pragma omp.
The code including this file can use OpenMP functions and
pragmas independently of whether it will be build with OpenMP or not. */

#ifdef _OPENMP

#include <omp.h>


namespace picmdk {
namespace utility {

inline bool useOpenMP() { return true; }
inline int getNumThreads() {
    int numThreads = 0;
    #pragma omp parallel
    {
        #pragma omp master
        numThreads = omp_get_num_threads();
    }
    return numThreads;
}

} // namespace picmdk::utility
} // namespace picmdk

#else

typedef void* omp_lock_t;
inline int omp_get_max_threads() { return 1; }
inline int omp_get_thread_num() { return 0; }
inline int omp_get_num_threads() { return 1; }
inline void omp_init_lock(omp_lock_t *) {}
inline void omp_set_lock(omp_lock_t *) {}
inline void omp_unset_lock(omp_lock_t *) {}
inline void omp_destroy_lock(omp_lock_t *) {}

#pragma omp

namespace picmdk {
namespace utility {

inline bool useOpenMP() { return false; }
inline int getNumThreads() { return 1; }

} // namespace picmdk::utility
} // namespace picmdk

#endif

#ifdef _MSC_VER
    #define collapse(N)
#endif

#endif
