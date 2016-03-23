#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <alloca.h>
//#include <omp.h>
#ifdef ARGOBOTS
   #include <abt.h>
#define GET_NUM_THREADS abt_num_threads()
#define GET_THREAD_NUM abt_get_thread_num()

int abt_num_threads(){
    int num;
    ABT_xstream_get_num(&num);
    return num;
}

int abt_get_thread_num(){
    int num;
    ABT_xstream_self_rank(&num);
    return num;
}


typedef struct abt_team{
	ABT_pool * pools;
	ABT_xstream * xstreams;
} abt_team_t;


abt_team_t * team;

#elif defined(MASSIVETHREADS)
#include <myth.h>
#include <sys/time.h>

#define GET_NUM_THREADS  num_workers
#define GET_THREAD_NUM   myth_get_worker_num()

int num_workers;

#else /* QTHREADS*/
#include<qthread/qthread.h>
#include <qthread/qtimer.h>

#define GET_NUM_THREADS  qthread_num_shepherds ()
#define GET_THREAD_NUM   qthread_shep()


#endif

/* Checking information */

static int solutions[] = {
        1,
        0,
        0,
        2,
        10, /* 5 */
        4,
        40,
        92,
        352,
        724, /* 10 */
        2680,
        14200,
        73712,
        365596,
};
#define MAX_SOLUTIONS sizeof(solutions)/sizeof(int)


int total_count;


/*
 * <a> contains array of <n> queen positions.  Returns 1
 * if none of the queens conflict, and returns 0 otherwise.
 */
int ok(int n, char *a)
{
     int i, j;
     char p, q;

     for (i = 0; i < n; i++) {
	  p = a[i];

	  for (j = i + 1; j < n; j++) {
	       q = a[j];
	       if (q == p || q == p - (j - i) || q == p + (j - i)){
		    	//printf("OK=false\n");
		    return 0;
		}
	  }
     }
	//printf("OK=true\n");
     return 1;
}


typedef struct glt_args {
	int n;
	int j;
	int i;
	char *a;
	int * sol;
	int depth;
} glt_args_t;

int global_dispatch=0;

void no_func()
{
	//printf("No func\n");
	return NULL;
}
void nqueens(/*int n, int j, char *a, int *solutions, int depth*/void * arguments);
void pre_nqueens(void * arguments)
{
	glt_args_t * in_args = (glt_args_t *)arguments;
	int n = in_args->n;
	int j = in_args->j;
	int i = in_args->i;
	char * a = in_args->a;
	int * solutions = in_args->sol;
        int depth = in_args->depth;

	char * b = alloca(n * sizeof(char));
	memcpy(b, a, j * sizeof(char));
	b[j] = (char) i;
	if (ok(j + 1, b))
	{
		glt_args_t out_args;
		out_args.n=n;
		out_args.j=j+1;
		out_args.i=i;
		out_args.a=b;
		out_args.sol=solutions;
		out_args.depth=depth;
       		nqueens(/*n, j + 1, b,solutions,depth*/(void *)&out_args); //FIXME: depth or depth+1 ???
	}


}

