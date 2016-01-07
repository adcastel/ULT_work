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
  int start;
  int end;
 
} Message;

pvd(int, funcHandler);


static void vectorScal(Message *msg)
{
    int mystart = msg->start;
    int myend = msg->end;
    float value = msg->value;
    float * ptr = msg->ptr;
    int i;
#ifdef VERBOSE

     printf("Thread: %d (CPU: %d), start->%d, end->%d\n",CmiMyRank(), sched_getcpu(), mystart,myend);

#endif

    for (i = mystart; i < myend; i++) {
            ptr[i] *= value;
    }

}

void init(){

    pvi(int, funcHandler);
    pva(funcHandler) = CmiRegisterHandler((CmiHandler)vectorScal);
}


void mymain(int argc, char * argv[]){
    int t;
    init();
    int id=CmiMyRank();
    for (t = 0; t < TIMES; t++) {
    	CmiNodeBarrier();
    	CthYield();
    	CmiNodeBarrier();
    }
    CsdExitScheduler();

}


int main(int argc, char * argv[]){

  int n,i,t;
  if (argc>1){
	n=atoi(argv[1]);
  }
  else{
	n=10;
  }
  double times[TIMES];
  double times_join[TIMES];
  struct timeval t_start, t_start2, t_end;

  float *a;
  a=malloc(sizeof (float)*n);
        
  ConverseInit(argc, argv, mymain, 1, 1);
 
  init();


   Message ** messages = malloc(sizeof(Message *)*CmiMyNodeSize());
  for (t = 0; t < TIMES; t++) {
    for (i = 0; i < n; i++) {
      a[i] = i * 1.0f;
    }
    
    gettimeofday(&t_start, NULL);

    int bloc = n / (CmiMyNodeSize());
    int rest = n % (CmiMyNodeSize());
    int start = 0;
    int end = 0;
    	int size = sizeof(Message);
    for(i=0;i<CmiMyNodeSize();i++){
	start = end;
        int inc = (i < rest) ? 1 : 0;
        end += bloc + inc;
    	messages[i] = (Message *) CmiAlloc(size);
    	messages[i]->start = start;
    	messages[i]->end = end;
    	messages[i]->value = 0.9f;
    	messages[i]->ptr = a;
    	CmiSetHandler(messages[i], pva(funcHandler));
        CmiSyncSend(i, size, messages[i]);
    }


    CmiNodeBarrier();
    CthYield(); 
    gettimeofday(&t_start2, NULL);

    CmiNodeBarrier();
    
    gettimeofday(&t_end, NULL);

    double time = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                        (t_start.tv_sec * 1000000 + t_start.tv_usec);

    times[t] = (time / 1000000.0);

    double time_join = (t_end.tv_sec * 1000000 + t_end.tv_usec) -
                        (t_start2.tv_sec * 1000000 + t_start2.tv_usec);

    times_join[t] = (time_join / 1000000.0);

  }
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
            CmiMyNodeSize(), n, avg, min, max, dev,avgj/TIMES);

    for (i = 0; i < n; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }
 
	CsdExitScheduler();

printf("FIN!\n");

}

