/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */

/*
 * See COPYRIGHT in top-level directory.
 */

/* Pragma omp parallel for directive evaluation 
   Output: avg time
 */

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_ELEMS 100 //2GB
#define EXT_LOOP_ELEM 2 //2GB
#define IN_LOOP_ELEM 2 //2GB
#define IN_LOOP_TH 1 //2GB
#ifndef VERBOSE
#define TIMES 50
#else
#define TIMES 1
#endif

/* Vector initialization */
void init(float *v, int n) 
{
    int i = 0;
    for (i = 0; i < n; i++) {
        v[i] = i + 100.0f;
    }
}

/* Called after each test to be sure that the compiler does
   not avoid to execute the test */
void check(float *v, int n) 
{
    int i = 0;
    for (i = 0; i < n; i++) {
        if (v[i] != (i+100.0f)*0.9f) {
            printf("v[%d]<=0.0f\n", i);
        }
    }
}

void vectorscal(float *v,float value,int init,int gran){
	int i=init;
	int ii=i+gran;
#ifdef VERBOSE
	printf("Thread %d ejecutando: init=%d end=%d\n",omp_get_thread_num(),i,ii);
#endif
	for(i=init;i<ii;i++){
		v[i]*=value;
	}

}

int main(int argc, char * argv[]) 
{
    int t, i, r, nthreads;
    double *time;
    float *v;
    #pragma omp parallel
    {
    #pragma omp master
        {
            nthreads = omp_get_num_threads();
        }
    }
    int ntasks = (argc > 1) ? atoi(argv[1]) : NUM_ELEMS;
    int granularity = (argc > 2) ? atoi(argv[2]) : 1;
    int rep = (argc > 3) ? atoi(argv[3]) : TIMES;
    time = malloc(sizeof (double)*rep);
    v = malloc(sizeof (float)*ntasks*granularity);
    int total = ntasks*granularity;
    for (r = 0; r < rep; r++) {
    init(v, total);
        time[r] = omp_get_wtime();
	#pragma omp parallel
	{
	        	#pragma omp for
			for (i = 0; i < total; i+=granularity) {
#ifdef VERBOSE
	printf("Thread %d: creando task de init=%d end=%d\n",omp_get_thread_num(),i,i+granularity);
#endif
				#pragma omp task firstprivate(i)
				{
                		//	v[i] *= 0.9f;
                			vectorscal(v,0.9f,i,granularity);
				}
            		}
	}
        time[r] = omp_get_wtime() - time[r];
    }
 double min, max, avg, aux, sigma, dev;
    min = time[0];
    max = time[0];
    aux = time[0];
    for (t = 1; t < TIMES; t++) {
        if (time[t] < min) min = time[t];
        if (time[t] > max) max = time[t];
        aux += time[t];
    }
    avg = aux / TIMES;
    for ( t = 0; t < TIMES; t++) {
        sigma = sigma + ((time[t] - avg)*(time[t] - avg));
    }
#ifndef VERBOSE
    dev = sqrt(sigma / (TIMES - 1));
#else
    dev = sqrt(sigma);
#endif
    printf("%d %d %f [%f - %f] %f\n",
            nthreads, ntasks, avg, min, max, dev);
    check(v, total);
    free(v);
    free(time);
    
    return 0;
}
