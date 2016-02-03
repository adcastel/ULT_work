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
#ifndef VERBOSE
#define TIMES 50
#else
#define TIMES 1
#endif

/* structure to pass arguments to expand tasks */
typedef struct {
    float * ptr;
    float value;
    int pos;
    int gran;
} vector_scal_args_t;

void vector_scal(void *arguments) {
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int pos = arg->pos;
    int gran = arg->gran;
    int posfin=pos+gran;
    float value = arg->value;
    float * ptr = arg->ptr;
    int i;
#ifdef VERBOSE
    ABT_xstream xstreams;
    ABT_xstream_self(&xstreams);
    int rank;
    ABT_xstream_self_rank(&rank);
    printf("#ES: %d, pos: %d\n", rank, pos);
#endif
    for(i=pos;i<posfin;i++){
        ptr[i] *= value;
    }
}

ABT_xstream * malloc_xstreams(int num_xstreams) {
    ABT_xstream * xstreams = (ABT_xstream *) malloc(sizeof (ABT_xstream) * num_xstreams);
    return xstreams;
}

ABT_pool * malloc_pools(int num_pools) {
    ABT_pool * pools = (ABT_pool *) malloc(sizeof (ABT_pool) * num_pools);
    
    for (int i = 0; i < num_pools; i++) {
        ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
                &pools[i]);
    }
    return pools;
}

ABT_thread * malloc_ults(int num_ult) {
    ABT_thread * ults = (ABT_thread *) malloc(sizeof (ABT_thread) * num_ult);
    return ults;
}

ABT_task * malloc_tasklets(int num_task) {
    ABT_task * tasklets = (ABT_task *) malloc(sizeof (ABT_task) * num_task);
    return tasklets;
}

int main(int argc, char *argv[]) {
    int i, j;
    int ntasks;
    int num_xstreams;
    char *str, * endptr;
    double times[TIMES];
    double times_join[TIMES];
    double times_crea[TIMES];
    //float sizeMB;
    ABT_pool *g_pools;
    ABT_xstream *xstreams;
    vector_scal_args_t * args;
    struct timeval t_start, t_end, t_start2, t_end_crea;
    float *a;
    num_xstreams = argc > 1 ? atoi(argv[1]) : NUM_XSTREAMS;
    if (argc > 2) {
        str = argv[2];
    }
    ntasks = argc > 2 ? strtoll(str, &endptr, 10) : NUM_ELEMS;
    int granularity = argc > 3 ? atoi(argv[3]) : 1;
    int total=ntasks*granularity;
     /* initialization */
     ABT_init(argc, argv);
    
    /*Create pools. If SINGLEPOOL is defined all ES are going to share it.
     If not, a pool for each ES is created
     */

#ifdef SINGLEPOOL
#ifdef VERBOSE
    printf("Creo una sola piscina (singlepool)\n");
#endif
    g_pools = malloc_pools(1);
#else
#ifdef VERBOSE
    printf("Creo una piscina por cada xstream\n");
#endif
    g_pools = malloc_pools(num_xstreams);
#endif    

    /*If ULT is defined, the test creates ULTs instead of tasklets*/
#ifdef ULT
    ABT_thread *threads;
    threads = malloc_ults(ntasks);
#ifdef VERBOSE
    printf("De ULTs\n");
#endif
#else 
#ifdef VERBOSE
    printf("De Tasklets\n");
#endif
    ABT_task *tasks;
    tasks = malloc_tasklets(ntasks);
#endif    
    a = malloc(sizeof (float)*total);
    for (i = 0; i < total; i++) {
        a[i] = i * 1.0f;
    }

    xstreams = malloc_xstreams(num_xstreams);

    args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * ntasks);



    /* ES creation */
    /*EachES accesses 
     * to its own pool */
    ABT_xstream_self(&xstreams[0]);
    ABT_xstream_set_main_sched_basic(xstreams[0], ABT_SCHED_DEFAULT,
            1, &g_pools[0]);
#ifdef VERBOSE
    printf("1 para cada 1 \n");
