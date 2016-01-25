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


typedef struct message_outerloop{
//    char core[CmiMsgHeaderSizeBytes];
    float * ptr;
    float value;
    int niterations;
    int nthreads;
    int it_start;
    int it_end;
} Message_outerloop;


typedef struct message_innerloop{
  //char core[CmiMsgHeaderSizeBytes]; 
    float * ptr;
    float value;
    int outer_pos;
    int inner_end_pos;
    int inner_start_pos;
    int niterations;
} Message_innerloop;

pvd(int, funcHandlerVectorScal);
pvd(int, funcHandlerTaskCreator);


static void vectorScal(Message_innerloop *msg)
{
    int outer_pos = msg->outer_pos;
    int inner_start_pos = msg->inner_start_pos;
    int inner_end_pos = msg->inner_end_pos;
    float value = msg->value;
    float * ptr = msg->ptr;
    int niterations = msg->niterations;
    int i;

#ifdef VERBOSE
     printf("Thread: %d (CPU: %d), pos_ini: %d, pos_fin: %d\n",CmiMyRank(), sched_getcpu(),  inner_start_pos*outer_pos, inner_end_pos*outer_pos);
#endif

    for(i=inner_start_pos;i<inner_end_pos;i++){
#ifdef VERBOSE
     printf("Thread: %d (CPU: %d), iter: %d\n",CmiMyRank(), sched_getcpu(),  outer_pos*niterations+i);
#endif
        ptr[outer_pos*niterations+i] *= value;
    }

}

static void taskCreator(Message_outerloop *msg){
    int i;
    int nthreads;
    int it_start, it_end, niterations;

    it_start=msg->it_start;
    it_end=msg->it_end;
    nthreads = msg->nthreads;
    niterations = msg->niterations;
    float value = msg->value;
    float * ptr = msg->ptr;
    int total=(it_end-it_start)*nthreads;
#ifdef VERBOSE
    printf("#Thread: %d (CPU: %d),  en task_creator y creare %d tareas para el rango de iteraciones de %d a %d del loop externo\n", CmiMyRank(), sched_getcpu(), nthreads, it_start, it_end);
#endif

    Message_innerloop * messages;
    messages = (Message_innerloop *)malloc(sizeof(Message_innerloop )*total);
    CthThread * workers = (CthThread *)malloc(sizeof(CthThread)*total);
    
    /*Message_innerloop * messages;
    messages = (Message_innerloop *)malloc(sizeof(Message_innerloop )*nthreads);
    CthThread * workers = (CthThread *)malloc(sizeof(CthThread)*nthreads);
    */
    int ct; //current_task
    int start, end;
    int bloc = niterations / nthreads;
    int rest = niterations % nthreads;
    int j,a;
    ct=0;
    for(i=it_start;i<it_end;i++){

        start=end=0;

        for (a = 0; a < nthreads; a++) {

            start=end;
            end=start+bloc;
            if(a<rest){end++;}
            messages[ct].outer_pos=i;
            messages[ct].inner_start_pos=start;
            messages[ct].inner_end_pos=end;
            messages[ct].value=value;
            messages[ct].ptr=ptr;
            messages[ct].niterations=niterations;

           workers[ct] = CthCreateMigratable((CthVoidFn)vectorScal, (void *) &messages[ct],0);
           CthAwaken(workers[ct]);
	   ct++;
        //CmiSetHandler(messages[i], pva(funcHandlerVectorScal));
        //CmiSyncSend(i, size, messages[i]);

#ifdef VERBOSE
printf("#Thread %d: creada mi tarea %d\n", CmiMyRank(), ct);
#endif
        }
    
        //CthYield(); 
        //CthYield(); 
        //CthYield(); 

    }
        //CthYield(); 
         
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
    	#ifdef VERBOSE
		printf("mymain despues de primer yield\n");
        #endif
        CthYield();
    	#ifdef VERBOSE
		printf("mymain despues de segundo yield\n");
        #endif
    //    CthYield(); 
    	#ifdef VERBOSE
		printf("mymain despues de tercer yield\n");
        #endif
    	CmiNodeBarrier();
    }
