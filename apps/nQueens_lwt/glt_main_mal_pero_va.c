#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <alloca.h>
//#include <omp.h>
#ifdef FASTGLT
    #include <fast_glt.h>
#else
   #include <glt.h>
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


void nqueens_ser (int n, int j, char *a, int *solutions)
{

	int res;
	int i;

	if (n == j) {
		/* good solution, count it */

		*solutions = 1;

		return;
	}


	*solutions = 0;


     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {
		{
	  		/* allocate a temporary array and copy <a> into it */
	  		a[j] = (char) i;
	  		if (ok(j + 1, a)) {

	       			nqueens_ser(n, j + 1, a,&res);
				*solutions += res;

			}
		}
	}
}


typedef struct glt_args {
	int n;
	int j;
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

void nqueens(/*int n, int j, char *a, int *solutions, int depth*/void * arguments)
{
	int me = glt_get_thread_num();
	//printf("%d inicio nqueens\n",me);
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

		*solutions = 1;
		//printf("Good solution!\n");
		return;
	}


		//printf("%d despues de sol\n",me);
	*solutions = 0;
	csols = alloca(n*sizeof(int));
	memset(csols,0,n*sizeof(int));
	
	glt_args_t * out_args = (glt_args_t *) malloc(sizeof(glt_args_t)*n);

#ifdef TASKLET
	GLT_tasklet * task_id = glt_tasklet_malloc(n);
#else
	GLT_ult * ult_id = glt_ult_malloc(n);
#endif

	int creadas = 0;
     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {

	  		/* allocate a temporary array and copy <a> into it */
	  		char * b = alloca(n * sizeof(char));
	  		memcpy(b, a, j * sizeof(char));
	  		b[j] = (char) i;
			int dest, id;
			glt_workunit_get_thread_id(&id);
	  		if (ok(j + 1, b))
			{
				out_args[i].n=n;
				out_args[i].j=j+1;
				out_args[i].a=b;
				out_args[i].sol=&csols[i];
				out_args[i].depth=depth;

				if(global_dispatch>=glt_get_num_threads()){
					dest = id;
				}
				else
					dest = (id !=0)? id : global_dispatch++;
				//printf("Soy %d/%d y voy a crear a %d\n",id,glt_get_num_threads(),dest); 
       				//nqueens(n, j + 1, b,&csols[i],depth); //FIXME: depth or depth+1 ???
#ifdef TASKLET
			        glt_tasklet_creation_to(nqueens,(void *)&out_args[i],&task_id[i],dest);
				//glt_tasklet_creation_to(no_func,NULL,&task_id[i],dest);
#else
			        glt_ult_creation_to(nqueens,(void *)&out_args[i],&ult_id[i],dest);
				//glt_ult_creation_to(no_func,NULL,&ult_id[i],dest);
#endif

				creadas++;			
			}
			else
			{
#ifdef TASKLET
			        glt_tasklet_creation_to(no_func,NULL,&task_id[i],id);
#else
			        glt_ult_creation_to(no_func,NULL,&ult_id[i],id);
#endif
			}

	}
		//printf("%d despues del for\n",me);
	glt_yield();
		//printf("%d despues del yield\n",me);

for(i = 0;i<n;i++)
	{
			//printf("%d para join %d\n",me,i);
#ifdef TASKLET
		glt_tasklet_join(&task_id[i]);
#else
		glt_ult_join(&ult_id[i]);
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

//void find_queens (int size)
int main(int argc, char * argv [])
{
	total_count=0;
        int size = atoi(argv[1]);

	glt_init(argc,argv);

	//#pragma omp parallel
	//{
	//	#pragma omp master
        //	{
			printf("Computing N-Queens algorithm (n=%d) ", size);
			printf(" using %d threads ",glt_get_num_threads());
	//	}
	//}
	double t_start = glt_get_wtime();


	glt_args_t init_arg;
//#ifndef TASKLET
	GLT_ult * ult_id = glt_ult_malloc(1);
//#else
//	GLT_tasklet * task_id = glt_tasklet_malloc(1);
//#endif
	//#pragma omp parallel
	//{
	//
	//	#pragma omp single
	//	{
	//
	//		char *a;
	init_arg.n=size;
	init_arg.j=0;
	init_arg.a=alloca(size * sizeof(char));
	init_arg.sol=&total_count;
	init_arg.depth=0;
	
//#ifdef TASKLET
//	glt_tasklet_creation_to(nqueens,(void *)&init_arg,&task_id[0],0);	
//#else
	glt_ult_creation_to(nqueens,(void *)&init_arg,&ult_id[0],0);	
//#endif

			//nqueens(size, 0, a, &total_count,0);

		//}

	//}
	glt_yield();
//#ifdef TASKLET
//	glt_tasklet_join(&task_id[0]);
//#else
	//printf("En el main\n");
	glt_ult_join(&ult_id[0]);
//#endif
	
	double t_end=glt_get_wtime();
	printf(" completed in %fs!\n",t_end-t_start);
}


