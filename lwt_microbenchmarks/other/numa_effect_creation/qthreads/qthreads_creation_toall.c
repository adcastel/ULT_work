#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <math.h>
#include <sys/time.h>
#ifndef VERBOSE
#define TIMES 50
#else
#define TIMES 1
#endif
#define NUM_ELEMS 10000

typedef struct {
    float * ptr;
    float value;
    int start;
    int end;
} vector_scal_args_t;

static aligned_t vector_scal(void *arguments) {
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int mystart = arg->start;
    int myend = arg->end;

#ifdef VERBOSE

    printf("#Shepherd: %d (CPU: %d) Worker %d, mystart: %d, myend: %d\n", qthread_shep(), qthread_worker(NULL),sched_getcpu(), mystart, myend);

#endif


    return 0;
}

int main(int argc,
        char *argv[]) {
    int status;
    int ntasks;
    char *str, * endptr;
    double times[TIMES];
    int num_shepherds, num_workers;
    vector_scal_args_t * args;
    struct timeval t_start, t_end;

    if (argc > 1) {
        str = argv[1];
    }
    ntasks = argc > 1 ? strtoll(str, &endptr, 10) : NUM_ELEMS;

    args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * ntasks);

    status = qthread_initialize();
    assert(status == QTHREAD_SUCCESS);
    num_shepherds = qthread_num_shepherds();
    num_workers = qthread_num_workers();
    aligned_t *returned_values;
    returned_values = (aligned_t *) malloc(sizeof (aligned_t) * ntasks*num_shepherds);

    //printf("%i shepherds...\n", qthread_num_shepherds());
    //printf("  %i threads total\n", qthread_num_workers());
    for(int np=0;np<num_shepherds;np++){
    for (int t = 0; t < TIMES; t++) {
        gettimeofday(&t_start, NULL);

        /* Each task is created on the xstream which is going to execute it*/

        int start = 0;
        int end = 0;
        for (int j = 0; j < ntasks; j++) {
            start = end;
            args[j].start = start;
            args[j].end = end;

            status = qthread_fork_to(vector_scal, (void *) &args[j], &returned_values[np*ntasks+j],np);
        }
        gettimeofday(&t_end, NULL);
        double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                (t_start.tv_sec * 1000000 + t_start.tv_usec);
        times[t] = (time / 1000000.0);
    }
    double min, max, avg, aux, sigma, dev;
    min = times[0];
    max = times[0];
    aux = times[0];
    for (int t = 1; t < TIMES; t++) {
        if (times[t] < min) min = times[t];
        if (times[t] > max) max = times[t];
        aux += times[t];
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
    printf("From 0 to %d %d %f [%f - %f] %f\n",
            np, ntasks, avg, min, max, dev);
    /*for (int j = 0; j < ntasks; j++) {
        int ret = qthread_readFF(NULL, &returned_values[j]);
    }*/
}
    for(int np=0;np<num_shepherds;np++){
    for (int j = 0; j < ntasks; j++) {
        int ret = qthread_readFF(NULL, &returned_values[np*ntasks+j]);
    }
    }


    //printf("greeter returned %lu\n", (unsigned long) return_value);

    return EXIT_SUCCESS;
}