#ifdef VERBOSE
printf("#Thread %d: saliendo... \n", CmiMyRank());
//sleep(20);
#endif
    CmiNodeBarrier();
    CsdExitScheduler();

}


int main(int argc, char * argv[]){

  ConverseInit(argc, argv, mymain, 1, 1);
  int niterations,i,t;
  if (argc>1){
	niterations=atoi(argv[1]);
  }
  else{
	niterations=10;
  }

  // int granularity = argc > 2 ? atoi(argv[2]) : 1;
  //int total=ntasks*granularity;
  int total=niterations*niterations;

  double times[TIMES];
  double times_join[TIMES];
  struct timeval t_start, t_start2, t_end;

  float *a;
  a=malloc(sizeof (float)*total);
        
 
  init();

CthThread * workers = (CthThread *)malloc(sizeof(CthThread)*CmiMyNodeSize());
   Message_outerloop * messages = (Message_outerloop *)malloc(sizeof(Message_outerloop)*CmiMyNodeSize());
   //Message_outerloop ** messages = malloc(sizeof(Message_outerloop *)*CmiMyNodeSize());
  for (t = 0; t < TIMES; t++) {
    for (i = 0; i < total; i++) {
      a[i] = i * 1.0f;
    }
    
    gettimeofday(&t_start, NULL);

    int bloc = niterations / (CmiMyNodeSize());
    int rest = niterations % (CmiMyNodeSize());
    int start = 0;
    int end = 0;
    int num_workers=CmiMyNodeSize();
    	int size = sizeof(Message_outerloop);
    for(i=0;i<CmiMyNodeSize();i++){
	start = end;
        int inc = (i < rest) ? 1 : 0;
        end += bloc + inc;
    	//messages[i] = (Message_outerloop *) CmiAlloc(size);
    	/*messages[i]->it_start = start;
    	messages[i]->it_end = end;
    	messages[i]->value = 0.9f;
    	messages[i]->ptr = a;
    	messages[i]->niterations=niterations;
    	messages[i]->nthreads=num_workers;*/
    	messages[i].it_start = start;
    	messages[i].it_end = end;
    	messages[i].value = 0.9f;
    	messages[i].ptr = a;
    	messages[i].niterations=niterations;
    	messages[i].nthreads=num_workers;
#ifdef VERBOSE
	printf("#Thread %d/%d: Repartiendo iteraciones de %d a %d para %d\n", CmiMyRank(),CmiMyNodeSize() ,start, end, i);
#endif
       workers[i] = CthCreateMigratable((CthVoidFn)taskCreator, (void *) &messages[i],1600000);
       CthAwaken(workers[i]);

     	//CmiSetHandler(messages[i], pva(funcHandlerTaskCreator));
        //CmiSyncSend(i, size, messages[i]);
    }


    CmiNodeBarrier();
    	CthYield();
    	#ifdef VERBOSE
		printf("main despues de primer yield\n");
        #endif
        CthYield();
    	#ifdef VERBOSE
		printf("main despues de segundo yield\n");
        #endif
      //  CthYield(); 
    	#ifdef VERBOSE
		printf("main despues de tercer yield\n");
        #endif
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
            CmiMyNodeSize(), niterations, avg, min, max, dev,avgj/TIMES);

    CmiNodeBarrier();
 
#ifdef VERBOSE
printf("#Thread %d: saliendo... \n", CmiMyRank());
//sleep(10);
#endif
//sleep(10);
    for (i = 0; i < total; i++) {
        if (a[i] != i * 0.9f) {
            printf("a[%d]=%f\n", i,a[i]);
//            return 0;
        }
    }
	CsdExitScheduler();

printf("FIN!\n");

}