void nqueens(/*int n, int j, char *a, int *solutions, int depth*/void * arguments)
{
	int me = GET_THREAD_NUM;

	glt_args_t * in_args = (glt_args_t *)arguments;
	int n = in_args->n;
	int j = in_args->j;
	char * a = in_args->a;
	int * solutions = in_args->sol;
        int depth = in_args->depth;
	int *csols;
	int i;
		//printf("%d inicio nqueens con j=%d\n",me,j);

	if (n == j) {
		/* good solution, count it */

		//*solutions = 1;
		*in_args->sol=1;
		//printf("Good solution!\n");
		return;
	}


		//printf("%d despues de sol\n",me);
	*solutions = 0;
	csols = alloca(n*sizeof(int));
	memset(csols,0,n*sizeof(int));
	
	glt_args_t * out_args = (glt_args_t *) malloc(sizeof(glt_args_t)*n);

#ifdef ARGOBOTS
#ifdef TASKLET
	ABT_task * task_id = (ABT_task *)malloc(sizeof(ABT_task)*n);
#else
	ABT_thread * ult_id = (ABT_thread *) malloc(sizeof(ABT_thread)*n);
#endif

#elif defined(MASSIVETHREADS)
	myth_thread_t * ult_id = (myth_thread_t *)malloc(sizeof(myth_thread_t)*n);
#else
	aligned_t * ult_id = (aligned_t *)malloc(sizeof(aligned_t)*n);


#endif

     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {

			int dest, id;
			id=GET_THREAD_NUM;
			out_args[i].n=n;
			out_args[i].j=j;
			out_args[i].i=i;
			out_args[i].a=a;
			out_args[i].sol=&csols[i];
			out_args[i].depth=depth;
	  		
				if(global_dispatch>=GET_NUM_THREADS){
					dest = id;
				}
				else
					dest = (id !=0)? id : global_dispatch++;
				//dest = (dest == 0)? global_dispatch++%glt_get_num_threads():dest;
				//printf("Soy %d/%d y voy a crear a %d\n",id,glt_get_num_threads(),dest); 
       				//nqueens(n, j + 1, b,&csols[i],depth); //FIXME: depth or depth+1 ???
#ifdef ARGOBOTS
#ifdef TASKLET
			        //glt_tasklet_creation_to(pre_nqueens,(void *)&out_args[i],&task_id[i],dest);
			        ABT_task_create_on_xstream(team->xstreams[dest],pre_nqueens,(void *)&out_args[i],&task_id[i]);
				//glt_tasklet_creation_to(no_func,NULL,&task_id[i],dest);
#else
			        //ABT_thread_create_on_xstream(team->xstreams[dest],pre_nqueens,(void *)&out_args[i],ABT_THREAD_ATTR_NULL,&ult_id[i]);
			        ABT_thread_create(team->pools[dest],pre_nqueens,(void *)&out_args[i],ABT_THREAD_ATTR_NULL,&ult_id[i]);
			        //glt_ult_creation_to(pre_nqueens,(void *)&out_args[i],&ult_id[i],dest);
				//glt_ult_creation_to(no_func,NULL,&ult_id[i],dest);
#endif

#elif defined(MASSIVETHREADS)
			        ult_id[i] = myth_create((void *)pre_nqueens,(void *)&out_args[i]);

#else
				qthread_fork_to((void *)pre_nqueens,(void *)&out_args[i],&ult_id[i],dest);
#endif
			

	}
		//printf("%d despues del for\n",me);
#ifdef ARGOBOTS	
ABT_thread_yield();

#elif defined (MASSIVETHREADS)
myth_yield(0);


#else
qthread_yield();
#endif
		//printf("%d despues del yield\n",me);

for(i = 0;i<n;i++)
	{
			//printf("%d para join %d\n",me,i);
#ifdef ARGOBOTS
#ifdef TASKLET
		//glt_tasklet_join(&task_id[i]);
		ABT_task_free(&task_id[i]);
#else
		//glt_ult_join(&ult_id[i]);
		ABT_thread_free(&ult_id[i]);
#endif

#elif defined(MASSIVETHREADS)
	myth_join(ult_id[i],NULL);

#else
	qthread_readFF(NULL,&ult_id[i]);
#endif
	}
		//printf("%d despues del join\n",me);

	//#pragma omp taskwait
	for ( i = 0; i < n; i++) 
	{

		*solutions += csols[i];
	}

	
}


