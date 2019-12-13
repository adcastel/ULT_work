#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
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

    printf("#Worker: %ld (CPU: %d), pos_ini %d, pos_fin\n", myth_get_worker_num(), sched_getcpu(), inner_start_pos, inner_end_pos);

#endif

    for(i=inner_start_pos;i<inner_end_pos;i++){
        ptr[outer_pos*niterations+i] *= value;
    }

}

void * task_creator(void *arguments){

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
    printf("#Worker: %ld (CPU: %d),  en task_creator y creare %d tareas para el rango de iteraciones de %d a %d del loop externo\n", myth_get_worker_num(), sched_getcpu(), nthreads, it_start, it_end);
#endif
    pthread_t * tasks;
    tasks = (pthread_t *)malloc(sizeof(pthread_t)*nthreads);    
    innerloop_args_t * out_args = (innerloop_args_t *) malloc(sizeof (innerloop_args_t) * nthreads);

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

            /*tasks[ct] = */pthread_create(&tasks[ct],NULL,vector_scal, (void *) &out_args[ct]);

#ifdef VERBOSE
            printf("#Worker %d: creada mi tarea %d\n", myth_get_worker_num(), ct);
#endif

        }

        pthread_yield();

        for (j = 0; j < nthreads; j++) {
           pthread_join(tasks[j],NULL);
        }
    }
}




int main(int argc, char *argv[]) {
    int niterations;
    char *str, * endptr;
    double times[TIMES];
    double times_join[TIMES];
    int num_workers;
    outerloop_args_t * args;
    struct timeval t_start, t_start2, t_end;
    float *a;

   /* if (argc > 1) {
        str = argv[1];
    }*/
    num_workers = argc > 1 ?  atoi(argv[1]): 4;
    niterations = argc > 2 ? atoi(argv[2]) : NUM_ELEMS;
    
    //int granularity = argc > 2 ? atoi(argv[2]) : 1;
    //int total=ntasks*granularity;
    int total=niterations*niterations;

    a = malloc(sizeof (float)*total);
    for (int i = 0; i < total; i++) {
        a[i] = i * 1.0f;
    }

    //myth_init();
    //num_workers=atoi(getenv("MYTH_WORKER_NUM"));
    pthread_t * workers;
    workers = (pthread_t *)malloc(sizeof(pthread_t)*num_workers);

    args = (outerloop_args_t *) malloc(sizeof (outerloop_args_t)
            * num_workers);

    

        gettimeofday(&t_start, NULL);
    for (int t = 0; t < TIMES; t++) {
        /*for (int i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
*/
        /* Each task is created on the xstream which is going to execute it*/
        int bloc = niterations / (num_workers);
        int rest = niterations % (num_workers);
        int start = 0;
        int end = 0;
        for (int j = 0; j < num_workers; j++) {
            start = end;
            int inc = (j < rest) ? 1 : 0;
            end += bloc + inc;
	    args[j].ptr=a;
            args[j].value=0.9f;
            args[j].it_start=start;
            args[j].it_end=end;
            args[j].niterations=niterations;
            args[j].nthreads=num_workers;

	    /*workers[j]=*/pthread_create(&workers[j], NULL, task_creator, (void *) &args[j]);

        }
	pthread_yield();
        for (int j = 0; j < num_workers; j++) {
           pthread_join(workers[j],NULL); 
        }
        
        /*times[t] = (time / 1000000.0);
	double time_join = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
            (t_start2.tv_sec * 1000000 + t_start2.tv_usec);
        times_join[t] = (time_join / 1000000.0);
*/
    }
        gettimeofday(&t_end, NULL);

        double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                (t_start.tv_sec * 1000000 + t_start.tv_usec);
    /*double min, max, avg, aux, sigma, dev;
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
#endif*/
    printf("%d %d %f\n",
            num_workers, niterations, (time/1000000.0)/TIMES);
    
    /*for (int i = 0; i < total; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }*/

    //printf("greeter returned %lu\n", (unsigned long) return_value);
//myth_fini();

    return EXIT_SUCCESS;
}



