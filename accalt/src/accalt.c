#include <accalt.h>

accalt_team_t * main_team;

void accalt_start() {
#ifdef ARGOBOTS
    //printf("Starting with ARGOBOTS\n");
#endif
#ifdef MASSIVETHREADS
    //printf("Starting with MASSIVETHREADS\n");
#endif
#ifdef QTHREADS
    //printf("Starting with QTHREADS\n");
#endif
}

void accalt_end() {
#ifdef ARGOBOTS
    //printf("Ending with ARGOBOTS\n");
#endif
#ifdef MASSIVETHREADS
    //printf("Ending with MASSIVETHREADS\n");
#endif
#ifdef QTHREADS
    //printf("Ending with QTHREADS\n");
#endif
}

void accalt_init(int argc, char * argv[]) {

    int num_threads = 1;
    main_team = (accalt_team_t *) malloc(sizeof (accalt_team_t));

#ifdef ARGOBOTS


    ABT_init(argc, argv);
    int num_pools = 1;
    if (getenv("ACCALT_NUM_THREADS") != NULL) {
        num_threads = atoi(getenv("ACCALT_NUM_THREADS"));
    }
    if (getenv("ACCALT_NUM_POOLS") != NULL) {
        num_pools = atoi(getenv("ACCALT_NUM_POOLS"));
    }
    main_team->num_xstreams = num_threads;
    main_team->num_pools = num_pools;
    //printf("Argobots %d ES, %d Pools\n", num_threads, num_pools);
    ABT_xstream_self(&main_team->master);

    main_team->team = (ABT_xstream *) malloc(sizeof (ABT_xstream) * num_threads);
    main_team->pools = (ABT_pool *) malloc(sizeof (ABT_pool) * num_pools);

    for (int i = 0; i < num_pools; i++) {
        ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
                &main_team->pools[i]);
    }

    ABT_xstream_self(&main_team->team[0]);
    ABT_xstream_set_main_sched_basic(main_team->team[0], ABT_SCHED_DEFAULT,
            1, &main_team->pools[0]);
    for (int i = 1; i < num_threads; i++) {
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1,
                &main_team->pools[i % main_team->num_pools],
                ABT_SCHED_CONFIG_NULL, &main_team->team[i]);
        ABT_xstream_start(main_team->team[i]);
    }
#endif
#ifdef MASSIVETHREADS
    char buff[10];
    if (getenv("ACCALT_NUM_THREADS") != NULL) {
        num_threads = atoi(getenv("ACCALT_NUM_THREADS"));
        sprintf(buff, "%d", num_threads);
        setenv("MYTH_WORKER_NUM", buff, 1);
    } else
        num_threads = atoi(getenv("MYTH_WORKER_NUM"));

    setenv("MYTH_BIND_WORKERS", "1", 1);

    //printf("Massive %d Workers\n", num_threads);
    main_team->num_workers = num_threads;
    myth_init(); //MassiveThreads
#endif
#ifdef QTHREADS
    char buff[10];
    int num_workers_per_thread;
    if (getenv("ACCALT_NUM_THREADS") != NULL) {
        num_threads = atoi(getenv("ACCALT_NUM_THREADS"));
        sprintf(buff, "%d", num_threads);
        setenv("QTHREAD_NUM_SHEPHERDS", buff, 1);
    } else
        num_threads = atoi(getenv("QTHREAD_NUM_SHEPHERDS"));

    if (getenv("ACOILT_NUM_WORKERS_PER_THREAD") != NULL) {
        num_workers_per_thread = atoi(getenv("ACOILT_NUM_WORKERS_PER_THREAD"));
        sprintf(buff, "%d", num_workers_per_thread);
        setenv("QTHREAD_NUM_WORKERS_PER_SHEPHERD", buff, 1);
    } else
        num_workers_per_thread = atoi(getenv("QTHREAD_NUM_WORKERS_PER_SHEPHERD"));
    if (num_threads == 1 && num_workers_per_thread > 1) {
        setenv("QTHREAD_SHEPHERDS_BOUNDARY", "node", 1);
        setenv("QTHREAD_WORKER_UNIT", "core", 1);
    }
    if (num_threads > 1) {
        setenv("QTHREAD_SHEPHERDS_BOUNDARY", "core", 1);
        setenv("QTHREAD_WORKER_UNIT", "core", 1);
    }
    setenv("QTHREAD_AFFINITY", "yes", 1);

    //printf("Qthreads %d Shepherds, %d Workers_per_shepherd\n", num_threads, num_workers_per_thread);

    main_team->num_shepherds = num_threads;
    main_team->num_workers_per_shepherd = num_workers_per_thread;
    qthread_initialize(); //qthreads
