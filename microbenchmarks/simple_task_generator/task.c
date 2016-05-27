#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#ifdef FASTGLT
#include <fast_glt.h>
#else
#include <glt.h>
#endif
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

void vector_scal(void *arguments) {
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int pos = arg->pos;
    int gran = arg->gran;
    int i;
    int posfin=pos+gran;
    float value = arg->value;
    float * ptr = arg->ptr;

#ifdef VERBOSE

    printf("#Thread: %d (CPU: %d) pos: %d\n", glt_get_thread_num(), sched_getcpu(), pos);

#endif
    for(i=pos;i<posfin;i++){
        ptr[i] *= value;
    }
    //return 0;
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
    int granularity = argc > 2 ? atoi(argv[2]) : 1;
    int total=ntasks*granularity;

    a = malloc(sizeof (float)*total);
    for (int i = 0; i < total; i++) {
        a[i] = i * 1.0f;
    }


    args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * ntasks);
glt_init(argc,argv);
#ifdef TASK
GLT_tasklet * tasklets;
tasklets = glt_tasklet_malloc(ntasks);
#else
GLT_ult * ults;
ults = glt_ult_malloc(ntasks);
#endif
int num_threads = glt_get_num_threads();
    
    for (int t = 0; t < TIMES; t++) {
        for (int i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);
	int current_task=0;
        /* Each task is created on the xstream which is going to execute it*/
        for (int j = 0; j < total; j+=granularity) {
            args[current_task].gran = granularity;
            args[current_task].pos = j;
            args[current_task].value = 0.9f;
            args[current_task].ptr = a;
#ifdef TASK
            glt_tasklet_create_to(vector_scal, (void *) &args[current_task],&tasklets[j],j%num_threads);
#else
            glt_ult_create_to(vector_scal, (void *) &args[current_task],&ults[j],j%num_threads);
#endif	   
 current_task++;
	}
        glt_yield();
        gettimeofday(&t_start2, NULL);
        for (int j = 0; j < ntasks; j++) {
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
    
    for (int i = 0; i < total; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

    return EXIT_SUCCESS;
}



