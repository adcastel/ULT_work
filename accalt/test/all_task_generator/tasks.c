#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <accalt.h>
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
    int it_start;
    int it_end;
} task_creator_args_t;


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

    printf("#Thread: %d (CPU: %d) pos: %d\n", accalt_get_thread_num(),sched_getcpu(),pos);

#endif
    for(i=pos;i<posfin;i++){
        ptr[i] *= value;
    }
    return 0;
}

void task_creator(void *arguments){
    int i;
    int ntasks;
    int status;
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
    printf("#Thread: %d (CPU: %d) crear task y creare  %d tareas (%d-%d) de granularidad %d\n", accalt_get_thread_num(), sched_getcpu(), ntasks, it_end, it_start, gran);
#endif
    vector_scal_args_t * out_args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t) * ntasks);
  
#ifdef TASK
    ACCALT_tasklet * tasklets;
    tasklets = accalt_tasklet_malloc(ntasks);
#else 
    ACCALT_ult * ults;
    ults = accalt_ult_malloc(ntasks);
#endif
int ct=0; 
for(i=it_start;i<it_end;i++){
        out_args[ct].pos=i*gran;
        out_args[ct].value=value;
        out_args[ct].ptr=ptr;
        out_args[ct].gran=gran;
#ifdef TASK
	accalt_tasklet_creation(vector_scal,(void *)&out_args[ct],&tasklets[ct]);
#else
	accalt_ult_creation(vector_scal,(void *)&out_args[ct],&ults[ct]);
#endif
        ct++;
}

accalt_yield();

for (i = 0; i < ntasks; i++) {
#ifdef TASK
   accalt_tasklet_join(&tasklets[i]);
#else            
accalt_ult_join(&ults[i]);
#endif
}
return 0;
}




int main(int argc, char *argv[]) {
    int status;
    int ntasks;
    int j;
    int start, end;
    char *str, * endptr;
    double times[TIMES];
    double times_join[TIMES];
    int num_shepherds, num_workers;
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


    args = (task_creator_args_t *) malloc(sizeof (task_creator_args_t)
            * ntasks);

    int num_threads = accalt_get_num_threads();

    ACCALT_ult * ults;
    ults = accalt_ult_malloc(num_threads);    
    for (int t = 0; t < TIMES; t++) {
        for (int i = 0; i < total; i++) {
            a[i] = i * 1.0f;
        }
        gettimeofday(&t_start, NULL);
	
	 /* Work here */
        start=end=0;
        int bloc = ntasks / num_threads;
        int rest = ntasks % num_threads;

        for (j = 0; j < num_threads; j++) {
                start=end;
                end=start+bloc;
                if(j<rest){end++;}
                args[j].ptr=a;
                args[j].value=0.9f;
                args[j].it_start=start;
                args[j].it_end=end;
                args[j].gran=granularity;
	        accalt_ult_creation_to(task_creator, (void *) &args[j],&ults[j],j%num_threads);        
	}
        accalt_yield();
        gettimeofday(&t_start2, NULL);
        for (int j = 0; j < num_workers; j++) {
            accalt_ult_join(&ults[j]);
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



