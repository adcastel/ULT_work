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

typedef struct {
    float * ptr;
    float value;
    int pos;
    int gran;
    int nchildtask;
    int nlevels;
} task_creator_args_t;



void * vector_scal(void *arguments) {
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int pos = arg->pos;
    int gran = arg->gran;
    int posfin=pos+gran;
    float value = arg->value;
    float * ptr = arg->ptr;
    int i;

#ifdef VERBOSE

    printf("#Worker: %ld (CPU: %d), pos %d and value %f\n", myth_get_worker_num(), sched_getcpu(), pos, value);

#endif

   for(i=pos;i<posfin;i++){
        ptr[i] *= value;
    }

}

void * task_creator(void *arguments){

    task_creator_args_t *arg;
    arg = (task_creator_args_t *) arguments;
    int pos = arg->pos;
    int gran = arg->gran;
    float value = arg->value;
    float * ptr = arg->ptr;
    int nchild = arg->nchildtask;
    int nlvl = arg->nlevels;
    int i;
    vector_scal_args_t *out_arg = (vector_scal_args_t *)malloc(sizeof(vector_scal_args_t)*nchild);
    if(nlvl > 1){
        printf("Aun nada\n");
    }
    else{

#ifdef VERBOSE
        printf("#Worker: %ld (CPU: %d),  en crear task para posicion %d, nchildtask %d, nlevels %d y granularidad %d\n", myth_get_worker_num(), sched_getcpu(),  pos ,nchild,nlvl,gran);
#endif
        myth_thread_t * tasks;
        tasks = (myth_thread_t *)malloc(sizeof(myth_thread_t)*nchild);

	
        int t;
        int start = pos;
        int stride=(nchild*nlvl*gran)/nchild;
        for(t=0;t<nchild;t++){
#ifdef VERBOSE
            printf("#Worker %d: voy a crear con start=%d\n", myth_get_worker_num(), start);
#endif
            out_arg[t].ptr=ptr;
            out_arg[t].value=value;
            out_arg[t].pos=start;
            out_arg[t].gran=gran;

            tasks[t] = myth_create(vector_scal, (void *) &out_arg[t]);
#ifdef VERBOSE
            printf("#Worker %d: creada mi tarea %d\n", myth_get_worker_num(), t);
#endif
            start+=stride;
        }


        myth_yield(0);

        for (i = 0; i < nchild; i++) {
            myth_join(tasks[i],NULL);

        }
    }
}




int main(int argc, char *argv[]) {
    double times[TIMES];
    double times_join[TIMES];
    int num_workers;
    task_creator_args_t * args;
    struct timeval t_start, t_start2, t_end;
    float *a;

    int ntasks = argc > 1 ? atoi(argv[1]) : NUM_ELEMS;
    int nchildtask = argc > 2 ? atoi(argv[2]) : 2;
    int nlevels = argc > 3 ? atoi(argv[3]) : 1;
    int granularity = argc > 4 ? atoi(argv[4]) : 1;

    //int total=ntasks*granularity;
    int total=ntasks*nchildtask*nlevels*granularity;
    int stride=nchildtask*nlevels*granularity;
    
    a = malloc(sizeof (float)*total);
    for (int i = 0; i < total; i++) {
        a[i] = i * 1.0f;
    }

    myth_init();
    num_workers=atoi(getenv("MYTH_WORKER_NUM"));
    myth_thread_t * workers;
    workers = (myth_thread_t *)malloc(sizeof(myth_thread_t)*ntasks);

    args = (task_creator_args_t *) malloc(sizeof (task_creator_args_t)
            * ntasks);

    

    for (int t = 0; t < TIMES; t++) {
        for (int i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);

        /* Each task is created on the xstream which is going to execute it*/

        int ct=0;
        for (int j = 0; j < total; j=j+stride) {
            args[ct].pos = j;
            args[ct].gran = granularity;
            args[ct].value = 0.9f;
            args[ct].ptr = a;
            args[ct].nlevels = nlevels;
            args[ct].nchildtask = nchildtask;
	    workers[ct]=myth_create(task_creator, (void *) &args[ct]);
            ct++;
        }
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
            num_workers, total, avg, min, max, dev,avgj/TIMES);
    
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



