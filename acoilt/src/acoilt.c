#include <acoilt.h>

#ifdef ARGOBOTS
    acoilt_team_t * main_team;
#endif

void acoilt_start(){
#ifdef ARGOBOTS
    printf("Starting with ARGOBOTS\n");
#endif
#ifdef MASSIVETHREADS
        printf("Starting with MASSIVETHREADS\n");
#endif
#ifdef QTHREADS
        printf("Starting with QTHREADS\n");
#endif
}

void acoilt_end(){
#ifdef ARGOBOTS
    printf("Ending with ARGOBOTS\n");
#endif
#ifdef MASSIVETHREADS
        printf("Ending with MASSIVETHREADS\n");
#endif
#ifdef QTHREADS
        printf("Ending with QTHREADS\n");
#endif
}

void acoilt_init(int argc, char * argv[]){
        
    int num_threads=1;
#ifdef ARGOBOTS
    
    main_team = (acoilt_team_t *) malloc (sizeof(acoilt_team_t));
    
    ABT_init(argc,argv);
    int num_pools=1;
    if(getenv("ACOILT_NUM_THREADS")!=NULL){
        num_threads= atoi(getenv("ACOILT_NUM_THREADS"));
    }
    if(getenv("ACOILT_NUM_POOLS")!=NULL){
        num_pools= atoi(getenv("ACOILT_NUM_POOLS"));
    }
    main_team->num_xstreams = num_threads;
    main_team->num_pools = num_pools;
    printf("Argobots %d ES, %d Pools\n",num_threads,num_pools);
    ABT_xstream_self(&main_team->master);
    
    main_team->team=(ABT_xstream *) malloc(sizeof (ABT_xstream) * num_threads);
    main_team->pools=(ABT_pool *) malloc(sizeof (ABT_pool) * num_pools);
    
    for (int i = 0; i < num_pools; i++) {
        ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
            &main_team->pools[i]);
    }
    
    ABT_xstream_self(&main_team->team[0]);
    ABT_xstream_set_main_sched_basic(main_team->team[0], ABT_SCHED_DEFAULT,
        1, &main_team->pools[0]);
    
    for (int i = 1; i < num_threads; i++) {
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1, 
                &main_team->pools[i%main_team->num_pools],
                ABT_SCHED_CONFIG_NULL, &main_team->team[i]);
        ABT_xstream_start(main_team->team[i]);
    }
#endif
#ifdef MASSIVETHREADS
    char buff[10];
    if(getenv("ACOILT_NUM_THREADS")!=NULL){
        num_threads= atoi(getenv("ACOILT_NUM_THREADS"));
        sprintf(buff,"%d",num_threads);
        setenv("MYTH_WORKER_NUM",buff,1);
    }
    else
        num_threads= atoi(getenv("MYTH_WORKER_NUM"));
    
    printf("Massive %d Workers\n",num_threads);
    myth_init(); //MassiveThreads
#endif
#ifdef QTHREADS
    char buff[10];
    int num_workers_per_thread;
    if(getenv("ACOILT_NUM_THREADS")!=NULL){
        num_threads= atoi(getenv("ACOILT_NUM_THREADS"));
        sprintf(buff,"%d",num_threads);
        setenv("QTHREADS_NUM_SHEPHERDS",buff,1);
    }
    else
        num_threads= atoi(getenv("QTHREADS_NUM_SHEPHERDS"));
    
    if(getenv("ACOILT_NUM_WORKERS_PER_THREAD")!=NULL){
        num_workers_per_thread = atoi(getenv("ACOILT_NUM_WORKERS_PER_THREAD"));
        sprintf(buff,"%d",num_workers_per_thread);
        setenv("QTHREADS_NUM_WORKERS_PER_SHEPHERDS",buff,1);
    }
    else
        num_workers_per_thread = atoi(getenv("QTHREADS_NUM_WORKERS_PER_SHEPHERDS"));
    
    printf("Qthreads %d Shepherds, %d Workers_per_shepherd\n",num_threads,num_workers_per_thread);
    qthread_initialize();  //qthreads
#endif
}




void acoilt_finalize(){

#ifdef ARGOBOTS
    
    for (int i = 1; i < main_team->num_xstreams; i++) {
        ABT_xstream_join(main_team->team[i]);
        ABT_xstream_free(&main_team->team[i]);
    }
    ABT_finalize();
        
#endif
#ifdef MASSIVETHREADS
    myth_fini(); //MassiveThreads
#endif    
}

