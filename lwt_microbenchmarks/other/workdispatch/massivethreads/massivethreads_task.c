#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <myth.h>
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
    int pos;
    int gran;
} vector_scal_args_t;

void * vector_scal(void *arguments) {
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int pos = arg->pos;
    int gran = arg->gran;
    int i;
    int posfin = pos+gran;
    float value = arg->value;
    float * ptr = arg->ptr;

#ifdef VERBOSE

    printf("#Worker: %ld (CPU: %d), pos: %d\n", myth_get_worker_num(), sched_getcpu(), pos);

#endif
    for(i=pos;i<posfin;i++){
        ptr[i] *= value;
    }
}

int main(int argc, char *argv[]) {
    int ntasks;
    char *str, * endptr;
    double times[TIMES];
    double times_join[TIMES];
    double times_crea[TIMES];
    int num_workers;
    vector_scal_args_t * args;
    struct timeval t_start, t_start2, t_end, t_end_crea;
    float *a;

    if (argc > 1) {
        str = argv[1];
    }
    ntasks = argc > 1 ? strtoll(str, &endptr, 10) : NUM_ELEMS;
    int granularity = argc > 2 ? atoi(argv[2]) : 1;
    int total=ntasks*granularity;

    //if (argc > 1) {
    //    str = argv[1];
    //}
    //num_workers = argc > 1 ? strtoll(str, &endptr, 10) : 1;
    a = malloc(sizeof (float)*total);
    for (int i = 0; i < total; i++) {
        a[i] = i * 1.0f;
    }

    myth_init();

    num_workers=atoi(getenv("MYTH_WORKER_NUM"));
    myth_thread_t * workers;
    workers = (myth_thread_t *)malloc(sizeof(myth_thread_t)*ntasks);

    args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * ntasks);

    

    for (int t = 0; t < TIMES; t++) {
        for (int i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);
        int current_task=0;
        /* Each task is created on the xstream which is going to execute it*/
        for (int j = 0; j < total; j+=granularity) {
            args[current_task].pos = j;
            args[current_task].gran = granularity;
            args[current_task].value = 0.9f;
            args[current_task].ptr = a;
//	    printf("Tarea creada\n");            
	    workers[current_task]=myth_create(vector_scal, (void *) &args[current_task]);

        current_task++;
	}
        gettimeofday(&t_end_crea, NULL);
	myth_yield(0);
        gettimeofday(&t_start2, NULL);
        for (int j = 0; j < ntasks; j++) {
           myth_join(workers[j],NULL); 
        }
        
        gettimeofday(&t_end, NULL);

        double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                (t_start.tv_sec * 1000000 + t_start.tv_usec);
        times[t] = (time / 1000000.0);
	double time_join = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
            (t_start2.tv_sec * 1000000 + t_start2.tv_usec);
        times_join[t] = (time_join / 1000000.0);
        double time_crea = (t_end_crea.tv_sec * 1000000 + t_end_crea.tv_usec) -
                (t_start.tv_sec * 1000000 + t_start.tv_usec);
        times_crea[t] = (time_crea / 1000000.0);

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
        avgj+=times_crea[t];
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
            num_workers, ntasks, avg, min, max, dev,avgcrea/TIMES,avgj/TIMES);
    
    for (int i = 0; i < total; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

    //printf("greeter returned %lu\n", (unsigned long) return_value);
myth_fini();

    return EXIT_SUCCESS;
}



