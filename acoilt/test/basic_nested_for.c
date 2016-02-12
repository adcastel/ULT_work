#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <acoilt.h>
#include <math.h>
#include <sys/time.h>
#ifndef VERBOSE
#define TIMES 50
#else
#define TIMES 1
#endif
#define NUM_ELEMS 10000


/* structure to pass arguments to expand tasks */
typedef struct {
    float * ptr;
    float value;
    int niterations;
    int nthreads;
    int it_start;
    int it_end;
} outerloop_args_t;

typedef struct {
    float * ptr;
    float value;
    int outer_pos;
    int inner_end_pos;
    int inner_start_pos;
    int niterations;
} innerloop_args_t;


void * vector_scal(void *arguments) {
    innerloop_args_t *arg;
    arg = (innerloop_args_t *) arguments;
    int outer_pos = arg->outer_pos;
    int inner_start_pos = arg->inner_start_pos;
    int inner_end_pos = arg->inner_end_pos;
    float value = arg->value;
    float * ptr = arg->ptr;
    int niterations = arg->niterations;
    int i;

#ifdef VERBOSE

    printf("#Shepherd: %d (CPU: %d) Worker %d, pos_ini: %d, pos_fin: %d\n", qthread_shep(),sched_getcpu(),qthread_worker(NULL), inner_start_pos, inner_end_pos);

#endif
    for(i=inner_start_pos;i<inner_end_pos;i++){
        ptr[outer_pos*niterations+i] *= value;
    }

    //return 0;
}

void * task_creator(void *arguments){

    int status;
    int i;
    int nthreads;
    int it_start, it_end, niterations;
    outerloop_args_t *in_arg;
    in_arg = (outerloop_args_t *) arguments;
    it_start=in_arg->it_start;
    it_end=in_arg->it_end;
    nthreads = in_arg->nthreads;
    niterations = in_arg->niterations;
    float value = in_arg->value;
    float * ptr = in_arg->ptr;

#ifdef VERBOSE
    printf("#Shepherd: %d (CPU: %d) en task_creator y creare %d tareas para el rango de iteraciones de %d a %d del loop externo\n", qthread_shep(), sched_getcpu(), nthreads, it_start, it_end);
#endif
 
    innerloop_args_t * out_args = (innerloop_args_t *) malloc(sizeof (innerloop_args_t) * nthreads);
    /*aligned_t *returned_values;
    returned_values = (aligned_t *) malloc(sizeof (aligned_t) * nthreads);
*/
    ACOILT_ult * ults;
    ults = acoilt_ult_malloc(nthreads);
    int ct; //current_task
    int start, end;
    int bloc = niterations / nthreads;
    int rest = niterations % nthreads;
    int j;

    for(i=it_start;i<it_end;i++){

        start=end=0;

        for (ct = 0; ct < nthreads; ct++) {

            start=end;
            end=start+bloc;
            if(ct<rest){end++;}
            out_args[ct].outer_pos=i;
            out_args[ct].inner_start_pos=start;
            out_args[ct].inner_end_pos=end;
            out_args[ct].value=value;
            out_args[ct].ptr=ptr;
            out_args[ct].niterations=niterations;
	
            acoilt_ult_creation_to(vector_scal, (void *) &out_args[ct],&ults[ct],acoilt_get_thread_num());

#ifdef VERBOSE
            printf("#Shepherd: %d creada la tarea %d\n", qthread_shep(), ct);
#endif
        }

        acoilt_yield();

        for (j = 0; j < nthreads; j++) {
            acoilt_ult_join(&ults[j]);
        }
    }
    
    return 0;
}




int main(int argc, char *argv[]) {
    int status;
    int niterations;
    int j;
    int start, end;
    char *str, * endptr;
    double times[TIMES];
    double times_join[TIMES];
    int num_shepherds, num_workers;
    outerloop_args_t * args;
    struct timeval t_start, t_start2, t_end;
    float *a;
    if (argc > 1) {
        str = argv[1];
    }
    niterations = argc > 1 ? strtoll(str, &endptr, 10) : NUM_ELEMS;
    //int granularity = argc > 2 ? atoi(argv[2]) : 1;
    //int total=ntasks*granularity;
    int total=niterations*niterations;

    a = malloc(sizeof (float)*total);
    for (int i = 0; i < total; i++) {
        a[i] = i * 1.0f;
    }



    /*status = qthread_initialize();
    assert(status == QTHREAD_SUCCESS);
    num_shepherds = qthread_num_shepherds();
    num_workers = qthread_num_workers();
    aligned_t *returned_values;
    returned_values = (aligned_t *) malloc(sizeof (aligned_t) * num_workers);
    */
acoilt_init(argc,argv);

ACOILT_ult * ults;

num_workers = acoilt_get_num_threads();
ults = acoilt_ult_malloc(num_workers);

     args = (outerloop_args_t *) malloc(sizeof (outerloop_args_t)
            * num_workers);

    for (int t = 0; t < TIMES; t++) {
        for (int i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);
	
	 /* Work here */
        start=end=0;
        int bloc = niterations / num_workers;
        int rest = niterations % num_workers;

        for (j = 0; j < num_workers; j++) {
                start=end;
                end=start+bloc;
                if(j<rest){end++;}
	        args[j].ptr=a;
                args[j].value=0.9f;
                args[j].it_start=start;
                args[j].it_end=end;
                args[j].niterations=niterations;
                args[j].nthreads=num_workers;
             
                acoilt_ult_creation_to(task_creator, (void *) &args[j],&ults[j],j%num_workers);
 
	}
        acoilt_yield();
        gettimeofday(&t_start2, NULL);
        for (int j = 0; j < num_workers; j++) {
            acoilt_ult_join(&ults[j]);
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
            num_workers, niterations, avg, min, max, dev,avgj/TIMES);
    
    for (int i = 0; i < total; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

    //printf("greeter returned %lu\n", (unsigned long) return_value);

    return EXIT_SUCCESS;
}



