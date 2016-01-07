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
//#ifdef VERBOSE

    printf("(CPU: %d) , mystart: %d, myend: %d\n", sched_getcpu(), mystart, myend);

//#endif

/*for (i = mystart; i < myend; i++) {
        ptr[i] *= value;
    }
sleep(5);*/          
}



void pre_vector_scal(void *arguments) {

CthThread main = CthCreateMigratable((CthVoidFn)vector_scal, arguments, 0);
     CthAwaken(main);
CthYield(); 
CsdExitScheduler();
}


void mymain(int argc, char * argv[]){




 //printf("NUM %d/%d (CmiMyNodeSize()=%d)\n",CmiMyRank(),CmiNumPes(),CmiMyNodeSize());
 //if (CmiMyRank() != CmiMyNodeSize()) {
 if (CmiMyRank() == 0) {

     vector_scal_args_t * args;
     args = (vector_scal_args_t *) malloc(sizeof (vector_scal_args_t)
            * 1);
     args[0].value=0.9f;
     args[0].ptr=a;
     int n=(argc>1)?atoi(argv[1]):10;
     int block = n/CmiMyNodeSize();
     int resto = n%CmiMyNodeSize();
     int start; 
     int end;
/*void * msg = CmiAlloc(CmiMsgHeaderSizeBytes+1000);     
     CmiSetHandler(&msg, CpvAccess(pre_vector_scal));
     CmiSyncSend(1, CmiMsgHeaderSizeBytes+1000, &msg);
*/

  	CthThread main = CthCreateMigratable((CthVoidFn)pre_vector_scal, (void *)&args[0], 0);

     /*if (resto==0){
	start=block*CmiMyRank();
	end=start+block;
     }
     else{
	if(CmiMyRank()<resto){
		if(CmiMyRank()==0){
			start=block*CmiMyRank();
			end=start+block+1;
		}
		else{
			start=block*CmiMyRank()+CmiMyRank();
			end=start+block+1;
		}

	}
	else{
		start=block*CmiMyRank()+resto;
		end=start+block;
	}
     }
     args[0].start=start;
     args[0].end=end;
     printf("Thread %d start->%d, end->%d\n",CmiMyRank(),start,end);
  	CthThread main = CthCreateMigratable((CthVoidFn)pre_vector_scal, (void *)&args[0], 0);
     CthAwaken(main);*/
CthYield(); 
 }
 
 
 	
 printf("fin mymain\n");


}

int main(int argc, char * argv[]){

// printf("MAIN from CPU %d\n",sched_getcpu());
  int n,i;
  if (argc>1){
	n=atoi(argv[1]);
  }
  else{
	n=10;
  }
//printf("n=%d\n",n);
  a=malloc(sizeof (float)*n);
  for (i = 0; i < n; i++) {
      a[i] = i * 1.0f;
  }
        
  ConverseInit(argc, argv, mymain, 0, 0);

for (i = 0; i < n; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }

printf("FIN!\n");
return 0;
}

