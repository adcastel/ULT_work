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

/*
ULT allocation. This function allocates memory for a given number of ULTs

acoilt_ult_malloc()

Tasklet allocation.  This function allocates memory for a given number of Tasklets

acoilt_tasklet_malloc()
call to acoilt_ult_malloc if tasklets are not supported

ULT creation. This function creates a ULT into the current thread queue

acoilt_ult_creation()
ABT_thread_create(self_pool) //Argobots
myth_create() //MassiveThreads
qthread_fork() //Qthreads
CthCreate() + CthAwaken() // Converse
go + pointer // GO 




ULT creation to. This function creates a ULT into a given thread queue

acoilt_ult_creation_to()
ABT_thread_create(pool) //Argobots
acoilt_ult_creation() //MassiveThreads
qthread_fork_to() // Qthreads
acoilt_ult_creation() //ConverseThreads
acoilt_ult_creation() //GO

Tasklet creation. This function creates a Tasklet into the current thread queue

acoilt_tasklet_creation()
call to acoilt_ult_creation() if tasklets are not supported
ABT_task_create(self_pool) //Argobots
CmiSetHandler() + CmiSyncSend() //ConverseThreads

Tasklet creation to. This function creates a Tasklet into a given thread queue

acoilt_tasklet_creation_to()
call to acoilt_ult_creation_to() if tasklets are not supported
ABT_thread_create(pool) //Argobots
CmiSetHandler() + CmiSyncSend() //ConverseThreads

Yield function. This function gives the control to the scheduler and the current thread is queued

acoilt_yield()
ABT_thread_yield() // Argobots
myth_yield() //MassiveThreads
qthread_yield() //Qthreads
CthYield() //ConverseThreads
GO // No yield function

Yield to function. This function gives the control to a given thread and the current thread is queued

acoilt_yield_to()
ABT_thread_yield_to() // Argobots
acoilt_yield() //MassiveThreads
acoilt_yield() //Qthreads
CthYieldPrio() //ConverseThreads
GO // No yield_to function

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
