/*
 * karatsuba.cpp
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

#include <cilk/cilk.h>

#include "karatsuba.h"
#include "temp_space.h"

// Cutoff from Karatsuba algorithm to simple alorithm
const size_t CutOff = 256;

/*
 * Simple polynomial multiplication C=A*B that takes quadratic time. This
 * routine is used for the "small n" base case in Karatsuba multiplication.
 *
 * Simple implementation using loops.
 */
static
void simple_mul(T c[], const T a[], const T b[], size_t n)
{
    for (size_t j=0; j<2*n-1; ++j)
        c[j] = 0;

    for (size_t i=0; i<n; ++i)
        for (size_t j=0; j<n; ++j)
            c[i+j] += a[i]*b[j];
}

/*
 * Simple polynomial multiplication C=A*B that takes quadratic time. This
 * routine is used for the "small n" base case in Karatsuba multiplication.
 *
 * Implementation using array notation.
 */
static
void vector_mul(T c[], const T a[], const T b[], size_t n)
{
    c[0:2*n-1] = 0;

    for (size_t i=0; i<n; ++i)
        c[i:n] += a[i]*b[0:n];
}

/*
 * Implementation of the Karasuba algorithm for polynomial multiplication
 * using loops
 */
void karatsuba_serial(T c[], const T a[], const T b[], size_t n)
{
    if (n <= CutOff)
    {
        simple_mul(c, a, b, n);
        return;
    }

    size_t m = n/2;

    // Set c[0:n-1] = t_0
    karatsuba_serial(c, a, b, m);

    // Set c[2*m:n-1] = t_2
    karatsuba_serial(c+2*m, a+m, b+m, n-m);

    temp_space<T> s(4*(n-m));
    T *a_=s.data(), *b_=a_+(n-m), *t=b_+(n-m);
    for (size_t j=0; j<m; ++j)
    {
        a_[j] = a[j]+a[m+j];
        b_[j] = b[j]+b[m+j];
    }
    if (n&1)
    {
        a_[m] = a[2*m];
        b_[m] = b[2*m];
    }

    // Set t = t_1
    karatsuba_serial(t, a_, b_, n-m);
   
    // Set t = t_1 - t_0 - t_2
    for (size_t j=0; j<2*m-1; ++j)
        t[j] -= c[j] + c[2*m+j];

    // Add (t_1 - t_0 - t_2) into final product.
    c[2*m-1] = 0;
    for (size_t j=0; j<2*m-1; ++j)
        c[m+j] += t[j];
    if (n&1)
        for( size_t j=0; j<2; ++j )
            c[3*m-1+j] += t[2*m-1+j] - c[4*m-1+j];
}

/*
 * Implementation of the Karasuba algorithm for polynomial multiplication
 * vectorized using array notation
 */
void karatsuba_vectorized (T c[], const T a[], const T b[], size_t n)
{
    if (n<=CutOff)
    {
        vector_mul(c, a, b, n);
        return;
    }

    size_t m = n/2;

    // Set c[0:n-1] = t_0
    karatsuba_vectorized(c, a, b, m);

    // Set c[2*m:n-1] = t_2
    karatsuba_vectorized(c+2*m, a+m, b+m, n-m);

    temp_space<T> s(4*(n-m));
    T *a_=s.data(), *b_=a_+(n-m), *t=b_+(n-m);
    a_[0:m] = a[0:m]+a[m:m];
    b_[0:m] = b[0:m]+b[m:m];
    if (n&1)
    {
        a_[m] = a[2*m];
        b_[m] = b[2*m];
    }

    // Set t = t_1
    karatsuba_vectorized( t, a_, b_, n-m );

    // Set t = t_1 - t_0 - t_2
    t[0:2*m-1] -= c[0:2*m-1] + c[2*m:2*m-1];

    // Add (t_1 - t_0 - t_2) into final product.
    c[2*m-1] = 0;
    c[m:2*m-1] += t[0:2*m-1];
    if (n&1)
        c[3*m-1:2] += t[2*m-1:2] - c[4*m-1:2];
}

/*
 * Implementation of the Karasuba algorithm for polynomial multiplication
 * using task parallelism
 */
