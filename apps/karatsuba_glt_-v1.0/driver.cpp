/*
 * driver.cpp
 *
 * Based on the implementation of the Karatsuba algorithm for polynomial
 * multiplication from "Structure Parallel Programming" by Michael McCool,
 * Arch Robison and James Reinders. The original source code is available
 * from http://parallelbook.com/downloads
 *
 * Information on the Karatsuba algorithm can be found at Wikipedia:
 * http://en.wikipedia.org/wiki/Karatsuba_algorithm
 */


/*
static void TimeRoutine(Routine r, const char* what)
{
    // First call is used to choose how many iterations to time.
    static int TrialCount;
    bool firstCall = TrialCount==0;
    static double tBase;
    double t;

    T x[nMax];
    T y[nMax];
    T z[2*nMax];

    FillRandom(x,nMax);
    FillRandom(y,nMax);

    if (firstCall)
        TrialCount=1;

    for(;;)
    {
        unsigned long long t0 = cilk_getticks();
        for (int i=0; i<TrialCount; ++i)
            (*r)(z,x,y,nMax);
        unsigned long long t1 = cilk_getticks();
        t = cilk_ticks_to_seconds(t1-t0);
        if (!firstCall || (t>=MinTime))
            break;
        // Double the number of iterations
        TrialCount*=2;
    }

    if (firstCall)
    {
        // Print table caption and heading
        printf("Timing %d multiplications of %lu-degree polynomials\n\n",TrialCount,(unsigned long)nMax);
        printf("%20s  %s  %s\n","Version","Time", "Speedup");
        tBase = t;
    }
    std::printf("%20s %6.3f %5.2f x\n",what,t,tBase/t);
}

*/

