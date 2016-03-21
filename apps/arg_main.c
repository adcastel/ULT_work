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
		    return 0;
		}
	  }
     }
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
       		nqueens((void *)&out_args); 
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

	if (n == j) {
		/* good solution, count it */

		*in_args->sol=1;
		return;
	}


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
#ifdef ARGOBOTS
#ifdef TASKLET
		        ABT_task_create_on_xstream(team->xstreams[dest],pre_nqueens,(void *)&out_args[i],&task_id[i]);
#else
		        ABT_thread_create_on_xstream(team->xstreams[dest],pre_nqueens,(void *)&out_args[i],ABT_THREAD_ATTR_NULL,&ult_id[i]);
#endif
#endif
	}
#ifdef ARGOBOTS	
	ABT_thread_yield();
#endif

	for(i = 0;i<n;i++)
	{
#ifdef ARGOBOTS
#ifdef TASKLET
		ABT_task_free(&task_id[i]);
#else
		ABT_thread_free(&ult_id[i]);
#endif
#endif
	}

	for ( i = 0; i < n; i++) 
	{
		*solutions += csols[i];
	}
}



int main(int argc, char * argv [])
{
	total_count=0;
        int size = atoi(argv[1]);
#ifdef ARGOBOTS
    	int num_threads =1;
    	int num_pools = num_threads;
    	team = (abt_team_t *)malloc(sizeof(abt_team_t));
    
	ABT_init(argc, argv);

    	if (getenv("ABT_NUM_THREADS") != NULL) {
        	num_threads = atoi(getenv("ABT_NUM_THREADS"));
    	}

    	if (getenv("ABT_NUM_POOLS") != NULL) {
        	num_pools = atoi(getenv("ABT_NUM_POOLS"));
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
#endif

	printf("Computing N-Queens algorithm (n=%d) ", size);
	printf(" using %d threads ",GET_NUM_THREADS);
	
#ifdef ARGOBOTS
	double t_start = ABT_get_wtime();
#endif

	glt_args_t init_arg;
	
	ABT_thread *ult_id = (ABT_thread *)malloc(sizeof(ABT_thread));
	
	char *a;
	a = alloca(size * sizeof(char));
	init_arg.n=size;
	init_arg.j=0;
	init_arg.a=a;
	init_arg.sol=&total_count;
	init_arg.depth=0;
	
	ABT_thread_create(team->pools[0],nqueens,(void *)&init_arg,ABT_THREAD_ATTR_NULL,&ult_id[0]);	

	ABT_thread_yield();
	ABT_thread_free(&ult_id[0]);
	
	double t_end=ABT_get_wtime();
	printf("total_count=%d completed in %fs!\n",total_count,t_end-t_start);
}

