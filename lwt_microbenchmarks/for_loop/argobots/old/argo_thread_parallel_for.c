/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */

/*
 * See COPYRIGHT in top-level directory.
 */ 

/*  argo_parallel_for.c code mimics the parallel for OpenMP directive
 *  It creates as many streams as user requires and tasks are created and 
 *  assigned by static blocs to each stream. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <abt.h>
#include <math.h>
#include <sys/time.h>

#define NUM_ELEMS 536870912 //2GB
#define NUM_XSTREAMS 4
#define TIMES 50


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
    int i;
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int mystart=arg->start;
    int myend=arg->end;
    float value=arg->value;
    float * ptr=arg->ptr;
#ifdef VERBOSE
ABT_xstream xstreams;
ABT_xstream_self(&xstreams);
int rank;
ABT_xstream_self_rank (&rank);
printf("#ES: %d, mystart: %d, myend: %d\n", rank, mystart, myend);
#endif

    for(i=mystart;i<myend;i++){   
    	ptr[i]*=value;
    }
} 


int main(int argc, char *argv[]) 
{
    int i, j;
    int ntasks;
    int num_xstreams;
    char *str, * endptr;
    double times[TIMES];
    //float sizeMB;
    ABT_xstream *xstreams;
    vector_scal_args_t * args;
    struct timeval t_start, t_end;
    float *a;
ABT_thread *threads;
    num_xstreams = argc > 1 ? atoi(argv[1]) : NUM_XSTREAMS;
    if(argc > 2){
        str = argv[2];
    }
    ntasks = argc > 2 ? strtoll(str, &endptr, 10) : NUM_ELEMS;

#ifdef SINGLEPOOL
    g_pools = (ABT_pool *) malloc(sizeof (ABT_pool) * 1);
#else
    g_pools = (ABT_pool *) malloc(sizeof (ABT_pool) * num_xstreams);
#endif

    threads = (ABT_thread *) malloc (sizeof(ABT_thread)*num_xstreams);
   
    a = malloc(sizeof (float)*ntasks);
    for (i = 0; i < ntasks; i++) {
        a[i] = i * 1.0f;
    }

    xstreams = (ABT_xstream *) malloc(sizeof (ABT_xstream) * num_xstreams);
    args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * num_xstreams);

    /* initialization */
    ABT_init(argc, argv);
#ifdef SINGLEPOOL
        ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
            &g_pools[0]);

#else

for (i = 0; i < num_xstreams; i++) {
    ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
            &g_pools[i]);
    }
#endif
    /* ES creation */
    ABT_xstream_self(&xstreams[0]);
ABT_xstream_set_main_sched_basic(xstreams[0], ABT_SCHED_DEFAULT,
            1, &g_pools[0]);

    for (i = 1; i < num_xstreams; i++) {
#ifdef SINGLEPOOL
               ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1, &g_pools[0],
#else
               ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1, &g_pools[i],
#endif
   
          ABT_SCHED_CONFIG_NULL, &xstreams[i]);
        ABT_xstream_start(xstreams[i]);
    }

for (int t=0; t<TIMES;t++)
{
    for (i = 0; i < ntasks; i++) {
        a[i] = i * 1.0f;
    }


    gettimeofday(&t_start, NULL);

    /* Each task is created on the xstream which is going to execute it*/

    int bloc = ntasks / (num_xstreams);
    int rest = ntasks % (num_xstreams);
    int start = 0;
    int end = 0;
    for (j = 0; j < num_xstreams; j++) {
        start = end;
        int inc = (j < rest) ? 1 : 0;
        end += bloc + inc;
    		args[j].start=start;
    		args[j].end=end;
		args[j].value=0.9f;
		args[j].ptr=a;
#ifdef VERBOSE
		printf("Tarea %d para ES %d. Desde it %d a %d\n", j, j, start, end);
#endif
#ifndef POOL        
                ABT_thread_create_on_xstream (xstreams[j], vector_scal,
#else
                ABT_thread_create (
#ifdef SINGLEPOOL
        	g_pools[0], vector_scal,
#else

		g_pools[j], vector_scal,
#endif
#endif
			 (void *)&args[j], ABT_THREAD_ATTR_NULL,&threads[j]);
    }
    
    ABT_thread_yield();

//    gettimeofday(&t_start2, NULL);
    for (i = 0; i < num_xstreams; i++) {
 	ABT_thread_free (&threads[i]);
    }
    
    gettimeofday(&t_end, NULL);
    double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) - 
    	(t_start.tv_sec * 1000000 + t_start.tv_usec);
 /*   double time_join = (t_end.tv_sec * 1000000 + t_end.tv_usec) - 
    	(t_start2.tv_sec * 1000000 + t_start2.tv_usec);
*/

    times[t]=(time/1000000.0);
}
    for (i = 1; i < num_xstreams; i++) {
        ABT_xstream_join(xstreams[i]);
    }

    for (i = 1; i < num_xstreams; i++) {
        ABT_xstream_free(&xstreams[i]);
    }

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
dev=sqrt(sigma/(TIMES-1));
    printf("%d %d %f [%f - %f] %f\n",
            num_xstreams, ntasks,avg,min,max,dev );

 
    ABT_finalize();
    free(xstreams);
    for (i = 0; i < ntasks; i++) {
        if (a[i] != i * 0.9f){
            printf("%f\n", a[i]);
	    return 0;
	}
    }
    
    return EXIT_SUCCESS;
}


















