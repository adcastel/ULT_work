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


/* structure to pass arguments to expand tasks */

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


static aligned_t vector_scal(void *arguments) {
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int pos = arg->pos;
    int gran = arg->gran;
    int i;
    int posfin=pos+gran;
    float value = arg->value;
    float * ptr = arg->ptr;

#ifdef VERBOSE

    printf("#Shepherd: %d (CPU: %d) Worker %d, pos: %d\n", qthread_shep(),sched_getcpu(),qthread_worker(NULL), pos);

#endif
    for(i=pos;i<posfin;i++){
        ptr[i] *= value;
    }
    return 0;
}

static aligned_t task_creator(void *arguments){
    int i;
    int status;
    task_creator_args_t *in_arg;
    in_arg = (task_creator_args_t *) arguments;
    int pos = in_arg->pos;
    int gran = in_arg->gran;
    float value = in_arg->value;
    float * ptr = in_arg->ptr;
    int nchild = in_arg->nchildtask;
    int nlvl = in_arg->nlevels;
#ifdef VERBOSE
        printf("#Shepherd: %d (CPU: %d) en crear task para posicion %d, nchildtask %d, nlevels %d y granularidad %d\n", qthread_shep(), sched_getcpu(),  pos ,nchild,nlvl,gran);
#endif

    if(nlvl > 1){
        printf("Aun nada\n");
    }
    else{
        vector_scal_args_t * out_arg = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t) * nchild);
        aligned_t *returned_values;
        returned_values = (aligned_t *) malloc(sizeof (aligned_t) * nchild);
	int t;
        int start = pos;
        int stride=(nchild*nlvl*gran)/nchild;
        for(t=0;t<nchild;t++){
            out_arg[t].ptr=ptr;
            out_arg[t].value=value;
            out_arg[t].pos=start;
            out_arg[t].gran=gran;
            status = qthread_fork_to(vector_scal, (void *) &out_arg[t], &returned_values[t],qthread_shep());

#ifdef VERBOSE
            printf("#Shepherd %d: creada la tarea %d\n", qthread_shep(), t);
#endif
           start+=stride;
        }

        qthread_yield();

        for (i = 0; i < nchild; i++) {
            int ret = qthread_readFF(NULL, &returned_values[i]);
        }
    }
    
    return 0;
}




int main(int argc, char *argv[]) {
    int status;
    int j;
    char *str, * endptr;
    double times[TIMES];
    double times_join[TIMES];
    int num_shepherds, num_workers;
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


    args = (task_creator_args_t *) malloc(sizeof (task_creator_args_t)
            * ntasks);

    status = qthread_initialize();
    assert(status == QTHREAD_SUCCESS);
    num_shepherds = qthread_num_shepherds();
    num_workers = qthread_num_workers();
    aligned_t *returned_values;
    returned_values = (aligned_t *) malloc(sizeof (aligned_t) * ntasks);

    for (int t = 0; t < TIMES; t++) {
        for (int i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);
	
	 /* Work here */
	int ct=0;
        for (j = 0; j < total; j=j+stride) {
	    args[ct].pos = j;
            args[ct].gran = granularity;
            args[ct].value = 0.9f;
            args[ct].ptr = a;
            args[ct].nlevels = nlevels;
            args[ct].nchildtask = nchildtask;
 
#ifdef FORKTO
            status = qthread_fork_to(task_creator, (void *) &args[ct], &returned_values[ct],ct%num_shepherds);
#else
            status = qthread_fork(task_creator, (void *) &args[ct], &returned_values[ct]);
#endif
	    ct++;
        }
        qthread_yield();
        gettimeofday(&t_start2, NULL);
        for (int j = 0; j < ntasks; j++) {
            int ret = qthread_readFF(NULL, &returned_values[j]);
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
    printf("%d %d %d %f [%f - %f] %f Join(%f)\n",
            num_shepherds,num_workers, total, avg, min, max, dev,avgj/TIMES);
    
    for (int i = 0; i < total; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

    //printf("greeter returned %lu\n", (unsigned long) return_value);

    return EXIT_SUCCESS;
}



