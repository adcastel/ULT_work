#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <alloca.h>
#include <omp.h>


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
	       if (q == p || q == p - (j - i) || q == p + (j - i))
		    return 0;
	  }
     }
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



void nqueens(int n, int j, char *a, int *solutions, int depth)

{

	int *csols;
	int i;

	//printf("%d inicio nqueens con j=%d\n", omp_get_thread_num(),j);
	if (n == j) {
		/* good solution, count it */

		*solutions = 1;

		return;
	}



	*solutions = 0;
	csols = alloca(n*sizeof(int));
	memset(csols,0,n*sizeof(int));


     	/* try each possible position for queen <j> */
	for (i = 0; i < n; i++) {
 		#pragma omp task untied
		{
	  		/* allocate a temporary array and copy <a> into it */
	  		char * b = alloca(n * sizeof(char));
	  		memcpy(b, a, j * sizeof(char));
	  		b[j] = (char) i;
	  		if (ok(j + 1, b))

       				nqueens(n, j + 1, b,&csols[i],depth); //FIXME: depth or depth+1 ???

		}
	}

	#pragma omp taskwait

	for ( i = 0; i < n; i++) *solutions += csols[i];

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
	#pragma omp parallel
	{
		#pragma omp master
        	{
			printf("Computing N-Queens algorithm (n=%d) ", size);
			printf(" using %d threads ", omp_get_num_threads());
		}
	}
	double t_start = omp_get_wtime();
	#pragma omp parallel
	{

		#pragma omp single
		{

			char *a;

			a = alloca(size * sizeof(char));

			nqueens(size, 0, a, &total_count,0);

		}

	}
	double t_end=omp_get_wtime();
	printf("total count %d completed in %fs!\n",total_count,t_end-t_start);
}


