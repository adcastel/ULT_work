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
    int start;
    int end;
} vector_scal_args_t;

void vector_scal(void *arguments) {
    int i;
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int mystart = arg->start;
    int myend = arg->end;
    float value = arg->value;
    float * ptr = arg->ptr;
#ifdef VERBOSE
    ABT_xstream xstreams;
    ABT_xstream_self(&xstreams);
    int rank;
    ABT_xstream_self_rank(&rank);
    printf("#ES: %d, mystart: %d, myend: %d\n", rank, mystart, myend);
#endif
    for (i = mystart; i < myend; i++) {
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
    //float sizeMB;
    ABT_pool *g_pools;
    ABT_xstream *xstreams;
    vector_scal_args_t * args;
    struct timeval t_start, t_end, t_start2;
    float *a;
    num_xstreams = argc > 1 ? atoi(argv[1]) : NUM_XSTREAMS;
    if (argc > 2) {
        str = argv[2];
    }
    ntasks = argc > 2 ? strtoll(str, &endptr, 10) : NUM_ELEMS;
   
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
    threads = malloc_ults(num_xstreams);
#ifdef VERBOSE
    printf("De ULTs\n");
#endif
#else 
#ifdef VERBOSE
    printf("De Tasklets\n");
#endif
    ABT_task *tasks;
    tasks = malloc_tasklets(num_xstreams);
#endif    
    a = malloc(sizeof (float)*ntasks);
    for (i = 0; i < ntasks; i++) {
        a[i] = i * 1.0f;
    }

    xstreams = malloc_xstreams(num_xstreams);

    args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * num_xstreams);



    /* ES creation */
    /*If WS is defined, all ES can access to all pools, if not, each ES accesses 
     * to its own pool */
    ABT_xstream_self(&xstreams[0]);
    ABT_xstream_set_main_sched_basic(xstreams[0], ABT_SCHED_DEFAULT,
#ifdef WORKSTEALING
            num_xstreams, &g_pools[0]);
#ifdef VERBOSE
    printf("Para WS \n");
#endif
    //	   printf("He creado %d pools\n", num_xstreams);
#else
            1, &g_pools[0]);
#ifdef VERBOSE
    printf("1 para cada 1 \n");
#endif

#endif
    /*If SINGLEPOOL is defined all ES share the same pool (just one)
     * If WS is defined, all ES can access to all pools, if not, each ES accesses 
     * to its own pool 
     * Else, each ES only access to its own pool*/
    for (i = 1; i < num_xstreams; i++) {
#ifdef SINGLEPOOL
#ifdef VERBOSE
        printf("ES %d pool %d (SP)\n", i, 0);
#endif
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1, &g_pools[0],
#else
#ifdef WORKSTEALING
        //	   printf("Para todos hay %d pools\n", num_xstreams);
#ifdef VERBOSE
        printf("ES %d todas pools (WS) \n", i);
#endif
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, num_xstreams, &g_pools[0],

#else
#ifdef VERBOSE
        printf("ES %d su pool  \n", i);
#endif
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1, &g_pools[i],
#endif
#endif
                ABT_SCHED_CONFIG_NULL, &xstreams[i]);
        ABT_xstream_start(xstreams[i]);
    }

    for (int t = 0; t < TIMES; t++) {
        for (i = 0; i < ntasks; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);


        int bloc = ntasks / (num_xstreams);
        int rest = ntasks % (num_xstreams);
        int start = 0;
        int end = 0;
        for (j = 0; j < num_xstreams; j++) {
            start = end;
            int inc = (j < rest) ? 1 : 0;
            end += bloc + inc;
            args[j].start = start;
            args[j].end = end;
            args[j].value = 0.9f;
            args[j].ptr = a;
            /* If pool is not defined, the task/ULT is created directly on the 
             destination ES else, the task is put it into a pool*/
#ifndef POOL
#ifdef ULT
            ABT_thread_create_on_xstream(xstreams[j], vector_scal, (void *) &args[j],
#else        
            ABT_task_create_on_xstream(xstreams[j], vector_scal, (void *) &args[j],
#endif
#else
#ifdef VERBOSE
            printf("Tarea %d a pool \n", j, 0);
#endif

#ifdef ULT
            ABT_thread_create(
#else
            ABT_task_create(
#endif
#ifdef SINGLEPOOL
                    g_pools[0], vector_scal, (void *) &args[j],
#else
                    g_pools[j], vector_scal, (void *) &args[j],
#endif
#endif
#ifdef ULT
                    ABT_THREAD_ATTR_NULL, &threads[j]);

#else
                    &tasks[j]);
#endif
        }

        ABT_thread_yield();

        gettimeofday(&t_start2, NULL);
        for (i = 0; i < num_xstreams; i++) {
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
        times_join[t] = (time_join / 1000000.0);
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
    min = times[0];
    max = times[0];
    aux = times[0];
    for (int t = 1; t < TIMES; t++) {
        if (times[t] < min) min = times[t];
        if (times[t] > max) max = times[t];
        aux += times[t];
        avgj+=times_join[t];
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
    printf("%d %d %f [%f - %f] %f Join(%f)\n",
            num_xstreams, ntasks, avg, min, max, dev,avgj/TIMES);

    ABT_finalize();
    free(xstreams);
    for (i = 0; i < ntasks; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

    return EXIT_SUCCESS;
}

