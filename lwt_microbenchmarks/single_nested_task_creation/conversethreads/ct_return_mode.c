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

#define NUM_ELEMS 50
#ifndef VERBOSE
#define TIMES 50
#else
#define TIMES 1
#endif


typedef struct message_vector_scal{
    //char core[CmiMsgHeaderSizeBytes];
    float * ptr;
    float value;
    int pos;
    int gran;
} Message_vector_scal;


typedef struct message_task_creator{
    char core[CmiMsgHeaderSizeBytes]; 
    float * ptr;
    float value;
    int pos;
    int gran;
    int nchildtask;
    int nlevels;
} Message_task_creator;

pvd(int, funcHandlerVectorScal);
pvd(int, funcHandlerTaskCreator);


static void vectorScal(Message_vector_scal *arg)
{
    int pos = arg->pos;
    int gran = arg->gran;
    int i;
    int posfin=pos+gran;
    float value = arg->value;
    float * ptr = arg->ptr;

#ifdef VERBOSE
     printf("Thread: %d (CPU: %d), pos: %d\n",CmiMyRank(), sched_getcpu(),  pos);
#endif

    for(i=pos;i<posfin;i++){
        ptr[i] *= value;
    }

}

static void taskCreator(Message_task_creator *arg){

    int i;
    int pos = arg->pos;
    int gran = arg->gran;
    float value = arg->value;
    float * ptr = arg->ptr;
    int nchild = arg->nchildtask;
    int nlvl = arg->nlevels;

    if(nlvl > 1){
        printf("Aun nada\n");
    }
    else{
#ifdef VERBOSE
        printf("#Thread: %d (CPU: %d), en crear task para posicion %d, nchildtask %d, nlevels %d y granularidad %d\n", CmiMyRank(), sched_getcpu(),pos,nchild,nlvl,gran);
#endif

        Message_vector_scal * messages;
        messages = (Message_vector_scal *)malloc(sizeof(Message_vector_scal )*nchild);
        CthThread * workers = (CthThread *)malloc(sizeof(CthThread)*nchild);
        int t;
        int start = pos;
        int stride=(nchild*nlvl*gran)/nchild;
        for(t=0;t<nchild;t++){
            messages[t].ptr=ptr;
            messages[t].value=value;
            messages[t].pos=start;
            messages[t].gran=gran;
            workers[t] = CthCreateMigratable((CthVoidFn)vectorScal, (void *) &messages[t],0);
            CthAwaken(workers[t]);
#ifdef VERBOSE
            printf("#Thread %d: creada mi tarea %d\n", CmiMyRank(), t);
#endif
            start+=stride;    
        }
    
    }     
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

    int ntasks = argc > 1 ? atoi(argv[1]) : NUM_ELEMS;
    int nchildtask = argc > 2 ? atoi(argv[2]) : 2;
    int nlevels = argc > 3 ? atoi(argv[3]) : 1;
    int granularity = argc > 4 ? atoi(argv[4]) : 1;

    //int total=ntasks*granularity;
    int total=ntasks*nchildtask*nlevels*granularity;
    int stride=nchildtask*nlevels*granularity;
    
    double times[TIMES];
    double times_join[TIMES];
    struct timeval t_start, t_start2, t_end;

    float *a;
    a=malloc(sizeof (float)*total);
    int num_workers=CmiMyNodeSize();
    int t,i,j; 
    init();

//CthThread * workers = (CthThread *)malloc(sizeof(CthThread)*CmiMyNodeSize());
   //Message_task_creator * messages = (Message_task_creator *)malloc(sizeof(Message_task_creator)*CmiMyNodeSize());
   Message_task_creator ** messages = malloc(sizeof(Message_task_creator *)*ntasks);
  for (t = 0; t < TIMES; t++) {
    for (i = 0; i < total; i++) {
      a[i] = i * 1.0f;
    }
    
    gettimeofday(&t_start, NULL);

    int ct=0;
    int size = sizeof(Message_task_creator);
    for (j = 0; j < total; j=j+stride) {

        messages[ct] = (Message_task_creator *) CmiAlloc(size);
        messages[ct]->pos = j;
        messages[ct]->gran = granularity;
        messages[ct]->value = 0.9f;
        messages[ct]->ptr = a;
        messages[ct]->nlevels = nlevels;
        messages[ct]->nchildtask = nchildtask;

//       workers[i] = CthCreateMigratable((CthVoidFn)taskCreator, (void *) &messages[i],0);
//       CthAwaken(workers[i]);

     	CmiSetHandler(messages[ct], pva(funcHandlerTaskCreator));
        CmiSyncSend(ct%num_workers, size, messages[ct]);
        ct++;
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
            CmiMyNodeSize(), total, avg, min, max, dev,avgj/TIMES);

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

