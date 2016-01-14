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
    int it_start;
    int it_end;
} task_creator_args_t;


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
    int posfin=pos+gran;
    float value = arg->value;
    float * ptr = arg->ptr;

#ifdef VERBOSE

    printf("#Worker: %ld (CPU: %d), pos %d\n", myth_get_worker_num(), sched_getcpu(), pos);

#endif

   for(i=pos;i<posfin;i++){
        ptr[i] *= value;
    }

}

void * task_creator(void *arguments){
    int i;
    int ntasks;
    int it_start, it_end, gran;
    task_creator_args_t *in_arg;
    in_arg = (task_creator_args_t *) arguments;
    it_start=in_arg->it_start;
    it_end=in_arg->it_end;
    gran=in_arg->gran;
    ntasks=it_end-it_start;
    float value = in_arg->value;
    float * ptr = in_arg->ptr;

#ifdef VERBOSE
    printf("#Worker: %ld (CPU: %d),  crear task y creare  %d tareas (%d-%d) de granularidad %d\n", myth_get_worker_num(), sched_getcpu(), ntasks, it_end, it_start, gran);
#endif
    myth_thread_t * tasks;
    tasks = (myth_thread_t *)malloc(sizeof(myth_thread_t)*ntasks);

 vector_scal_args_t * out_args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t) * ntasks);
int ct=0;
for(i=it_start;i<it_end;i++){
        out_args[ct].pos=i*gran;
        out_args[ct].value=value;
        out_args[ct].ptr=ptr;
        out_args[ct].gran=gran;
	tasks[ct] = myth_create(vector_scal, (void *) &out_args[ct]);

#ifdef VERBOSE
printf("#Worker %d: creada mi tarea %d\n", myth_get_worker_num(), ct);
#endif
ct++;
}

        myth_yield(0);

for (i = 0; i < ntasks; i++) {
           myth_join(tasks[i],NULL);

}
}




int main(int argc, char *argv[]) {
    int ntasks;
    char *str, * endptr;
    double times[TIMES];
    double times_join[TIMES];
    int num_workers;
    task_creator_args_t * args;
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

    myth_init();
    num_workers=atoi(getenv("MYTH_WORKER_NUM"));
    myth_thread_t * workers;
    workers = (myth_thread_t *)malloc(sizeof(myth_thread_t)*num_workers);

    args = (task_creator_args_t *) malloc(sizeof (task_creator_args_t)
            * num_workers);

    

    for (int t = 0; t < TIMES; t++) {
        for (int i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);

        /* Each task is created on the xstream which is going to execute it*/
        int bloc = ntasks / (num_workers);
        int rest = ntasks % (num_workers);
        int start = 0;
        int end = 0;
        for (int j = 0; j < num_workers; j++) {
            start = end;
            int inc = (j < rest) ? 1 : 0;
            end += bloc + inc;
            args[j].value = 0.9f;
            args[j].ptr = a;
	    args[j].it_start=start;
            args[j].it_end=end;
            args[j].gran=granularity;

	    workers[j]=myth_create(task_creator, (void *) &args[j]);

        }
	myth_yield(0);
        gettimeofday(&t_start2, NULL);
        for (int j = 0; j < num_workers; j++) {
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
            num_workers, ntasks, avg, min, max, dev,avgj/TIMES);
    
    for (int i = 0; i < ntasks; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

    //printf("greeter returned %lu\n", (unsigned long) return_value);
myth_fini();

    return EXIT_SUCCESS;
}