#endif
}

void accalt_finalize() {

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

ACCALT_ult * accalt_ult_malloc(int number_of_ult) {
    ACCALT_ult * ults = (ACCALT_ult *) malloc(sizeof (ACCALT_ult) * number_of_ult);
    return ults;
}

ACCALT_tasklet * accalt_tasklet_malloc(int number_of_tasklets) {
    ACCALT_tasklet * tasklets = (ACCALT_tasklet *) malloc(sizeof (ACCALT_tasklet) * number_of_tasklets);
    return tasklets;
}

void accalt_ult_creation(void(*thread_func)(void *), void *arg, ACCALT_ult *new_ult) {
#ifdef ARGOBOTS
    ABT_xstream xstream;
    ABT_xstream_self(&xstream);
    ABT_pool pool;
    ABT_xstream_get_main_pools(xstream, 1, &pool);
    ABT_thread_create(pool, thread_func, arg, ABT_THREAD_ATTR_NULL, new_ult);
#endif
#ifdef MASSIVETHREADS
    *new_ult = myth_create((void *) thread_func, arg);
#endif
#ifdef QTHREADS
    qthread_fork((void *) thread_func, arg, new_ult);
#endif
}

void accalt_ult_creation_to(void(*thread_func)(void *), void *arg, ACCALT_ult *new_ult, int dest) {
#ifdef ARGOBOTS
    ABT_pool pool;
    ABT_xstream_get_main_pools(main_team->team[dest], 1, &pool);
    ABT_thread_create(pool, thread_func, arg, ABT_THREAD_ATTR_NULL, new_ult);
#endif
#ifdef MASSIVETHREADS
    acoilt_ult_creation(thread_func, arg, new_ult);
#endif
#ifdef QTHREADS
    qthread_fork_to((void *) thread_func, arg, new_ult, dest);
#endif
}

void accalt_tasklet_creation(void(*thread_func)(void *), void *arg, ACCALT_tasklet *new_ult) {
#ifdef ARGOBOTS
    ABT_xstream xstream;
    ABT_xstream_self(&xstream);
    ABT_pool pool;
    ABT_xstream_get_main_pools(xstream, 1, &pool);
    ABT_task_create(pool, thread_func, arg, new_ult);
#endif
#ifdef MASSIVETHREADS
    *new_ult = myth_create((void *) thread_func, arg);
#endif
#ifdef QTHREADS
    qthread_fork((void *) thread_func, arg, new_ult);
#endif
}

void accalt_tasklet_creation_to(void(*thread_func)(void *), void *arg, ACCALT_tasklet *new_ult, int dest) {
#ifdef ARGOBOTS
    ABT_pool pool;
    ABT_xstream_get_main_pools(main_team->team[dest], 1, &pool);
    ABT_task_create(pool, thread_func, arg, new_ult);
#endif
#ifdef MASSIVETHREADS
    acoilt_ult_creation(thread_func, arg, new_ult);
#endif
#ifdef QTHREADS
    qthread_fork_to((void *) thread_func, arg, new_ult, dest);
#endif
}

void accalt_yield() {
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

void accalt_yield_to(ACCALT_ult ult) {
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

void accalt_ult_join(ACCALT_ult *ult) {
#ifdef ARGOBOTS
    ABT_thread_free(ult);
#endif
#ifdef MASSIVETHREADS
    myth_join(*ult, NULL);
#endif
#ifdef QTHREADS
    qthread_readFF(NULL, ult);
#endif
}

void accalt_tasklet_join(ACCALT_tasklet *tasklet) {
#ifdef ARGOBOTS
    ABT_task_free(tasklet);
#endif
#ifdef MASSIVETHREADS
    myth_join(*tasklet, NULL);
#endif
#ifdef QTHREADS
    qthread_readFF(NULL, tasklet);
#endif
}

int accalt_get_num_threads() {
#ifdef ARGOBOTS
    return main_team->num_xstreams;
#endif
#ifdef MASSIVETHREADS
    return main_team->num_workers;
#endif
#ifdef QTHREADS
    return main_team->num_shepherds;
#endif
}

int accalt_get_thread_num() {
#ifdef ARGOBOTS
    int rank;
    ABT_xstream_self_rank(&rank);
    return rank;
#endif
#ifdef MASSIVETHREADS
    return myth_get_worker_num();
#endif
#ifdef QTHREADS
    return qthread_shep();
#endif
}

/*
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
