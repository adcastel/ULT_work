/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */

/*
 * See COPYRIGHT in top-level directory.
 */ 

/*  argo_parallel_for.c code mimics the parallel for OpenMP directive
 *  It creates as many streams as user requires and tasks are created and 
 *  assigned by static blocs to each stream. 
 */

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <abt.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
//#include <unistd.h>
//#include <sys/syscall.h>
//#include <sched.h>

#define NUM_ELEMS 536870912 //2GB
#define NUM_XSTREAMS 4
#ifndef VERBOSE
#define TIMES 50
#else
#define TIMES 1
#endif

ABT_pool *g_pools;
/* structure to pass arguments to expand tasks */
typedef struct {
    float * ptr;
    float value;
    int start;
    int end;
} vector_scal_args_t;


void vector_scal(void *arguments)
{
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int mystart=arg->start;
    int myend=arg->end;
   
ABT_xstream xstreams;
ABT_xstream_self(&xstreams);
int rank;
ABT_xstream_self_rank (&rank);
#ifdef VERBOSE

/*cpu_set_t mask;
        CPU_ZERO(&mask);
int ret = sched_getaffinity(0, sizeof(mask), &mask);
printf("Ret %d, tid 0, affinity %x\n", ret, mask);*/
printf("#ES: %d (CPU: %d) , mystart: %d, myend: %d\n", rank,sched_getcpu() ,mystart, myend);

#endif
} 


int main(int argc, char *argv[]) 
{
    int i, j;
    int ntasks;
    int num_xstreams;
    char *str, * endptr;
    double times[TIMES];
    ABT_xstream *xstreams;
    vector_scal_args_t * args;
    struct timeval t_start, t_end;
    
    num_xstreams = argc > 1 ? atoi(argv[1]) : NUM_XSTREAMS;
    if(argc > 2){
        str = argv[2];
    }
    ntasks = argc > 2 ? strtoll(str, &endptr, 10) : NUM_ELEMS;
    g_pools = (ABT_pool *) malloc(sizeof (ABT_pool) * num_xstreams);
    
    xstreams = (ABT_xstream *) malloc(sizeof (ABT_xstream) * num_xstreams);
    args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * ntasks);

    /* initialization */
    ABT_init(argc, argv);
for (i = 0; i < num_xstreams; i++) {
    ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
            &g_pools[i]);
    }
    printf("Main ES: (CPU: %d)\n", sched_getcpu());

    /* ES creation */
    ABT_xstream_self(&xstreams[0]);
    ABT_xstream_set_main_sched_basic(xstreams[0], ABT_SCHED_DEFAULT,
            1, &g_pools[0]);


    for (i = 1; i < num_xstreams; i++) {
               ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1, &g_pools[i],
                ABT_SCHED_CONFIG_NULL, &xstreams[i]);
        ABT_xstream_start(xstreams[i]);
    }
for (int np=0;np<num_xstreams;np++){
	for (int t=0; t<TIMES;t++)
	{
    		gettimeofday(&t_start, NULL);

    		/* Each task is created on the xstream which is going to execute it*/

    		int start = 0;
    		int end = 0;
    		for (j = 0; j < ntasks; j++) {
        		start = end;
    			args[j].start=start;
    			args[j].end=end;
#ifndef ULT
        		ABT_task_create (
#else
        		ABT_thread_create (
#endif
			g_pools[np], vector_scal,(void *)&args[j],
#ifndef ULT
			NULL);
#else
			ABT_THREAD_ATTR_NULL,NULL);
#endif
		}
    
    		gettimeofday(&t_end, NULL);
    		double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) - 
    			(t_start.tv_sec * 1000000 + t_start.tv_usec);
    /*double time_join = (t_end.tv_sec * 1000000 + t_end.tv_usec) - 
    	(t_start2.tv_sec * 1000000 + t_start2.tv_usec);
*/
   		times[t]=(time/1000000.0);

	}
	ABT_thread_yield();
	double min, max, avg, aux, sigma, dev;
	min=times[0];
	max=times[0];
	aux=times[0];
	for(int t=1;t<TIMES;t++){
		if (times[t]<min) min=times[t];
		if (times[t]>max) max=times[t];
		aux+=times[t];
	}
	avg=aux/TIMES;
	for(int t=0;t<TIMES;t++){
		sigma=sigma+((times[t]-avg)*(times[t]-avg));
	}
#ifndef VERBOSE
	dev=sqrt(sigma/(TIMES-1));
#else
	dev=sqrt(sigma);
#endif    
	printf("From 0 to %d %d %f [%f - %f] %f\n", 
            np, ntasks,avg,min,max,dev );

}
    for (i = 1; i < num_xstreams; i++) {
        ABT_xstream_join(xstreams[i]);
    }

    for (i = 1; i < num_xstreams; i++) {
        ABT_xstream_free(&xstreams[i]);
    }
    
    ABT_finalize();
    free(xstreams);
    
    return EXIT_SUCCESS;
}

