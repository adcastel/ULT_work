#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

#include <stdio.h>
#include "converse.h"
#include <math.h>
#include <sys/time.h>


#define pva CsvAccess
#define pvd CsvStaticDeclare
#define pvi CsvInitialize

#ifndef VERBOSE
#define TIMES 50
#else
#define TIMES 1
#endif


typedef struct message_{
  char core[CmiMsgHeaderSizeBytes];
  float * ptr;
  float value;
  int pos;
  int gran;
 
} Message;

pvd(int, funcHandler);


static void vectorScal(Message *msg)
{
    int pos = msg->pos;
    int gran = msg->gran;
    int i;
    int posfin=pos+gran;
    float value = msg->value;
    float * ptr = msg->ptr;
#ifdef VERBOSE

     printf("Thread: %d (CPU: %d), pos->%d\n",CmiMyRank(), sched_getcpu(), pos);

#endif
    for(i=pos;i<posfin;i++){
            ptr[i] *= value;
    }
}

void init(){

    pvi(int, funcHandler);
    pva(funcHandler) = CmiRegisterHandler((CmiHandler)vectorScal);
}


void mymain(int argc, char * argv[]){
  init();
  int id=CmiMyRank();
  int ntasks,i,t;
  float *a;
  double times[TIMES];
  double times_join[TIMES];
  struct timeval t_start, t_start2, t_end;
  Message ** messages;
  if (argc>1){
        ntasks=atoi(argv[1]);
  }
  else{
        ntasks=10;
  }

  int granularity = argc > 2 ? atoi(argv[2]) : 1;
  int total=ntasks*granularity;

  if(id==0){
	a=malloc(sizeof (float)*total);
      	messages = malloc(sizeof(Message *)*ntasks);
  }
  for (t = 0; t < TIMES; t++) {
  	if(id==0){
      
        	for (i = 0; i < total; i++) {
        		a[i] = i * 1.0f;
        	}
        	
		gettimeofday(&t_start, NULL);

		int size = sizeof(Message);
                int current_task=0;

		for(i=0;i<total;i+=granularity){
          
          		messages[current_task] = (Message *) CmiAlloc(size);
          		messages[current_task]->pos = i;
          		messages[current_task]->gran = granularity;
          		messages[current_task]->value = 0.9f;
          		messages[current_task]->ptr = a;
          
	  		CmiSetHandler(messages[current_task], pva(funcHandler));
          		CmiSyncSend(current_task%CmiMyNodeSize(), size, messages[current_task]);
      			current_task++;
		}

  	}

        CmiNodeBarrier();

        CthYield();
        gettimeofday(&t_start2, NULL);

        CmiNodeBarrier();
	if(id==0){
		gettimeofday(&t_end, NULL);
		
		double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                	(t_start.tv_sec * 1000000 + t_start.tv_usec);
	  
		times[t] = (time / 1000000.0);
        	
		double time_join = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
            		(t_start2.tv_sec * 1000000 + t_start2.tv_usec);
        
		times_join[t] = (time_join / 1000000.0);
	}	
        CmiNodeBarrier();
  }
 

  if(id==0){
	double min, max, avg, aux, sigma, dev;
    	double avgj=times_join[0];
    	min = times[0];
    	max = times[0];
    	aux = times[0];
    
	for (t = 1; t < TIMES; t++) {
        	if (times[t] < min) min = times[t];
        	if (times[t] > max) max = times[t];
        	aux += times[t];
        	avgj+=times_join[t];
    	}
    
	avg = aux / TIMES;
    	for (t = 0; t < TIMES; t++) {
        	sigma = sigma + ((times[t] - avg)*(times[t] - avg));
    	}
#ifndef VERBOSE
    	dev = sqrt(sigma / (TIMES - 1));
#else
    	dev = sqrt(sigma);
#endif
    	printf("%d %d %f [%f - %f] %f Join(%f)\n",
            CmiMyNodeSize(), ntasks, avg, min, max, dev,avgj/TIMES);
    
        for (i = 0; i < total; i++) {
          if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
     }
  }

  CmiNodeBarrier();
  ConverseExit();

}


int main(int argc, char * argv[]){

        
  ConverseInit(argc, argv, mymain, 0, 0); 
  return 0;
}