#endif

    /*If SINGLEPOOL is defined all ES share the same pool (just one)
     * Else, each ES only access to its own pool*/
    for (i = 1; i < num_xstreams; i++) {
#ifdef SINGLEPOOL
#ifdef VERBOSE
        printf("ES %d pool %d (SP)\n", i, 0);
#endif
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1, &g_pools[0],
#else
#ifdef VERBOSE
        printf("ES %d su pool  \n", i);
#endif
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1, &g_pools[i],
#endif
                ABT_SCHED_CONFIG_NULL, &xstreams[i]);
        ABT_xstream_start(xstreams[i]);
    }

    for (int t = 0; t < TIMES; t++) {
        for (i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);
        int current_task=0;
        for (j = 0; j < total; j=j+granularity) {

            args[current_task].pos = j;
            args[current_task].gran = granularity;
            args[current_task].value = 0.9f;
            args[current_task].ptr = a;
            /* If pool is not defined, the task/ULT is created directly on the 
             destination ES else, the task is put it into a pool*/
#ifndef POOL
#ifdef ULT
            ABT_thread_create_on_xstream(xstreams[current_task%num_xstreams], vector_scal, (void *) &args[current_task],
#else        
            ABT_task_create_on_xstream(xstreams[current_task%num_xstreams], vector_scal, (void *) &args[current_task],
#endif
#else
#ifdef VERBOSE
            printf("Tarea %d a pool %d\n", current_task, 0);
#endif

#ifdef ULT
            ABT_thread_create(
#else
            ABT_task_create(
#endif
#ifdef SINGLEPOOL
                    g_pools[0], vector_scal, (void *) &args[current_task],
#else
                    g_pools[current_task%num_xstreams], vector_scal, (void *) &args[current_task],
#endif
#endif
#ifdef ULT
                    ABT_THREAD_ATTR_NULL, &threads[current_task]);

#else
                    &tasks[current_task]);
#endif
        current_task++;
	}

        gettimeofday(&t_end_crea, NULL);
        ABT_thread_yield();

        gettimeofday(&t_start2, NULL);
        for (i = 0; i < ntasks; i++) {
#ifdef ULT
            ABT_thread_free(&threads[i]);

#else
            ABT_task_free(&tasks[i]);
#endif    
        }

        gettimeofday(&t_end, NULL);
        double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                (t_start.tv_sec * 1000000 + t_start.tv_usec);
        double time_join = (t_end.tv_sec * 1000000 + t_end.tv_usec) - 
            (t_start2.tv_sec * 1000000 + t_start2.tv_usec);
        double time_crea = (t_end_crea.tv_sec * 1000000 + t_end_crea.tv_usec) - 
            (t_start.tv_sec * 1000000 + t_start.tv_usec);
        times_join[t] = (time_join / 1000000.0);
        times_crea[t] = (time_crea / 1000000.0);
        times[t] = (time / 1000000.0);
    }

    for (i = 1; i < num_xstreams; i++) {
        ABT_xstream_join(xstreams[i]);
    }

    for (i = 1; i < num_xstreams; i++) {
        ABT_xstream_free(&xstreams[i]);
    }
    double min, max, avg, aux, sigma, dev;
    double avgj=times_join[0];
    double avgcrea=times_crea[0];
    min = times[0];
    max = times[0];
    aux = times[0];
    for (int t = 1; t < TIMES; t++) {
        if (times[t] < min) min = times[t];
        if (times[t] > max) max = times[t];
        aux += times[t];
        avgj+=times_join[t];
        avgcrea+=times_crea[t];
    }
    avg = aux / TIMES;
    for (int t = 0; t < TIMES; t++) {
        sigma = sigma + ((times[t] - avg)*(times[t] - avg));
    }
#ifndef VERBOSE
    dev = sqrt(sigma / (TIMES - 1));
#else
    dev = sqrt(sigma);
#endif    
    printf("%d %d %f [%f - %f] %f Crea (%f) Join(%f)\n",
            num_xstreams, ntasks, avg, min, max, dev,avgcrea/TIMES,avgj/TIMES);

    ABT_finalize();
    free(xstreams);
    for (i = 0; i < total; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

    return EXIT_SUCCESS;
}

