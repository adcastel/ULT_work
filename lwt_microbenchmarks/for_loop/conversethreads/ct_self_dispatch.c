#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

#include <stdio.h>
#include "converse.h"




  float * a;


typedef struct {
    float * ptr;
    float value;
    int start;
    int end;
} vector_scal_args_t;

void vector_scal(void *arguments) {
    vector_scal_args_t *arg;
    arg = (vector_scal_args_t *) arguments;
    int mystart = arg->start;
    int myend = arg->end;
    float value = arg->value;
    float * ptr = arg->ptr;
    int i;
#ifdef VERBOSE

    printf("Thread: %d (CPU: %d) , mystart: %d, myend: %d\n", CmiMyRank(), sched_getcpu(), mystart, myend);

#endif

for (i = mystart; i < myend; i++) {
        ptr[i] *= value;
    }
}



void pre_vector_scal(void *arguments) {

     int n=*(int *)arguments;
     int block = n/CmiMyNodeSize();
     int resto = n%CmiMyNodeSize();
     int start; 
     int end;
     int i;
     struct timeval t_start, t_start2, t_end;
     int id=CmiMyRank();
     if (CmiMyRank() != CmiMyNodeSize()) {
        CmiNodeBarrier();
	if(id==0)
               gettimeofday(&t_start, NULL);

     	vector_scal_args_t * args;
     	args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * 1);
     	args[0].value=0.9f;
     	args[0].ptr=a;
     	if (resto==0){
		start=block*CmiMyRank();
		end=start+block;
     	}
     	else{
		if(CmiMyRank()<resto){
			start=block*CmiMyRank()+CmiMyRank();
			end=start+block+1;

	}
	else{
		start=block*CmiMyRank()+resto;
		end=start+block;
	}
     }
     args[0].start=start;
     args[0].end=end;
     CthThread main = CthCreateMigratable((CthVoidFn)vector_scal, (void *)&args[0], 0);
     CthAwaken(main);
     CthYield(); 
  } 
CmiNodeBarrier();
	if(id==0){
                gettimeofday(&t_end, NULL);
		double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                        (t_start.tv_sec * 1000000 + t_start.tv_usec);
		printf("%d %d %f \n",
                    CmiMyNodeSize(), n, time/1000000.0);

	}
CmiNodeBarrier();
for (i = start; i < end; i++) {
        if (a[i] != i * 0.9f) {
            printf("a[%d]=%f != %f\n", i,a[i],i*0.9f);
	    CsdExitScheduler();
        }
    }
CsdExitScheduler();
}


void mymain(int argc, char * argv[]){

 if (CmiMyRank() != CmiMyNodeSize()) {
     int n=atoi(argv[1]);
  	CthThread main = CthCreateMigratable((CthVoidFn)pre_vector_scal, (void*)&n, 0);
        CthAwaken(main);
	CthYield();
 }
 
 	


}

int main(int argc, char * argv[]){

  int n,i;
  if (argc>1){
	n=atoi(argv[1]);
  }
  else{
	n=10;
  }
  a=malloc(sizeof (float)*n);
  for (i = 0; i < n; i++) {
      a[i] = i * 1.0f;
  }
        
  ConverseInit(argc, argv, mymain, 0, 0); 


return 0;
}

