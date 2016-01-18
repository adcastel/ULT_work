#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>

#include <stdio.h>
#include "converse.h"
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

#define pva CsvAccess
#define pvd CsvStaticDeclare
#define pvi CsvInitialize

#ifndef VERBOSE
#define TIMES 50
#else
#define TIMES 1
#endif


typedef struct message_task_creator{
  char core[CmiMsgHeaderSizeBytes];
  float * ptr;
  float value;
  int pos;
  int gran;
  int it_start;
  int it_end;

} Message_task_creator;


typedef struct message_vector_scal{
  //char core[CmiMsgHeaderSizeBytes];
  float * ptr;
  float value;
  int pos;
  int gran;
 
} Message_vector_scal;

pvd(int, funcHandlerVectorScal);
pvd(int, funcHandlerTaskCreator);


static void vectorScal(Message_vector_scal *msg)
{
    float value = msg->value;
    float * ptr = msg->ptr;
    int pos = msg->pos;
    int gran = msg->gran;
    int posfin = pos+gran;
    int i;
#ifdef VERBOSE

     printf("Thread: %d (CPU: %d), pos->%d\n",CmiMyRank(), sched_getcpu(), pos);

#endif

    for (i = pos; i < posfin; i++) {
            ptr[i] *= value;
    }

}

static void taskCreator(Message_task_creator *msg){
    int i;
    int ntasks;
    int it_start, it_end, gran;
    it_start=msg->it_start;
    it_end=msg->it_end;
    gran=msg->gran;
    ntasks=it_end-it_start;
    float value = msg->value;
    float * ptr = msg->ptr;
    Message_vector_scal * messages;
    messages = (Message_vector_scal *)malloc(sizeof(Message_vector_scal )*ntasks);
    CthThread * workers = (CthThread *)malloc(sizeof(CthThread)*ntasks);
#ifdef VERBOSE
    printf("#Thread: %d (CPU: %d),  crear task y creare  %d tareas (%d-%d) de granularidad %d\n", CmiMyRank(), sched_getcpu(), ntasks, it_end, it_start, gran);
#endif

    int ct=0;
    int size = sizeof(Message_vector_scal);

    for(i=it_start;i<it_end;i++){
        //messages[ct] = (Message_vector_scal *) CmiAlloc(size);
        messages[ct].pos=i*gran;
        messages[ct].value=value;
        messages[ct].ptr=ptr;
        messages[ct].gran=gran;
        workers[ct] = CthCreateMigratable((CthVoidFn)vectorScal, (void *) &messages[ct],0);
	CthAwaken(workers[ct]);
        //CmiSetHandler(messages[i], pva(funcHandlerVectorScal));
        //CmiSyncSend(i, size, messages[i]);

#ifdef VERBOSE
printf("#Thread %d: creada mi tarea %d\n", CmiMyRank(), ct);
#endif
ct++;
}
    CthYield(); 

}





void init(){

    pvi(int, funcHandlerVectorScal);
    pva(funcHandlerVectorScal) = CmiRegisterHandler((CmiHandler)vectorScal);
    pvi(int, funcHandlerTaskCreator);
    pva(funcHandlerTaskCreator) = CmiRegisterHandler((CmiHandler)taskCreator);
}


void mymain(int argc, char * argv[]){
    int t;
    init();
    int id=CmiMyRank();
    for (t = 0; t < TIMES; t++) {
    	CmiNodeBarrier();
    	CthYield();
        CthYield(); 
    	CmiNodeBarrier();
    }
#ifdef VERBOSE
printf("#Thread %d: saliendo... \n", CmiMyRank());
//sleep(20);
#endif
    CsdExitScheduler();

}


int main(int argc, char * argv[]){

  ConverseInit(argc, argv, mymain, 1, 1);
  int ntasks,i,t;
  if (argc>1){
	ntasks=atoi(argv[1]);
  }
  else{
	ntasks=10;
  }

   int granularity = argc > 2 ? atoi(argv[2]) : 1;
  int total=ntasks*granularity;

  double times[TIMES];
  double times_join[TIMES];
  struct timeval t_start, t_start2, t_end;

  float *a;
  a=malloc(sizeof (float)*total);
        
 
  init();

//CthThread * workers = (CthThread *)malloc(sizeof(CthThread)*CmiMyNodeSize());
   //Message_task_creator * messages = (Message_task_creator *)malloc(sizeof(Message_task_creator)*CmiMyNodeSize());
   Message_task_creator ** messages = malloc(sizeof(Message_task_creator *)*CmiMyNodeSize());
  for (t = 0; t < TIMES; t++) {
    for (i = 0; i < total; i++) {
      a[i] = i * 1.0f;
    }
    
    gettimeofday(&t_start, NULL);

    int bloc = ntasks / (CmiMyNodeSize());
    int rest = ntasks % (CmiMyNodeSize());
    int start = 0;
    int end = 0;
    	int size = sizeof(Message_task_creator);
    for(i=0;i<CmiMyNodeSize();i++){
	start = end;
        int inc = (i < rest) ? 1 : 0;
        end += bloc + inc;
    	messages[i] = (Message_task_creator *) CmiAlloc(size);
    	messages[i]->it_start = start;
    	messages[i]->it_end = end;
    	messages[i]->value = 0.9f;
    	messages[i]->ptr = a;
    	messages[i]->gran = granularity;
#ifdef VERBOSE
	printf("#Thread %d/%d: Repartiendo iteraciones de %d a %d para %d\n", CmiMyRank(),CmiMyNodeSize() ,start, end, i);
#endif
//       workers[i] = CthCreateMigratable((CthVoidFn)taskCreator, (void *) &messages[i],0);
//       CthAwaken(workers[i]);

     	CmiSetHandler(messages[i], pva(funcHandlerTaskCreator));
        CmiSyncSend(i, size, messages[i]);
    }


    CmiNodeBarrier();
    CthYield(); 
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
            CmiMyNodeSize(), ntasks, avg, min, max, dev,avgj/TIMES);

 
#ifdef VERBOSE
printf("#Thread %d: saliendo... \n", CmiMyRank());
//sleep(10);
#endif
    for (i = 0; i < total; i++) {
        if (a[i] != i * 0.9f) {
            printf("%f\n", a[i]);
            return 0;
        }
    }
	CsdExitScheduler();

printf("FIN!\n");

}

