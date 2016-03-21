/*
 * karatsuba.h
 *
 * Implementations of the Karatsuba algorithm for polynomial multiplication
 *
 * Polynomial multiplication C=A*B.
 * A = a[0:n]
 * B = b[0:n]
 * C = c[0:2*n-1]
 *
 * Based on the implementation of the Karatsuba algorithm for polynomial
 * multiplication from "Structure Parallel Programming" by Michael McCool,
 * Arch Robison and James Reinders. The original source code is available
 * from http://parallelbook.com/downloads
 *
 * Information on the Karatsuba algorithm can be found at Wikipedia:
 * http://en.wikipedia.org/wiki/Karatsuba_algorithm
 */

// Define size_t
#include <fast_glt.h>
// Assumed to hold 32 bits
typedef unsigned int T;


typedef struct glt_args {
	unsigned int * z;
	unsigned int * x;
        unsigned int * y;
        size_t nMax;
   //     int dest;
}glt_args_t;

typedef struct glt_temp_space {

    size_t n;
    T temp[4096/sizeof(T)];
    T* base;
}glt_temp_space_t;
// The four implementations of the Karatsuba algorithm
//void karatsuba_serial (T c[], const T a[], const T b[], size_t n);
//void karatsuba_vectorized (T c[], const T a[], const T b[], size_t n);
//void karatsuba_parallel(T c[], const T a[], const T b[], size_t n);
void karatsuba_glt(void * arguments);

//void karatsuba_parallel_vectorized (T c[], const T a[], const T b[], size_t n);