ACOILT_ult * acoilt_ult_malloc(int number_of_ult){
    ACOILT_ult * ults = (ACOILT_ult *) malloc(sizeof(ACOILT_ult) * number_of_ult);
    return ults;
}

ACOILT_tasklet * acoilt_tasklet_malloc(int number_of_tasklets){
    ACOILT_tasklet * tasklets = (ACOILT_tasklet *) malloc(sizeof(ACOILT_tasklet) * number_of_tasklets);
    return tasklets;
}

void acoilt_ult_creation(void(*thread_func)(void *), void *arg, ACOILT_ult *new_ult){
#ifdef ARGOBOTS
    ABT_xstream xstream;
    ABT_xstream_self(&xstream);
    ABT_pool pool;
    ABT_xstream_get_main_pools ( xstream, 1, &pool);
    ABT_thread_create(pool, thread_func, arg, ABT_THREAD_ATTR_NULL ,new_ult);
#endif
#ifdef MASSIVETHREADS
    *new_ult = myth_create((void *)thread_func, arg);
#endif
#ifdef QTHREADS
    qthread_fork((void *)thread_func, arg, new_ult);
#endif
}

void acoilt_ult_creation_to(void(*thread_func)(void *), void *arg, ACOILT_ult *new_ult, int dest){
#ifdef ARGOBOTS
    ABT_pool pool;
    ABT_xstream_get_main_pools ( main_team->team[dest], 1, &pool);
    ABT_thread_create(pool, thread_func, arg, ABT_THREAD_ATTR_NULL ,new_ult);
#endif
#ifdef MASSIVETHREADS
    acoilt_ult_creation(thread_func,arg,new_ult);
#endif
#ifdef QTHREADS
    qthread_fork_to((void *)thread_func, arg, new_ult,dest);
#endif
}

void acoilt_tasklet_creation(void(*thread_func)(void *), void *arg, ACOILT_tasklet *new_ult){
#ifdef ARGOBOTS
    ABT_xstream xstream;
    ABT_xstream_self(&xstream);
    ABT_pool pool;
    ABT_xstream_get_main_pools ( xstream, 1, &pool);
    ABT_task_create(pool, thread_func, arg ,new_ult);
#endif
#ifdef MASSIVETHREADS
    *new_ult = myth_create((void *)thread_func, arg);
#endif
#ifdef QTHREADS
    qthread_fork((void *)thread_func, arg, new_ult);
#endif
}

void acoilt_tasklet_creation_to(void(*thread_func)(void *), void *arg, ACOILT_tasklet *new_ult, int dest){
#ifdef ARGOBOTS
    ABT_pool pool;
    ABT_xstream_get_main_pools ( main_team->team[dest], 1, &pool);
    ABT_task_create(pool, thread_func, arg ,new_ult);
#endif
#ifdef MASSIVETHREADS
    acoilt_ult_creation(thread_func,arg,new_ult);
#endif
#ifdef QTHREADS
    qthread_fork_to((void *)thread_func, arg, new_ult,dest);
#endif
}

void acoilt_yield(){
#ifdef ARGOBOTS
    ABT_thread_yield();
#endif
#ifdef MASSIVETHREADS
   myth_yield(0);
#endif
#ifdef QTHREADS
    qthread_yield();
#endif
}

void acoilt_yield_to(ACOILT_ult ult){
#ifdef ARGOBOTS
    ABT_thread_yield_to(ult);
#endif
#ifdef MASSIVETHREADS
   myth_yield(0);
#endif
#ifdef QTHREADS
    qthread_yield();
#endif
}

/*

ULT join. This function joins a given ULT

acoilt_ult_join()
ABT_thread_free() // Argobots
myth_join() //MassiveThreads
qthread_readFF() //Qthreads
Not supported in ConverseThreads. CmiNodeBarrier()?
Channel //GO

Tasklet join.  This function joins a given Tasklet

acoilt_tasklet_join()
call to acoilt_ult_join if tasklets are not supported
ABT_task_free() // Argobots
Not supported in ConverseThreads. CmiNodeBarrier()?


Structures

ULT
ABT_thread //Argobots
myth_thread_t //MassiveThreads
aligned_t //Qthreads
CthThread //ConverseThreads
goroutine //GO
	
Tasklet
ABT_task //Argobots
Message //ConverseThreads

*/
