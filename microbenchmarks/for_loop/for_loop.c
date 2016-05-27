#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <glt.h>
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

void vector_scal(void *arguments) {
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int mystart = arg->start;
    int myend = arg->end;
    float value = arg->value;
    float * ptr = arg->ptr;

#ifdef VERBOSE

    printf("#Thread: %d (CPU: %d) mystart: %d, myend: %d\n", glt_get_thread_num(), sched_getcpu(), mystart, myend);

#endif

for (int i = mystart; i < myend; i++) {
        ptr[i] *= value;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int status;
    int ntasks;
    char *str, * endptr;
    double times[TIMES];
    double times_join[TIMES];
    int num_shepherds, num_workers;
    vector_scal_args_t * args;
    struct timeval t_start, t_start2, t_end;
    float *a;
    if (argc > 1) {
        str = argv[1];
    }
    ntasks = argc > 1 ? strtoll(str, &endptr, 10) : NUM_ELEMS;

    a = malloc(sizeof (float)*ntasks);
    for (int i = 0; i < ntasks; i++) {
        a[i] = i * 1.0f;
    }

    glt_init(argc,argv);
    args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * ntasks);

    int num_threads = glt_get_num_threads();

#ifdef TASK
    GLT_tasklet * tasklets;
    tasklets = glt_tasklet_malloc(num_threads);  

#else
    GLT_ult * ults;
    ults = glt_ult_malloc(num_threads);  
#endif
    for (int t = 0; t < TIMES; t++) {
        for (int i = 0; i < ntasks; i++) {
            a[i] = i * 1.0f;
        }
        //for (int j = 0; j < num_workers; j++) {
            //returned_values[j]=NULL;
        //}
        gettimeofday(&t_start, NULL);

        /* Each task is created on the xstream which is going to execute it*/
        int bloc = ntasks / (num_threads);
        int rest = ntasks % (num_threads);
        int start = 0;
        int end = 0;
        for (int j = 0; j < num_threads; j++) {
            start = end;
            int inc = (j < rest) ? 1 : 0;
            end += bloc + inc;
            args[j].start = start;
            args[j].end = end;
            args[j].value = 0.9f;
            args[j].ptr = a;
#ifdef TASK
            glt_tasklet_create_to(vector_scal, (void *) &args[j],&tasklets[j],j%num_threads);        
#else
            glt_ult_create_to(vector_scal, (void *) &args[j],&ults[j],j%num_threads);        
#endif
        }
        glt_yield();
        gettimeofday(&t_start2, NULL);
        for (int j = 0; j < num_workers; j++) {
#ifdef TASK
            glt_tasklet_join(&tasklets[j]);
#else
            glt_ult_join(&ults[j]);
#endif
        }
        
        gettimeofday(&t_end, NULL);

        double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                (t_start.tv_sec * 1000000 + t_start.tv_usec);
        times[t] = (time / 1000000.0);
	double time_join = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
            (t_start2.tv_sec * 1000000 + t_start2.tv_usec);
        times_join[t] = (time_join / 1000000.0);

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
            num_threads, ntasks, avg, min, max, dev,avgj/TIMES);
    
    for (int i = 0; i < ntasks; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

    //printf("greeter returned %lu\n", (unsigned long) return_value);

    return EXIT_SUCCESS;
}