int verify_queens (int size)
{
	if ( size > MAX_SOLUTIONS ) return 1;
	if ( total_count == solutions[size-1]) return 0;
	return -1;
}
char *a;
//void find_queens (int size)
int main(int argc, char * argv [])
{
	total_count=0;
        int size = atoi(argv[1]);
#ifdef ARGOBOTS
    int num_threads =1;
    int num_pools = num_threads;
    team = (abt_team_t *)malloc(sizeof(abt_team_t));
    ABT_init(argc, argv);

    if (getenv("GLT_NUM_THREADS") != NULL) {
        num_threads = atoi(getenv("GLT_NUM_THREADS"));
    }

    if (getenv("GLT_NUM_POOLS") != NULL) {
        num_pools = atoi(getenv("GLT_NUM_POOLS"));
    }

    team->xstreams = (ABT_xstream *) malloc(sizeof (ABT_xstream) * num_threads);
    team->pools = (ABT_pool *) malloc(sizeof (ABT_pool) * num_pools);


    for (int i = 0; i < num_pools; i++) {
        ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
            &team->pools[i]);
    }
    ABT_xstream_self(&team->xstreams[0]);
    ABT_xstream_set_main_sched_basic(team->xstreams[0], ABT_SCHED_DEFAULT,
            1, &team->pools[0]);

    for (int i = 1; i < num_threads; i++) {
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1,
                &team->pools[i % num_pools],
                ABT_SCHED_CONFIG_NULL, &team->xstreams[i]);
        ABT_xstream_start(team->xstreams[i]);
    }	
#elif defined(MASSIVETHREADS)

    myth_init();

    num_workers = atoi(getenv("MYTH_WORKER_NUM"));

    struct timeval t_start,t_end;
#else
	qthread_initialize();
	qtimer_t timer = qtimer_create();
#endif
//glt_init(argc,argv);

	//#pragma omp parallel
	//{
	//	#pragma omp master
        //	{
			//printf(" %d threads ",GET_NUM_THREADS);
	//	}
	//}
	#ifdef ARGOBOTS
	double t_start = ABT_get_wtime();
	
	#elif defined(MASSIVETHREADS)
	gettimeofday(&t_start, NULL);

	#else
	qtimer_start(timer);

	#endif

	
	glt_args_t init_arg;
	
#ifdef ARGOBOTS
	ABT_thread *ult_id = (ABT_thread *)malloc(sizeof(ABT_thread));
#elif defined(MASSIVETHREADS)
	myth_thread_t *ult_id = (myth_thread_t *)malloc(sizeof(myth_thread_t));
#else
	aligned_t * ult_id = (aligned_t *)malloc(sizeof(aligned_t));
#endif	
	//#pragma omp parallel
	//{
	//
	//	#pragma omp single
	//	{
	//
	a = alloca(size * sizeof(char));
	init_arg.n=size;
	init_arg.j=0;
	init_arg.a=a;
	init_arg.sol=&total_count;
	init_arg.depth=0;
	
#ifdef ARGOBOTS
	ABT_thread_create(team->pools[0],nqueens,(void *)&init_arg,ABT_THREAD_ATTR_NULL,&ult_id[0]);	
	ABT_thread_yield();
	ABT_thread_free(&ult_id[0]);
#elif defined(MASSIVETHREADS)
	ult_id[0]=myth_create((void *)nqueens,(void *)&init_arg);
	myth_yield(0);
	myth_join(ult_id[0],NULL);
#else
	qthread_fork_to((void *)nqueens,(void *)&init_arg,&ult_id[0],0);
	qthread_yield();
	qthread_readFF(NULL, &ult_id[0]);

#endif

#ifdef ARGOBOTS	
	double t_end=ABT_get_wtime();
	printf("%f\n",t_end-t_start);
	ABT_finalize();
#elif defined(MASSIVETHREADS)
	gettimeofday(&t_end, NULL);
	double time = ((t_end.tv_sec * 1000000 + t_end.tv_usec)
		  - (t_start.tv_sec * 1000000 + t_start.tv_usec))/1000000.0;
	printf("%f\n",time);
	myth_fini();
#else
	qtimer_stop(timer);
	printf("%f\n",qtimer_secs(timer));
	qthread_finalize();
#endif
}

