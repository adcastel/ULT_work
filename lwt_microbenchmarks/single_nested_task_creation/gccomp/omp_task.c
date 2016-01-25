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
	printf("OMP_THREAD %d calculando de %d a %d\n", omp_get_thread_num(),i,ii);
#endif
	for(i=init;i<ii;i++){
		v[i]*=value;
	}

}

void createTask(float *v,float value, int pos ,int nchildtask, int nlevels,int granularity){

#ifdef VERBOSE
	printf("OMP_THREAD %d en createTask para posicion %d, nchildtask %d, nlevels %d y granularidad %d\n", omp_get_thread_num(),pos ,nchildtask,nlevels,granularity);
#endif
    if (nlevels > 1){

        createTask(v,value, pos ,nchildtask,nlevels-1,granularity);

    }
    else{

        int t;
	int start=pos;
        int stride=(nchildtask*nlevels*granularity)/nchildtask;
        for(t=0;t<nchildtask;t++){
#ifdef VERBOSE
	printf("OMP_THREAD %d en createTask dentro de for con start %d stride %d\n", omp_get_thread_num(),start,stride);
#endif
            #pragma omp task firstprivate(start)
            {
		vectorscal(v,value, start, granularity);		
            }
	    start+=stride;
        }

    }


}

int main(int argc, char * argv[]) 
{
    int i, r, nthreads, t;
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
    int nchildtask = (argc > 2) ? atoi(argv[2]) : 2;
    int nlevels = (argc > 3) ? atoi(argv[3]) : 1;
    int granularity = (argc > 4) ? atoi(argv[4]) : 1;
    int rep = (argc > 5) ? atoi(argv[5]) : TIMES;
    time = malloc(sizeof (double)*rep);
    int total = ntasks*nchildtask*nlevels*granularity;
    int stride=nchildtask*nlevels*granularity;
    v = malloc(sizeof (float)*total);
    for (r = 0; r < rep; r++) {
    init(v, total);
        time[r] = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp single
		{
	        	for (i = 0; i < total; i+=stride) {
				#pragma omp task firstprivate(i)
				{
                		//	v[i] *= 0.9f;
                		//	vectorscal(v,0.9f,i,granularity);
                		        createTask(v,0.9f,i,nchildtask,nlevels,granularity);
				}
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
    for (t = 0; t < TIMES; t++) {
        sigma = sigma + ((time[t] - avg)*(time[t] - avg));
    }
#ifndef VERBOSE
    dev = sqrt(sigma / (TIMES - 1));
#else
    dev = sqrt(sigma);
#endif
    printf("%d %d %f [%f - %f] %f\n",
            nthreads, total, avg, min, max, dev);
    check(v, total);
    free(v);
    free(time);
    
    return 0;
}