void karatsuba_parallel(T c[], const T a[], const T b[], size_t n)
{
    if (n<=CutOff)
    {
        simple_mul( c, a, b, n );
        return;
    }

    size_t m = n/2;

    // Set c[0:n-1] = t_0
    cilk_spawn karatsuba_parallel(c, a, b, m);

    // Set c[2*m:n-1] = t_2
    cilk_spawn karatsuba_parallel(c+2*m, a+m, b+m, n-m);

    temp_space<T> s(4*(n-m));
    T *a_=s.data(), *b_=a_+(n-m), *t=b_+(n-m);
    for (size_t j=0; j<m; ++j)
    {
        a_[j] = a[j]+a[m+j];
        b_[j] = b[j]+b[m+j];
    }
    if (n&1)
    {
        a_[m] = a[2*m];
        b_[m] = b[2*m];
    }

    // Set t = t_1
    karatsuba_parallel(t, a_, b_, n-m);
    cilk_sync;

    // Set t = t_1 - t_0 - t_2
    for (size_t j=0; j<2*m-1; ++j)
        t[j] -= c[j] + c[2*m+j];

    // Add (t_1 - t_0 - t_2) into final product.
    c[2*m-1] = 0;
    for(size_t j=0; j<2*m-1; ++j)
        c[m+j] += t[j];
    if (n&1)
        for (size_t j=0; j<2; ++j)
            c[3*m-1+j] += t[2*m-1+j] - c[4*m-1+j];
}
/*
void karatsuba_glt(void * arguments)
{
    glt_args *in_arguments = (glt_args *) arguments;
    T *c = in_arguments->z;
    T *a = in_arguments->x;
    T *b = in_arguments->y;
    size_t n = in_arguments->nMax;
    if (n<=CutOff)
    {
        simple_mul( c, a, b, n );
        return;
    }

    size_t m = n/2;

    // Set c[0:n-1] = t_0
    int id;
    glt_workunit_get_thread_id(&id);
    int dest = (id+1)%glt_get_num_threads(); 

    GLT_ult * ults;
    ults = glt_ult_malloc(2);
    glt_args args[2];
    
    args[0].z = c;
    args[0].x = a;
    args[0].y = b;
    args[0].nMax = m;
    //cilk_spawn karatsuba_glt(c, a, b, m);
    glt_ult_creation_to(karatsuba_glt,(void *)&args[0],&ults[0],dest);

    // Set c[2*m:n-1] = t_2
    //cilk_spawn karatsuba_glt(c+2*m, a+m, b+m, n-m);
    args[1].z = c+2*m;
    args[1].x = a+m;
    args[1].y = b+m;
    args[1].nMax = n-m;
    glt_ult_creation_to(karatsuba_glt,(void *)&args[1],&ults[1],dest);


    temp_space<T> s(4*(n-m));
    T *a_=s.data(), *b_=a_+(n-m), *t=b_+(n-m);
    for (size_t j=0; j<m; ++j)
    {
        a_[j] = a[j]+a[m+j];
        b_[j] = b[j]+b[m+j];
    }
    if (n&1)
    {
        a_[m] = a[2*m];
        b_[m] = b[2*m];
    }

    // Set t = t_1
    glt_args call;
    call.z=t;
    call.x=a_;
    call.y=b_;
    call.nMax=n-m;
    karatsuba_glt((void *)&call);
//    cilk_sync;

    glt_ult_join(&ults[0]);
    glt_ult_join(&ults[1]);
    // Set t = t_1 - t_0 - t_2
    for (size_t j=0; j<2*m-1; ++j)
        t[j] -= c[j] + c[2*m+j];

    // Add (t_1 - t_0 - t_2) into final product.
    c[2*m-1] = 0;
    for(size_t j=0; j<2*m-1; ++j)
        c[m+j] += t[j];
    if (n&1)
        for (size_t j=0; j<2; ++j)
            c[3*m-1+j] += t[2*m-1+j] - c[4*m-1+j];
}
*/


/*
 * Implementation of the Karasuba algorithm for polynomial multiplication
 * using both data and task parallelism
 */
void karatsuba_parallel_vectorized (T c[], const T a[], const T b[], size_t n)
{
    if (n<=CutOff)
    {
        vector_mul(c, a, b, n);
        return;
    }

    size_t m = n/2;

    // Set c[0:n-1] = t_0
    cilk_spawn karatsuba_parallel_vectorized(c, a, b, m);

    // Set c[2*m:n-1] = t_2
    cilk_spawn karatsuba_parallel_vectorized(c+2*m, a+m, b+m, n-m);

    temp_space<T> s(4*(n-m));
    T *a_=s.data(), *b_=a_+(n-m), *t=b_+(n-m);
    a_[0:m] = a[0:m]+a[m:m];
    b_[0:m] = b[0:m]+b[m:m];
    if (n&1)
    {
        a_[m] = a[2*m];
        b_[m] = b[2*m];
    }

    // Set t = t_1
    karatsuba_parallel_vectorized(t, a_, b_, n-m);
    cilk_sync;

    // Set t = t_1 - t_0 - t_2
    t[0:2*m-1] -= c[0:2*m-1] + c[2*m:2*m-1];

    // Add (t_1 - t_0 - t_2) into final product.
    c[2*m-1] = 0;
    c[m:2*m-1] += t[0:2*m-1];
    if(n&1)
        c[3*m-1:2] += t[2*m-1:2] - c[4*m-1:2];
}
