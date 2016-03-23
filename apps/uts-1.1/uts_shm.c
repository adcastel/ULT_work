/*
 *         ---- The Unbalanced Tree Search (UTS) Benchmark ----
 *  
 *  Copyright (c) 2010 See AUTHORS file for copyright holders
 *
 *  This file is part of the unbalanced tree search benchmark.  This
 *  project is licensed under the MIT Open Source license.  See the LICENSE
 *  file for copyright and licensing information.
 *
 *  UTS is a collaborative project between researchers at the University of
 *  Maryland, the University of North Carolina at Chapel Hill, and the Ohio
 *  State University.  See AUTHORS file for more information.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "uts.h"
//#define ARGOBOTS
/***********************************************************
 *                                                         *
 *  Compiler Type (these flags are set by at compile time) *
 *     (default) ANSI C compiler - sequential execution    *
 *     (_OPENMP) OpenMP enabled C compiler                 *
 *     (__UPC__) UPC compiler                              *
 *     (_SHMEM)  Cray Shmem                                *
 *     (__PTHREADS__) Pthreads multithreaded execution     *
 *                                                         *
 ***********************************************************/

/**** OpenMP Definitions ****/
#ifdef _OPENMP
#include <omp.h>
#define PARALLEL         1
#define COMPILER_TYPE    1
#define SHARED 
#define SHARED_INDEF
#define VOLATILE         volatile
#define MAX_THREADS       32
#define LOCK_T           omp_lock_t
#define GET_NUM_THREADS  omp_get_num_threads()
#define GET_THREAD_NUM   omp_get_thread_num()
#define SET_LOCK(zlk)    omp_set_lock(zlk)
#define UNSET_LOCK(zlk)  omp_unset_lock(zlk)
#define INIT_LOCK(zlk)   zlk=omp_global_lock_alloc()
#define INIT_SINGLE_LOCK(zlk) zlk=omp_global_lock_alloc()
#define SMEMCPY          memcpy
#define ALLOC            malloc
#define BARRIER          
// OpenMP helper function to match UPC lock allocation semantics
omp_lock_t * omp_global_lock_alloc() {
  omp_lock_t *lock = (omp_lock_t *) malloc(sizeof(omp_lock_t) + 128);
  omp_init_lock(lock);
  return lock;
}


/**** UPC Definitions ****/
#elif defined(__UPC__)
#include <upc.h>
#define PARALLEL         1
#define COMPILER_TYPE    2
#define SHARED           shared
#define SHARED_INDEF     shared [0]
#define VOLATILE         strict
#define MAX_THREADS       (THREADS)
#define LOCK_T           upc_lock_t
#define GET_NUM_THREADS  (THREADS)
#define GET_THREAD_NUM   (MYTHREAD)
#define SET_LOCK(zlk)    upc_lock(zlk)
#define UNSET_LOCK(zlk)  upc_unlock(zlk)
#define INIT_LOCK(zlk)   zlk=upc_global_lock_alloc()
#define INIT_SINGLE_LOCK(zlk) zlk=upc_all_lock_alloc()
#define SMEMCPY          upc_memget
#define ALLOC            upc_alloc
#define BARRIER          upc_barrier;


/**** Shmem Definitions ****/
#elif defined(_SHMEM)
#include <mpp/shmem.h>
#define PARALLEL         1
#define COMPILER_TYPE    3
#define SHARED           
#define SHARED_INDEF
#define VOLATILE         volatile
#define MAX_THREADS       64
#define LOCK_T           long
#define GET_NUM_THREADS  shmem_n_pes()
#define GET_THREAD_NUM   shmem_my_pe()
#define SET_LOCK(zlk)    shmem_set_lock(zlk)
#define UNSET_LOCK(zlk)  shmem_clear_lock(zlk)
#define INIT_LOCK(zlk)   zlk = shmem_global_lock_alloc()
#define INIT_SINGLE_LOCK(zlk)	zlk = shmem_global_lock_alloc()
#define SMEMCPY          shmem_getmem
  // Shmem's get has different semantics from memcpy():
  //   void shmem_getmem(void *target, const void *source, size_t len, int pe)
#define ALLOC            shmalloc
#define BARRIER          shmem_barrier_all();

// Shmem helper function to match UPC lock allocation semantics
LOCK_T * shmem_global_lock_alloc() {    
    LOCK_T *lock = (LOCK_T *) shmalloc(sizeof(LOCK_T));
    *lock = 0;
    return lock;
}

#define GET(target,source,from_id) shmem_int_get(&(target),&(source),1,from_id)        
#define PUT(target,source,to_id)   shmem_int_put(&(target),&(source),1,to_id)

#define PUT_ALL(a,b)								\
	do {						 			\
		int _iter, _node; 						\
		for (_iter = 1; _iter < GET_NUM_THREADS; _iter++) {		\
			_node = (GET_THREAD_NUM + _iter) % GET_NUM_THREADS;	\
			shmem_int_put((int *)&a,(int *)&b,1,_node); \
		}								\
	} while(0)


/**** Pthreads Definitions ****/
#elif defined(__PTHREADS__)
#include <pthread.h>
#define PARALLEL         1
#define COMPILER_TYPE    4
#define SHARED
#define SHARED_INDEF
#define VOLATILE         volatile
#define MAX_THREADS       128
#define LOCK_T           pthread_mutex_t
#define GET_NUM_THREADS  pthread_num_threads
#define GET_THREAD_NUM   *(int*)pthread_getspecific(pthread_thread_num)
#define SET_LOCK(zlk)    pthread_mutex_lock(zlk)
#define UNSET_LOCK(zlk)  pthread_mutex_unlock(zlk)
#define INIT_LOCK(zlk)   zlk = pthread_global_lock_alloc()
#define INIT_SINGLE_LOCK(zlk)  zlk = pthread_global_lock_alloc()
#define SMEMCPY          memcpy
#define ALLOC            malloc
#define BARRIER           

int pthread_num_threads = 1;              // Command line parameter - default to 1
pthread_key_t pthread_thread_num;         // Key to store each thread's ID

/* helper function to match UPC lock allocation semantics */
LOCK_T * pthread_global_lock_alloc() {    
    LOCK_T *lock = (LOCK_T *) malloc(sizeof(LOCK_T));
    pthread_mutex_init(lock, NULL);
    return lock;
}
#elif defined(GLT)
#ifdef FASTGLT
#include <fast_glt.h>
#else
#include <glt.h>
#endif
#define PARALLEL         1
#define COMPILER_TYPE    1
#define SHARED
#define SHARED_INDEF
#define VOLATILE         volatile
#define MAX_THREADS       128
#define LOCK_T           GLT_mutex
#define GET_NUM_THREADS  glt_get_num_threads()
#define GET_THREAD_NUM   glt_get_thread_num()
#define SET_LOCK(zlk)    glt_mutex_lock(zlk)
#define UNSET_LOCK(zlk)  glt_mutex_unlock(zlk)
#define INIT_LOCK(zlk)   zlk = glt_global_lock_alloc()
#define INIT_SINGLE_LOCK(zlk)  zlk = glt_global_lock_alloc()
#define SMEMCPY          memcpy
#define ALLOC            malloc
#define BARRIER           

//  GLT_barrier barrier;


//int glt_num_threads = 1;              // Command line parameter - default to 1
//pthread_key_t pthread_thread_num;         // Key to store each thread's ID

/* helper function to match UPC lock allocation semantics */
LOCK_T glt_global_lock_alloc() {    
    LOCK_T lock;
    glt_mutex_create(&lock);
    return lock;
}

#elif defined(ARGOBOTS)
#include <abt.h>
#define PARALLEL         1
#define COMPILER_TYPE    1
#define SHARED
#define SHARED_INDEF
#define VOLATILE         volatile
#define MAX_THREADS       128
#define LOCK_T           ABT_mutex
#define GET_NUM_THREADS  abt_num_threads()
#define GET_THREAD_NUM   abt_get_thread_num()
#define SET_LOCK(zlk)    pre_ABT_mutex_lock(zlk)
#define UNSET_LOCK(zlk)  pre_ABT_mutex_unlock(zlk)
#define INIT_LOCK(zlk)   zlk = abt_global_lock_alloc()
#define INIT_SINGLE_LOCK(zlk)  zlk = abt_global_lock_alloc()
#define SMEMCPY          memcpy
#define ALLOC            malloc
#define BARRIER           

//  GLT_barrier barrier;


//int glt_num_threads = 1;              // Command line parameter - default to 1
//pthread_key_t pthread_thread_num;         // Key to store each thread's ID

/* helper function to match UPC lock allocation semantics */
LOCK_T * abt_global_lock_alloc() {  
        printf("Voy a crear el lock\n");

    LOCK_T * lock;
    ABT_mutex_create(lock);
    return lock;
}

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

void pre_ABT_mutex_lock(LOCK_T zlk){
    printf("Voy a hacer el lock\n");
    ABT_mutex_lock(zlk);
        printf("Lock hecho\n");

}

void pre_ABT_mutex_unlock(LOCK_T zlk){
        printf("Voy a hacer el unlock\n");

    ABT_mutex_unlock(zlk);
    printf("unlock hecho\n");

}

#elif defined(QTHREADS)
#include <qthread/qthread.h>
#define PARALLEL         1
#define COMPILER_TYPE    1
#define SHARED
#define SHARED_INDEF
#define VOLATILE         volatile
#define MAX_THREADS       128
#define LOCK_T           aligned_t *
#define GET_NUM_THREADS  qthread_num_shepherds ()
#define GET_THREAD_NUM   qthread_shep()
#define SET_LOCK(zlk)    qthread_lock(zlk)
#define UNSET_LOCK(zlk)  qthread_unlock(zlk)
#define INIT_LOCK(zlk)   //zlk = qth_global_lock_alloc()
#define INIT_SINGLE_LOCK(zlk)  //zlk = qth_global_lock_alloc()
#define SMEMCPY          memcpy
#define ALLOC            malloc
#define BARRIER           

//  GLT_barrier barrier;


//int glt_num_threads = 1;              // Command line parameter - default to 1
//pthread_key_t pthread_thread_num;         // Key to store each thread's ID

/* helper function to match UPC lock allocation semantics */
LOCK_T  qth_global_lock_alloc() {  
     //   printf("Voy a crear el lock\n");
    //LOCK_T lock;
    //return lock;
}

#elif defined(MASSIVETHREADS)
#include <myth.h>
#define PARALLEL         1
#define COMPILER_TYPE    1
#define SHARED
#define SHARED_INDEF
#define VOLATILE         volatile
#define MAX_THREADS       128
#define LOCK_T           myth_mutex_t
#define GET_NUM_THREADS  num_workers
#define GET_THREAD_NUM   myth_get_worker_num()
#define SET_LOCK(zlk)    myth_mutex_lock(zlk)
#define UNSET_LOCK(zlk)  myth_mutex_unlock(zlk)
#define INIT_LOCK(zlk)   zlk = myth_mutex_create()
#define INIT_SINGLE_LOCK(zlk)  zlk = myth_mutex_create()
#define SMEMCPY          memcpy
#define ALLOC            malloc
#define BARRIER           
int num_workers;
//  GLT_barrier barrier;


//int glt_num_threads = 1;              // Command line parameter - default to 1
//pthread_key_t pthread_thread_num;         // Key to store each thread's ID

/* helper function to match UPC lock allocation semantics */
LOCK_T * myth_global_lock_alloc() {  
        printf("Voy a crear el lock\n");

    LOCK_T * lock;
    //ABT_mutex_create(lock);
    return lock;
}


 
/**** Default Sequential Definitions ****/
#else
#define PARALLEL         0
#define COMPILER_TYPE    0
#define SHARED
#define SHARED_INDEF
#define VOLATILE
#define MAX_THREADS 1
#define LOCK_T           void
#define GET_NUM_THREADS  1
#define GET_THREAD_NUM   0
#define SET_LOCK(zlk)    
#define UNSET_LOCK(zlk)  
#define INIT_LOCK(zlk) 
#define INIT_SINGLE_LOCK(zlk) 
#define SMEMCPY          memcpy
#define ALLOC            malloc
#define BARRIER           

#endif /* END Par. Model Definitions */


/***********************************************************
 *  Parallel execution parameters                          *
 ***********************************************************/

int doSteal   = PARALLEL; // 1 => use work stealing


int chunkSize = 15;//20;       // number of nodes to move to/from shared area

int cbint     = 1;        // Cancellable barrier polling interval
int pollint   = 1;        // BUPC Polling interval

#ifdef __BERKELEY_UPC__
/* BUPC nonblocking I/O Handles */
bupc_handle_t cb_handle       = BUPC_COMPLETE_HANDLE;
const int     local_cb_cancel = 1;
#endif


/***********************************************************
 * Tree statistics (if selected via UTS_STAT)              *
 *   compute overall size and imbalance metrics            *
 *   and histogram size and imbalance per level            *
 ***********************************************************/
#ifdef UTS_STAT

/* Check that we are not being asked to compile parallel with stats.
 * Parallel stats collection is presently not supported.  */
#if PARALLEL
#error "ERROR: Parallel stats collection is not supported!"
#endif

#define MAXHISTSIZE      2000  // max tree depth in histogram
int    stats     = 1;
int    unbType   = 1;
int    maxHeight = 0;         // maximum depth of tree
double maxImb    = 0;         // maximum imbalance
double minImb    = 1;
double treeImb   =-1;         // Overall imbalance, undefined

int    hist[MAXHISTSIZE+1][2];      // average # nodes per level
double unbhist[MAXHISTSIZE+1][3];   // average imbalance per level

int    *rootSize;             // size of the root's children 
double *rootUnb;              // imbalance of root's children

/* Tseng statistics */
int    totalNodes = 0;
double imb_max    = 0;         // % of work in largest child (ranges from 100/n to 100%)
double imb_avg    = 0;
double imb_devmaxavg     = 0;  // ( % of work in largest child ) - ( avg work )
double imb_normdevmaxavg = 0;  // ( % of work in largest child ) - ( avg work ) / ( 100% - avg work )
#else
int stats   = 0;
int unbType = -1;
#endif


/***********************************************************
 *  Execution Tracing                                      *
 ***********************************************************/

#define SS_WORK    0
#define SS_SEARCH  1
#define SS_IDLE    2
#define SS_OVH     3
#define SS_CBOVH   4
#define SS_NSTATES 5

/* session record for session visualization */
struct sessionRecord_t {
  double startTime, endTime;
};
typedef struct sessionRecord_t SessionRecord;

/* steal record for steal visualization */
struct stealRecord_t {
  long int nodeCount;           /* count nodes generated during the session */
  int victimThread;             /* thread from which we stole the work  */
};
typedef struct stealRecord_t StealRecord;

/* Store debugging and trace data */
struct metaData_t {
  SessionRecord sessionRecords[SS_NSTATES][20000];   /* session time records */
  StealRecord stealRecords[20000]; /* steal records */
};
typedef struct metaData_t MetaData;

/* holds text string for debugging info */
char debug_str[1000];


/***********************************************************
 * StealStack types                                        *
 ***********************************************************/

#define MAXSTACKDEPTH 500000

/* stack of nodes */
struct stealStack_t
{
  int stackSize;     /* total space avail (in number of elements) */
  int workAvail;     /* elements available for stealing */
  int sharedStart;   /* index of start of shared portion of stack */
  int local;         /* index of start of local portion */
  int top;           /* index of stack top */
  int maxStackDepth;                      /* stack stats */ 
  int nNodes, maxTreeDepth;               /* tree stats  */
  int nLeaves;
  int nAcquire, nRelease, nSteal, nFail;  /* steal stats */
  int wakeups, falseWakeups, nNodes_last;
  double time[SS_NSTATES], timeLast;         /* perf measurements */
  int entries[SS_NSTATES], curState; 
#ifdef GLT
  LOCK_T stackLock; /* lock for manipulation of shared portion */
#else
    #ifdef ARGOBOTS
  LOCK_T stackLock;
#elif defined(QTHREADS)
  LOCK_T stackLock;
#elif defined(MASSIVETHREADS)
  LOCK_T stackLock;
    #else
    LOCK_T *stackLock; /* lock for manipulation of shared portion */
    #endif
#endif
  Node * stack;       /* addr of actual stack of nodes in local addr space */
  SHARED_INDEF Node * stack_g; /* addr of same stack in global addr space */
#ifdef TRACE
  MetaData * md;        /* meta data used for debugging and tracing */
#endif
};
typedef struct stealStack_t StealStack;

typedef SHARED StealStack * SharedStealStackPtr;


/***********************************************************
 *  Global shared state                                    *
 ***********************************************************/

// shared access to each thread's stealStack
SHARED SharedStealStackPtr stealStack[MAX_THREADS];

// termination detection 
VOLATILE SHARED int cb_cancel;
VOLATILE SHARED int cb_count;
VOLATILE SHARED int cb_done;
#ifdef GLT
LOCK_T cb_lock;
#else
    #ifdef ARGOBOTS
    LOCK_T cb_lock;
#elif defined(QTHREADS)
    LOCK_T cb_lock;
#elif defined(MASSIVETHREADS)
    LOCK_T cb_lock;
    #else
    LOCK_T *cb_lock;
    #endif
#endif

SHARED double startTime[MAX_THREADS];


/***********************************************************
 *  UTS Implementation Hooks                               *
 ***********************************************************/

// Return a string describing this implementation
char * impl_getName() {
  char * name[] = {"Sequential C", "C/OpenMP", "UPC", "SHMEM", "PThreads"};
  return name[COMPILER_TYPE];
}


// construct string with all parameter settings 
int impl_paramsToStr(char *strBuf, int ind) {
  ind += sprintf(strBuf+ind, "Execution strategy:  ");
  if (PARALLEL) {
    ind += sprintf(strBuf+ind, "Parallel search using %d threads\n", GET_NUM_THREADS);
    if (doSteal) {
      ind += sprintf(strBuf+ind, "   Load balance by work stealing, chunk size = %d nodes\n",chunkSize);
      ind += sprintf(strBuf+ind, "  CBarrier Interval: %d\n", cbint);
      ind += sprintf(strBuf+ind, "   Polling Interval: %d\n", pollint);
    }
    else
      ind += sprintf(strBuf+ind, "   No load balancing.\n");
  }
  else
    ind += sprintf(strBuf+ind, "Iterative sequential search\n");
      
  return ind;
}


int impl_parseParam(char *param, char *value) {
  int err = 0;  // Return 0 on a match, nonzero on an error

  switch (param[1]) {
#if (PARALLEL == 1)
    case 'c':
      chunkSize = atoi(value); break;
    case 's':
      doSteal = atoi(value); 
      if (doSteal != 1 && doSteal != 0) 
	err = 1;
      break;
    case 'i':
      cbint = atoi(value); break;
#ifdef __BERKELEY_UPC__
    case 'I':
      pollint = atoi(value); break;
#endif
#ifdef __PTHREADS__
    case 'T':
      pthread_num_threads = atoi(value);
      if (pthread_num_threads > MAX_THREADS) {
        printf("Warning: Requested threads > MAX_THREADS.  Truncated to %d threads\n", MAX_THREADS);
        pthread_num_threads = MAX_THREADS;
      }
      break;
#endif
#else /* !PARALLEL */
#ifdef UTS_STAT
    case 'u':
      unbType = atoi(value); 
      if (unbType > 2) {
        err = 1;
        break;
      }
      if (unbType < 0)
        stats = 0;
      else
        stats = 1;
      break;
#endif
#endif /* PARALLEL */
    default:
      err = 1;
      break;
  }

  return err;
}

void impl_helpMessage() {
  if (PARALLEL) {
    printf("   -s  int   zero/nonzero to disable/enable work stealing\n");
    printf("   -c  int   chunksize for work stealing\n");
    printf("   -i  int   set cancellable barrier polling interval\n");
#ifdef __BERKELEY_UPC__
    printf("   -I  int   set working bupc_poll() interval\n");
#endif
#ifdef __PTHREADS__
    printf("   -T  int   set number of threads\n");
#endif
  } else {
#ifdef UTS_STAT
    printf("   -u  int   unbalance measure (-1: none; 0: min/size; 1: min/n; 2: max/n)\n");
#else
    printf("   none.\n");
#endif
  }
}

void impl_abort(int err) {
#if defined(__UPC__)
  upc_global_exit(err);
#elif defined(_OPENMP)
  exit(err);
#elif defined(_SHMEM)
  exit(err);
#else
  exit(err);
#endif
}


/***********************************************************
 *                                                         *
 *  FUNCTIONS                                              *
 *                                                         *
 ***********************************************************/

/* 
 * StealStack
 *    Stack of nodes with sharing at the bottom of the stack
 *    and exclusive access at the top for the "owning" thread 
 *    which has affinity to the stack's address space.
 *
 *    * All operations on the shared portion of the stack
 *      must be guarded using the stack-specific lock.
 *    * Elements move between the shared and exclusive
 *      portion of the stack solely under control of the 
 *      owning thread. (ss_release and ss_acquire)
 *    * workAvail is the count of elements in the shared
 *      portion of the stack.  It may be read without 
 *      acquiring the stack lock, but of course its value
 *      may not be acurate.  Idle threads read workAvail in
 *      this speculative fashion to minimize overhead to 
 *      working threads.
 *    * Elements can be stolen from the bottom of the shared
 *      portion by non-owning threads.  The values are 
 *      reserved under lock by the stealing thread, and then 
 *      copied without use of the lock (currently space for
 *      reserved values is never reclaimed).
 *
 */

/* restore stack to empty state */
void ss_mkEmpty(StealStack *s) {
    
#ifdef ARGOBOTS
  ABT_mutex_lock(s->stackLock);
#else
  SET_LOCK(s->stackLock);
#endif
  s->sharedStart = 0;
  s->local  = 0;
  s->top    = 0;
  s->workAvail = 0;
#ifdef ARGOBOTS
  ABT_mutex_unlock(s->stackLock);
#else
  UNSET_LOCK(s->stackLock);
#endif
}

/* fatal error */
void ss_error(char *str) {
  printf("*** [Thread %i] %s\n",GET_THREAD_NUM, str);
  exit(4);
}

/* initialize the stack */
void ss_init(StealStack *s, int nelts) {
  int nbytes = nelts * sizeof(Node);

  if (debug & 1)
    printf("Thread %d intializing stealStack %p, sizeof(Node) = %X\n", 
           GET_THREAD_NUM, s, (int)(sizeof(Node)));

  // allocate stack in shared addr space with affinity to calling thread
  // and record local addr for efficient access in sequel
  s->stack_g = (SHARED_INDEF Node *) ALLOC (nbytes);
  s->stack = (Node *) s->stack_g;
#ifdef TRACE
  s->md = (MetaData *) ALLOC (sizeof(MetaData));
  if (s->md == NULL)
    ss_error("ss_init: out of memory");
#endif
  if (s->stack == NULL) {
    printf("Request for %d bytes for stealStack on thread %d failed\n",
           nbytes, GET_THREAD_NUM);
    ss_error("ss_init: unable to allocate space for stealstack");
  }
  //printf("Antes init lock\n");
#ifdef ARGOBOTS
    ABT_mutex_create(&s->stackLock);
    
#else
  INIT_LOCK(s->stackLock);
#endif
  //printf("Despues init lock\n");
  if (debug & 1)
    printf("Thread %d init stackLock %p\n", GET_THREAD_NUM, (void *) s->stackLock);
  s->stackSize = nelts;
  s->nNodes = 0;
  s->maxStackDepth = 0;
  s->maxTreeDepth = 0;
  s->nLeaves = 0;
  s->nAcquire = 0;
  s->nRelease = 0;
  s->nSteal = 0;
  s->nFail = 0;
  s->wakeups = 0;
  s->falseWakeups = 0;
  s->nNodes_last = 0;
    //printf("Antes empty\n");

  ss_mkEmpty(s);
      //printf("Despues empty\n");

}


/* local push */
void ss_push(StealStack *s, Node *c) {
  if (s->top >= s->stackSize)
    ss_error("ss_push: overflow");
  if (debug & 1)
    printf("ss_push: Thread %d, posn %d: node %s [%d]\n",
           GET_THREAD_NUM, s->top, rng_showstate(c->state.state, debug_str), c->height);
  memcpy(&(s->stack[s->top]), c, sizeof(Node));
  s->top++;
  s->nNodes++;
  s->maxStackDepth = max(s->top, s->maxStackDepth);
  s->maxTreeDepth = max(s->maxTreeDepth, c->height);
}

/* local top:  get local addr of node at top */ 
Node * ss_top(StealStack *s) {
  Node *r;
  if (s->top <= s->local)
    ss_error("ss_top: empty local stack");
  r = &(s->stack[(s->top) - 1]);
  if (debug & 1) 
    printf("ss_top: Thread %d, posn %d: node %s [%d] nchild = %d\n",
           GET_THREAD_NUM, s->top - 1, rng_showstate(r->state.state, debug_str),
           r->height, r->numChildren);
  return r;
}

/* local pop */
void ss_pop(StealStack *s) {
  Node *r;
  if (s->top <= s->local)
    ss_error("ss_pop: empty local stack");
  s->top--;
  r = &(s->stack[s->top]);
  if (debug & 1)
    printf("ss_pop: Thread %d, posn %d: node %s [%d] nchild = %d\n",
           GET_THREAD_NUM, s->top, rng_showstate(r->state.state, debug_str), 
           r->height, r->numChildren);
}
  
/* local top position:  stack index of top element */
int ss_topPosn(StealStack *s)
{
  if (s->top <= s->local)
    ss_error("ss_topPosn: empty local stack");
  return s->top - 1;
}

/* local depth */
int ss_localDepth(StealStack *s) {
  return (s->top - s->local);
}

/* release k values from bottom of local stack */
void ss_release(StealStack *s, int k) {
#ifdef ARGOBOTS
 ABT_mutex_lock(s->stackLock);

#else
  SET_LOCK(s->stackLock);
#endif
  if (s->top - s->local >= k) {
    s->local += k;
    s->workAvail += k;
    s->nRelease++;
  }
  else
    ss_error("ss_release:  do not have k vals to release");
#ifdef ARGOBOTS
  ABT_mutex_unlock(s->stackLock);

#else
  UNSET_LOCK(s->stackLock);
#endif
}

/* move k values from top of shared stack into local stack
 * return false if k vals are not avail on shared stack
 */
int ss_acquire(StealStack *s, int k) {
  int avail;
#ifdef ARGOBOTS
  ABT_mutex_lock(s->stackLock);  
#else
  SET_LOCK(s->stackLock);
#endif
  avail = s->local - s->sharedStart;
  if (avail >= k) {
    s->local -= k;
    s->workAvail -= k;
    s->nAcquire++;
  }
#ifdef ARGOBOTS
  ABT_mutex_unlock(s->stackLock);  
#else
  UNSET_LOCK(s->stackLock);
#endif
  return (avail >= k);
}

/* steal k values from shared portion of victim thread's stealStack
 * onto local portion of current thread's stealStack.
 * return false if k vals are not avail in victim thread
 */
int ss_steal(StealStack *s, int victim, int k) {
  int victimLocal, victimShared, victimWorkAvail;
  int ok;
  
  if (s->sharedStart != s->top)
    ss_error("ss_steal: thief attempts to steal onto non-empty stack");

  if (s->top + k >= s->stackSize)
    ss_error("ss_steal: steal will overflow thief's stack");
  
  /* lock victim stack and try to reserve k elts */
  if (debug & 32)
    printf("Thread %d wants    SS %d\n", GET_THREAD_NUM, victim);
  
  
#ifdef ARGOBOTS
  ABT_mutex_lock(stealStack[victim]->stackLock);  
#else
SET_LOCK(stealStack[victim]->stackLock);
#endif
#ifdef _SHMEM
  /* Get remote steal stack */
  SMEMCPY(stealStack[victim], stealStack[victim], sizeof(StealStack), victim);
#endif

  if (debug & 32)
    printf("Thread %d acquires SS %d\n", GET_THREAD_NUM, victim);
  
  victimLocal = stealStack[victim]->local;
  victimShared = stealStack[victim]->sharedStart;
  victimWorkAvail = stealStack[victim]->workAvail;
  
  if (victimLocal - victimShared != victimWorkAvail)
    ss_error("ss_steal: stealStack invariant violated");
  
  ok = victimWorkAvail >= k;
  if (ok) {
    /* reserve a chunk */
    stealStack[victim]->sharedStart =  victimShared + k;
    stealStack[victim]->workAvail = victimWorkAvail - k;

#ifdef _SHMEM
    // FIXME: These transfers ought to be combined.  They can't be
    // though because the data protected by the stacklock is not
    // the only data in the StealStack structure.
    PUT(stealStack[victim]->sharedStart, stealStack[victim]->sharedStart, victim);
    PUT(stealStack[victim]->workAvail, stealStack[victim]->workAvail, victim);
#endif
  }
  #ifdef ARGOBOTS
  ABT_mutex_unlock(stealStack[victim]->stackLock);  
#else
  UNSET_LOCK(stealStack[victim]->stackLock);
#endif
  //UNSET_LOCK(stealStack[victim]->stackLock);
  
  if (debug & 32)
    printf("Thread %d releases SS %d\n", GET_THREAD_NUM, victim);
	
  /* if k elts reserved, move them to local portion of our stack */
  if (ok) {
    SHARED_INDEF Node * victimStackBase = stealStack[victim]->stack_g;
    SHARED_INDEF Node * victimSharedStart = victimStackBase + victimShared;

#ifdef _SHMEM
    SMEMCPY(&(s->stack[s->top]), victimSharedStart, k * sizeof(Node), victim);
#else
    SMEMCPY(&(s->stack[s->top]), victimSharedStart, k * sizeof(Node));
#endif

    s->nSteal++;
    if (debug & 4) {
      int i;
      for (i = 0; i < k; i ++) {
        Node * r = &(s->stack[s->top + i]);
        printf("ss_steal:  Thread %2d posn %d (steal #%d) receives %s [%d] from thread %d posn %d (%p)\n",
               GET_THREAD_NUM, s->top + i, s->nSteal,
               rng_showstate(r->state.state, debug_str),
               r->height, victim, victimShared + i, 
               (void *) victimSharedStart);
      }
    }
    s->top += k;

   #ifdef TRACE
      /* update session record of theif */
      s->md->stealRecords[s->entries[SS_WORK]].victimThread = victim;
   #endif
  }
  else {
    s->nFail++;
    if (debug & 4) {
      printf("Thread %d failed to steal %d nodes from thread %d, ActAv = %d, sh = %d, loc =%d\n",
	     GET_THREAD_NUM, k, victim, victimWorkAvail, victimShared, victimLocal);
    }
  }
  return (ok);
} 

/* search other threads for work to steal */
int findwork(int k) {
    int id;
#ifdef GLT
    glt_workunit_get_thread_id(&id);
#else 
    id=GET_THREAD_NUM;
#endif
    //printf("En find work %d/%d\n",id,GET_NUM_THREADS);
  int i,v;
  for (i = 1; i < GET_NUM_THREADS; i++) {
    v = (id + i) % GET_NUM_THREADS;
#ifdef _SHMEM
    GET(stealStack[v]->workAvail, stealStack[v]->workAvail, v);
#endif
    if (stealStack[v]->workAvail >= k)
      return v;
  }
  return -1;
}


/**
 *  Tracing functions
 *   Track changes in the search state for offline analysis.
**/
void ss_initState(StealStack *s) {
  int i;
  s->timeLast = uts_wctime();
  for (i = 0; i < SS_NSTATES; i++) {
    s->time[i] = 0.0;
    s->entries[i] = 0;
  }
  s->curState = SS_IDLE;
  if (debug & 8)
    printf("Thread %d start state %d (t = %f)\n", 
           GET_THREAD_NUM, s->curState, s->timeLast);
}

void ss_setState(StealStack *s, int state){
  double time;
  if (state < 0 || state >= SS_NSTATES)
    ss_error("ss_setState: thread state out of range");
  if (state == s->curState)
    return;
  time = uts_wctime();
  s->time[s->curState] +=  time - s->timeLast;

  #ifdef TRACE  
    /* close out last session record */
    s->md->sessionRecords[s->curState][s->entries[s->curState] - 1].endTime = time;
    if (s->curState == SS_WORK)
    {
       s->md->stealRecords[s->entries[SS_WORK] - 1].nodeCount = s->nNodes
           - s->md->stealRecords[s->entries[SS_WORK] - 1].nodeCount;
    }

    /* initialize new session record */
    s->md->sessionRecords[state][s->entries[state]].startTime = time;
    if (state == SS_WORK)
    {
       s->md->stealRecords[s->entries[SS_WORK]].nodeCount = s->nNodes;
    }
  #endif

  s->entries[state]++;
  s->timeLast = time;
  s->curState = state;

  if(debug & 8)
    printf("Thread %d enter state %d [#%d] (t = %f)\n",
           GET_THREAD_NUM, state, s->entries[state], time);
}

#ifdef UTS_STAT
/*
 * Statistics, 
 * : number of nodes per level
 * : imbalanceness of nodes per level
 *
 */
void initHist()
{
  int i;
  for (i=0; i<MAXHISTSIZE; i++){
    hist[i][0]=0;
    hist[i][1]=0;
    unbhist[i][1]=1;
    unbhist[i][2]=0;
  }
}

void updateHist(Node* c, double unb)
{
  if (c->height<MAXHISTSIZE){
    hist[c->height][1]++;
    hist[c->height][0]+=c->numChildren;

    unbhist[c->height][0]+=unb;
    if (unbhist[c->height][1]>unb)
      unbhist[c->height][1]=unb;
    if (unbhist[c->height][2]<unb)
      unbhist[c->height][2]=unb;
		
  }
  else {
    hist[MAXHISTSIZE][1]++;
    hist[MAXHISTSIZE][0]+=c->numChildren;
  }
}

void showHist(FILE *fp)
{
  int i;	
  fprintf(fp, "depth\tavgNumChildren\t\tnumChildren\t imb\t maxImb\t minImb\t\n");
  for (i=0; i<MAXHISTSIZE; i++){
    if ((hist[i][0]!=0)&&(hist[i][1]!=0))
      fprintf(fp, "%d\t%f\t%d\t %lf\t%lf\t%lf\n", i, (double)hist[i][0]/hist[i][1], 
              hist[i][0], unbhist[i][0]/hist[i][1], unbhist[i][1], unbhist[i][2]);	
  }
}

double getImb(Node *c)
{
  int i=0;
  double avg=.0, tmp=.0;
  double unb=0.0;
  
  avg=(double)c->sizeChildren/c->numChildren;

  for (i=0; i<c->numChildren; i++){		
    if ((type==BIN)&&(c->pp==NULL))
      {
        if (unbType<2)
          tmp=min((double)rootSize[i]/avg, avg/(double)rootSize[i]);
        else 
          tmp=max((double)rootSize[i]/avg, avg/(double)rootSize[i]);
        
        if (unbType>0)
          unb+=tmp*rootUnb[i];
        else 
          unb+=tmp*rootUnb[i]*rootSize[i];
      }	
    else{
      if (unbType<2)
        tmp=min((double)c->size[i]/avg, avg/(double)c->size[i]);
      else 
        tmp=max((double)c->size[i]/avg, avg/(double)c->size[i]);
      
      if (unbType>0)
        unb+=tmp*c->unb[i];
      else 
        unb+=tmp*c->unb[i]*c->size[i];
    }
  }
	
  if (unbType>0){
    if (c->numChildren>0) 
      unb=unb/c->numChildren;
    else unb=1.0;
  }
  else {
    if (c->sizeChildren>1) 
      unb=unb/c->sizeChildren;
    else unb=1.0;
  }
  if ((debug & 1) && unb>1) printf("unb>1%lf\t%d\n", unb, c->numChildren);
	
  return unb;
}

void getImb_Tseng(Node *c)
{
  double t_max, t_avg, t_devmaxavg, t_normdevmaxavg;

  if (c->numChildren==0)
    {
      t_avg =0;
      t_max =0;
    }
  else 
    {
      t_max = (double)c->maxSizeChildren/(c->sizeChildren-1);
      t_avg = (double)1/c->numChildren;
    }

  t_devmaxavg = t_max-t_avg;
	
  if (debug & 1)
    printf("max\t%lf, %lf, %d, %d, %d\n", t_max, t_avg, 
           c->maxSizeChildren, c->sizeChildren, c->numChildren);
	
  if (1-t_avg==0)
    t_normdevmaxavg = 1;
  else
    t_normdevmaxavg = (t_max-t_avg)/(1-t_avg);

  imb_max += t_max;
  imb_avg += t_avg;
  imb_devmaxavg += t_devmaxavg;
  imb_normdevmaxavg +=t_normdevmaxavg;
}

void updateParStat(Node *c)
{
  double unb;

  totalNodes++;
  if (maxHeight<c->height) 
    maxHeight=c->height;
	
  unb=getImb(c);
  maxImb=max(unb, maxImb);
  minImb=min(unb, minImb);
  updateHist(c, unb);
  
  getImb_Tseng(c);
	
  if (c->pp!=NULL){
    if ((c->type==BIN)&&(c->pp->pp==NULL)){
      rootSize[c->pp->ind]=c->sizeChildren;
      rootUnb[c->pp->ind]=unb;
    }
    else{
      c->pp->size[c->pp->ind]=c->sizeChildren;
      c->pp->unb[c->pp->ind]=unb;
    }
    /* update statistics per node*/
    c->pp->ind++;
    c->pp->sizeChildren+=c->sizeChildren;
    if (c->pp->maxSizeChildren<c->sizeChildren)
      c->pp->maxSizeChildren=c->sizeChildren;		
  }
  else 
    treeImb = unb;
}
#endif

/*
 *	Tree Implementation      
 *
 */
void initNode(Node * child)
{
  child->type = -1;
  child->height = -1;
  child->numChildren = -1;    // not yet determined

#ifdef UTS_STAT
  if (stats){	
    int i;
    child->ind = 0;
    child->sizeChildren = 1;
    child->maxSizeChildren = 0;
    child->pp = NULL;
    for (i = 0; i < MAXNUMCHILDREN; i++){
      child->size[i] = 0;
      child->unb[i]  = 0.0;
    }
  }
#endif
}


void initRootNode(Node * root, int type)
{
  uts_initRoot(root, type);

  #ifdef TRACE
    stealStack[0]->md->stealRecords[0].victimThread = 0;  // first session is own "parent session"
  #endif

#ifdef UTS_STAT
  if (stats){
    int i;
    root->ind = 0;
    root->sizeChildren = 1;
    root->maxSizeChildren = 1;
    root->pp = NULL;
    
    if (type != BIN){
      for (i=0; i<MAXNUMCHILDREN; i++){
        root->size[i] = 0;
        root->unb[i]  =.0; 
      }
    }
    else {
      int rbf = (int) ceil(b_0);
      rootSize = malloc(rbf*sizeof(int));
      rootUnb = malloc(rbf*sizeof(double));
      for (i = 0; i < rbf; i++) {
        rootSize[i] = 0;
        rootUnb[i] = 0.0;
      }
    }
  }
#endif
}


// forward decl
void releaseNodes(StealStack *ss);

/* 
 * Generate all children of the parent
 *
 * details depend on tree type, node type and shape function
 *
 */
void genChildren(Node * parent, Node * child, StealStack * ss) {
  int parentHeight = parent->height;
  int numChildren, childType;

  numChildren = uts_numChildren(parent);
  childType   = uts_childType(parent);

  // record number of children in parent
  parent->numChildren = numChildren;
  if (debug & 2) {
    printf("Gen:  Thread %d, posn %2d: node %s [%d] has %2d children\n",
           GET_THREAD_NUM, ss_topPosn(ss),
           rng_showstate(parent->state.state, debug_str), 
           parentHeight, numChildren);
  }
  
  // construct children and push onto stack
  if (numChildren > 0) {
    int i, j;
    child->type = childType;
    child->height = parentHeight + 1;

#ifdef UTS_STAT
    if (stats)
      child->pp = parent;  // pointer to parent
#endif

    for (i = 0; i < numChildren; i++) {
      for (j = 0; j < computeGranularity; j++) {
        // TBD:  add parent height to spawn
        // computeGranularity controls number of rng_spawn calls per node
        rng_spawn(parent->state.state, child->state.state, i);
      }
      ss_push(ss, child);
      releaseNodes(ss);
    }
  } else {
    ss->nLeaves++;
  }
}


    
/*
 *  Parallel tree traversal
 *
 */

// cancellable barrier

// initialize lock:  single thread under omp, all threads under upc
void cb_init(){
      //printf("EN CB_INIT ANTES DEL SINGLE LOCK\n");

#ifdef ARGOBOTS
    ABT_mutex_create(&cb_lock);
#else
  INIT_SINGLE_LOCK(cb_lock);
#endif
  if (debug & 4)
    printf("Thread %d, cb lock at %p\n", GET_THREAD_NUM, (void *) cb_lock);
  //printf("EN CB_INIT DESPUES DEL SINGLE LOCK\n");
  // fixme: no need for all upc threads to repeat this
#ifdef ARGOBOTS
  ABT_mutex_lock(cb_lock);
#else
  SET_LOCK(cb_lock);
#endif
    //printf("EN CB_INIT DESPUES DEL LOCK\n");

  cb_count = 0;
  cb_cancel = 0;
  cb_done = 0;
#ifdef ARGOBOTS
  ABT_mutex_unlock(cb_lock);
#else
  UNSET_LOCK(cb_lock);
#endif
    //printf("EN CB_INIT DESPUES DEL UNLOCK\n");

}

//  delay this thread until all threads arrive at barrier
//     or until barrier is cancelled
int cbarrier_wait() {
  int l_count, l_done, l_cancel;
  int id;
#ifdef GLT
    glt_workunit_get_thread_id(&id);
#else 
    id=GET_THREAD_NUM;
#endif
  int pe = id;

  #ifdef ARGOBOTS
  ABT_mutex_lock(cb_lock);
#else
  SET_LOCK(cb_lock);
#endif
  cb_count++;
#ifdef _SHMEM
  PUT_ALL(cb_count, cb_count);
#endif
  if (cb_count == GET_NUM_THREADS) {
    cb_done = 1;
#ifdef _SHMEM
    PUT_ALL(cb_done, cb_done);
#endif
  }
  l_count = cb_count;
  l_done = cb_done;
  if (stealStack[pe]->nNodes_last == stealStack[pe]->nNodes) {
    ++stealStack[pe]->falseWakeups;
  }
  stealStack[id]->nNodes_last = stealStack[pe]->nNodes;
#ifdef ARGOBOTS
  ABT_mutex_unlock(cb_lock);
#else
  UNSET_LOCK(cb_lock);
#endif

  if (debug & 16)
    printf("Thread %d enter spin-wait, count = %d, done = %d\n",
           GET_THREAD_NUM, l_count, l_done);

  // spin
  do {
#ifdef __BERKELEY_UPC__
    bupc_poll();
#endif
    l_count = cb_count;
    l_cancel = cb_cancel;
    l_done = cb_done;
  }
  while (!l_cancel && !l_done);

  if (debug & 16)
    printf("Thread %d exit  spin-wait, count = %d, done = %d, cancel = %d\n",
           GET_THREAD_NUM, l_count, l_done, l_cancel);


  #ifdef ARGOBOTS
  ABT_mutex_lock(cb_lock);
#else
  SET_LOCK(cb_lock);
#endif
  
  cb_count--;
  l_count = cb_count;
#ifdef _SHMEM
  PUT_ALL(cb_count, cb_count);
#endif
  cb_cancel = 0;
  l_done = cb_done;
  ++stealStack[id]->wakeups;
#ifdef ARGOBOTS
  ABT_mutex_unlock(cb_lock);
#else
  UNSET_LOCK(cb_lock);
#endif

  if (debug & 16)
    printf("Thread %d exit idle state, count = %d, done = %d\n",
           GET_THREAD_NUM, l_count, cb_done);

  return cb_done;
}

// causes one or more threads waiting at barrier, if any,
//  to be released
void cbarrier_cancel() {
#ifdef _SHMEM
  cb_cancel = 1;
  PUT_ALL(cb_cancel, cb_cancel);
#elif defined (__BERKELEY_UPC__)
  bupc_waitsync(cb_handle);
  cb_handle = bupc_memput_async((shared void*)&cb_cancel, (const void*)&local_cb_cancel, sizeof(int));
#else
  cb_cancel = 1;
#endif /* _SHMEM */
}

void releaseNodes(StealStack *ss){
  if (doSteal) {
    if (ss_localDepth(ss) > 2 * chunkSize) {
      // Attribute this time to runtime overhead
      ss_setState(ss, SS_OVH);
      ss_release(ss, chunkSize);
      // This has significant overhead on clusters!
      if (ss->nNodes % cbint == 0) {
        ss_setState(ss, SS_CBOVH);
        cbarrier_cancel();
      }

#ifdef __BERKELEY_UPC__
      if (ss->nNodes % pollint == 0) {
        ss_setState(ss, SS_OVH);
        bupc_poll();
      }
#endif
      ss_setState(ss, SS_WORK);
    }
  }
}

/* 
 * parallel search of UTS trees using work stealing 
 * 
 *   Note: tree size is measured by the number of
 *         push operations
 */
void parTreeSearch(StealStack *ss) {
  int done = 0;
  Node * parent;
  Node child;
  int id;
#ifdef GLT
  glt_workunit_get_thread_id(&id);
#else
  id=GET_THREAD_NUM;
#endif
  
  /* template for children */
  initNode(&child);

  /* tree search */
  while (done == 0) {
    
    /* local work */
    while (ss_localDepth(ss) > 0) {		
        //printf("%d tengo trabajo\n",id);
      ss_setState(ss, SS_WORK);

      /* examine node at stack top */
      parent = ss_top(ss);
      if (parent->numChildren < 0){
	// first time visited, construct children and place on stack
	genChildren(parent,&child,ss);

      }
      else {
	// second time visit, process accumulated statistics and pop
#ifdef UTS_STAT
        if (stats)
          updateParStat(parent);
#endif
        ss_pop(ss);
      }
      
      // release some nodes for stealing, if enough are available
      // and wake up quiescent threads
      releaseNodes(ss);
    }
	        //printf("%d ya no tengo trabajo\n",id);
	
    /* local work exhausted on this stack - resume tree search if able
     * to re-acquire work from shared portion of this thread's stack
     */
    if (ss_acquire(ss, chunkSize))
      continue;

    /* no work left in this thread's stack           */
    /* try to steal work from another thread's stack */
    if (doSteal) {
      int goodSteal = 0;
      int victimId;
      
      ss_setState(ss, SS_SEARCH);
      victimId = findwork(chunkSize);
      while (victimId != -1 && !goodSteal) {
	// some work detected, try to steal it
	goodSteal = ss_steal(ss, victimId, chunkSize);
	if (!goodSteal)
	  victimId = findwork(chunkSize);
      }
      if (goodSteal)
	  continue;
    }
	
    /* unable to steal work from shared portion of other stacks -
     * enter quiescent state waiting for termination (done != 0)
     * or cancellation because some thread has made work available
     * (done == 0).
     */
    ss_setState(ss, SS_IDLE);
#ifdef GLT
    //glt_barrier_wait(&barrier);
    done = 1;//cbarrier_wait();
#else
#ifdef ARGOBOTS
    done = 1;
    #elif defined(QTHREADS)
        done = 1;
#elif defined(MASSIVETHREADS)
        done = 1;

#else
    cbarrier_wait();
#endif
#endif
  }
  //printf("%d/%d Fin total!!\n",id/GET_NUM_THREADS);
  /* tree search complete ! */
}

#ifdef __PTHREADS__
/* Pthreads ParTreeSearch Arguments */
struct pthread_args {
	StealStack *ss;
	int         id;
};

/* Pthreads ParTreeSearch Wrapper */
void * pthread_spawn_search(void *arg)
{
	pthread_setspecific(pthread_thread_num, &((struct pthread_args*)arg)->id);
	parTreeSearch(((struct pthread_args*)arg)->ss);
	return NULL;
}
#endif /* __PTHREADS__ */


#ifdef TRACE
// print session records for each thread (used when trace is enabled)
void printSessionRecords()
{
  int i, j, k;
  double offset;

  for (i = 0; i < GET_NUM_THREADS; i++) {
    offset = startTime[i] - startTime[0];

    for (j = 0; j < SS_NSTATES; j++)
       for (k = 0; k < stealStack[i]->entries[j]; k++) {
          printf ("%d %d %f %f", i, j,
            stealStack[i]->md->sessionRecords[j][k].startTime - offset,
            stealStack[i]->md->sessionRecords[j][k].endTime - offset);
          if (j == SS_WORK)
            printf (" %d %ld",
              stealStack[i]->md->stealRecords[k].victimThread,
              stealStack[i]->md->stealRecords[k].nodeCount);
            printf ("\n");
     }
  }
}
#endif

// display search statistics
void showStats(double elapsedSecs) {
  int i;
  int tnodes = 0, tleaves = 0, trel = 0, tacq = 0, tsteal = 0, tfail= 0;
  int mdepth = 0, mheight = 0;
  double twork = 0.0, tsearch = 0.0, tidle = 0.0, tovh = 0.0, tcbovh = 0.0;

#ifdef _SHMEM
  {
    int pe;
    /* Assemble all of the stealstacks so we can gather some stats. */
    for (i = 1; i < GET_NUM_THREADS; i++) {
      pe = (GET_THREAD_NUM + i) % GET_NUM_THREADS;
      /* Collect up all of the StealStacks */
      SMEMCPY(stealStack[pe], stealStack[pe], sizeof(StealStack), pe);
#ifdef TRACE
      /* Get the MetaData as well */
      SMEMCPY(stealStack[pe]->md, stealStack[pe]->md, sizeof(StealStack), pe);
#endif
    }
  }
#endif

  // combine measurements from all threads
  for (i = 0; i < GET_NUM_THREADS; i++) {
    tnodes  += stealStack[i]->nNodes;
    //printf("In show stats id=%d, nodes=%d\n",i,stealStack[i]->nNodes);
    tleaves += stealStack[i]->nLeaves;
    trel    += stealStack[i]->nRelease;
    tacq    += stealStack[i]->nAcquire;
    tsteal  += stealStack[i]->nSteal;
    tfail   += stealStack[i]->nFail;
    twork   += stealStack[i]->time[SS_WORK];
    tsearch += stealStack[i]->time[SS_SEARCH];
    tidle   += stealStack[i]->time[SS_IDLE];
    tovh    += stealStack[i]->time[SS_OVH];
    tcbovh  += stealStack[i]->time[SS_CBOVH];
    mdepth   = max(mdepth, stealStack[i]->maxStackDepth);
    mheight  = max(mheight, stealStack[i]->maxTreeDepth);
  }
  if (trel != tacq + tsteal) {
    printf("*** error! total released (%d) != total acquired (%d)+ total stolen(%d) = %d\n",trel,tacq,tsteal,tacq+tsteal);
  }
    
  uts_showStats(GET_NUM_THREADS, chunkSize, elapsedSecs, tnodes, tleaves, mheight);

  if (verbose > 1) {
    if (doSteal) {
      printf("Total chunks released = %d, of which %d reacquired and %d stolen\n",
          trel, tacq, tsteal);
      printf("Failed steal operations = %d, ", tfail);
    }

    printf("Max stealStack size = %d\n", mdepth);
    printf("Avg time per thread: Work = %.6f, Search = %.6f, Idle = %.6f\n", (twork / GET_NUM_THREADS),
        (tsearch / GET_NUM_THREADS), (tidle / GET_NUM_THREADS));
    printf("                     Overhead = %6f, CB_Overhead = %6f\n\n", (tovh / GET_NUM_THREADS),
        (tcbovh/GET_NUM_THREADS));
  }

  // per thread execution info
  if (verbose > 2) {
    for (i = 0; i < GET_NUM_THREADS; i++) {
      printf("** Thread %d\n", i);
      printf("  # nodes explored    = %d\n", stealStack[i]->nNodes);
      printf("  # chunks released   = %d\n", stealStack[i]->nRelease);
      printf("  # chunks reacquired = %d\n", stealStack[i]->nAcquire);
      printf("  # chunks stolen     = %d\n", stealStack[i]->nSteal);
      printf("  # failed steals     = %d\n", stealStack[i]->nFail);
      printf("  maximum stack depth = %d\n", stealStack[i]->maxStackDepth);
      printf("  work time           = %.6f secs (%d sessions)\n",
             stealStack[i]->time[SS_WORK], stealStack[i]->entries[SS_WORK]);
      printf("  overhead time       = %.6f secs (%d sessions)\n",
             stealStack[i]->time[SS_OVH], stealStack[i]->entries[SS_OVH]);
      printf("  search time         = %.6f secs (%d sessions)\n",
             stealStack[i]->time[SS_SEARCH], stealStack[i]->entries[SS_SEARCH]);
      printf("  idle time           = %.6f secs (%d sessions)\n",
             stealStack[i]->time[SS_IDLE], stealStack[i]->entries[SS_IDLE]);
      printf("  wakeups             = %d, false wakeups = %d (%.2f%%)",
             stealStack[i]->wakeups, stealStack[i]->falseWakeups,
             (stealStack[i]->wakeups == 0) ? 0.00 : ((((double)stealStack[i]->falseWakeups)/stealStack[i]->wakeups)*100.0));
      printf("\n");
    }
  }

  #ifdef TRACE
    printSessionRecords();
  #endif

  // tree statistics output to stat.txt, if requested
#ifdef UTS_STAT
  if (stats) {
    FILE *fp;
    char * tmpstr;
    char strBuf[5000];
    int  ind = 0;
    
    fp = fopen("stat.txt", "a+w");
    fprintf(fp, "\n------------------------------------------------------------------------------------------------------\n");
    ind = uts_paramsToStr(strBuf, ind);
    ind = impl_paramsToStr(strBuf, ind);
    //showParametersStr(strBuf);
    fprintf(fp, "%s\n", strBuf);
    
    fprintf(fp, "\nTotal nodes = %d\n", totalNodes); 
    fprintf(fp, "Max depth   = %d\n\n", maxHeight); 
    fprintf(fp, "Tseng ImbMeasure(overall)\n max:\t\t%lf \n avg:\t\t%lf \n devMaxAvg:\t %lf\n normDevMaxAvg: %lf\t\t\n\n", 
            imb_max/totalNodes, imb_avg/totalNodes, imb_devmaxavg/totalNodes, 
            imb_normdevmaxavg/totalNodes);
    
    switch (unbType){
    case 0: tmpstr = "(min imb weighted by size)"; break;
    case 1: tmpstr = "(min imb not weighted by size)"; break;
    case 2: tmpstr = "(max imb not weighted by size)"; break;
    default: tmpstr = "(?unknown measure)"; break;
    }
    fprintf(fp, "ImbMeasure:\t%s\n Overall:\t %lf\n Max:\t\t%lf\n Min:\t\t%lf\n\n", 
            tmpstr, treeImb, minImb, maxImb);
    showHist(fp);
    fprintf(fp, "\n------------------------------------------------------------------------------------------------------\n\n\n");
    fclose(fp);
  }
#endif
}


/* PThreads main() function:
 *   Pthreads is quite a bit different because of how global data has to be stored
 *   using setspecific() and getspecific().  So, many functions are not safe to call
 *   in the single-threaded context.
 */
#ifdef __PTHREADS__
int pthread_main(int argc, char *argv[]) {
  Node   root;
  double t1, t2;
  int    i, err;
  void  *rval;
  struct pthread_args *args;
  pthread_t *thread_ids;

  uts_parseParams(argc, argv);
  uts_printParams();
  cb_init();

  /* allocate stealstacks */
  for (i = 0; i < GET_NUM_THREADS; i++) {
    stealStack[i] = ALLOC (sizeof(StealStack));
    ss_init(stealStack[i], MAXSTACKDEPTH);
  }

  /* initialize root node and push on thread 0 stack */
  uts_initRoot(&root, type);
  ss_push(stealStack[0], &root);

  thread_ids = malloc(sizeof(pthread_t)*GET_NUM_THREADS);
  args = malloc(sizeof(struct pthread_args)*GET_NUM_THREADS);
  pthread_key_create(&pthread_thread_num, NULL);

  /* start timing */
  t1 = uts_wctime();

  for (i = 0; i < GET_NUM_THREADS; i++) {
    ss_initState(stealStack[i]);
    args[i].ss = stealStack[i];
    args[i].id = i;

    err = pthread_create(&thread_ids[i], NULL, pthread_spawn_search, (void*)&args[i]);
    if (err != 0) {
      printf("FATAL: Error spawning thread %d\n", err);
      impl_abort(1);
    }
  }
  for (i = 0; i < GET_NUM_THREADS; i++) {
    pthread_join(thread_ids[i], &rval);
  }

  /* stop timing */
  t2 = uts_wctime();

  showStats(t2-t1);

  return 0;
}
#endif /* __PTHREADS__ */
//#define GLT
#ifdef GLT

struct glt_args {
	StealStack *ss;
	int         id;
};

/* Glt ParTreeSearch Wrapper */
void glt_spawn_search(void *arg)
{
	//pthread_setspecific(pthread_thread_num, &((struct pthread_args*)arg)->id);
        //int id;
        //glt_workunit_get_thread_id(&id);
        //printf("%d/%d Inicio glt_spawn_search!!\n",id/GET_NUM_THREADS);

       // ((struct glt_args*)arg)->id=id;
	parTreeSearch(((struct glt_args*)arg)->ss);
	//return NULL;
          //printf("%d/%d Fin glt_spawn_search!!\n",id/GET_NUM_THREADS);

}

int glt_main(int argc, char *argv[]) {
    
    
  glt_init(argc,argv);
  Node   root;

  double t1, t2, tcreation, tyield, tjoin;
  int    i, err;
  void  *rval;
  struct glt_args *args;
#ifdef TASKLET
    GLT_tasklet *tasklet_ids;
#else
  GLT_ult *ult_ids;
#endif
  //printf("Init\n");
  uts_parseParams(argc, argv);
  uts_printParams();
  //cb_init();
  //glt_barrier_create(GET_NUM_THREADS, &barrier);
//printf("despues barrier\n");
  /* allocate stealstacks */
  for (i = 0; i < GET_NUM_THREADS; i++) {
    stealStack[i] = ALLOC (sizeof(StealStack));
    ss_init(stealStack[i], MAXSTACKDEPTH);
  }
//printf("despues ss\n");

  /* initialize root node and push on thread 0 stack */
  uts_initRoot(&root, type);
  ss_push(stealStack[0], &root);
//printf("despues push\n");
#ifdef TASKLET
    tasklet_ids = glt_tasklet_malloc(GET_NUM_THREADS);//malloc(sizeof(GLT_ult)*GET_NUM_THREADS);
#else
  ult_ids = glt_ult_malloc(GET_NUM_THREADS);//malloc(sizeof(GLT_ult)*GET_NUM_THREADS);
#endif
  args = malloc(sizeof(struct glt_args)*GET_NUM_THREADS);
  //pthread_key_create(&pthread_thread_num, NULL);

  /* start timing */
  t1 = uts_wctime();

  for (i = 0; i < GET_NUM_THREADS; i++) {
    ss_initState(stealStack[i]);
    args[i].ss = stealStack[i];
    args[i].id = i;
    //printf("creando tares %d\n",i);
    //err = pthread_create(&thread_ids[i], NULL, pthread_spawn_search, (void*)&args[i]);
    //void glt_ult_creation_to(void(*thread_func)(void *), void *arg, GLT_ult *new_ult, int dest)
#ifdef TASKLET
    glt_tasklet_creation_to(glt_spawn_search, (void*)&args[i],&tasklet_ids[i],i);
#else
    glt_ult_creation_to(glt_spawn_search, (void*)&args[i],&ult_ids[i],i);
#endif
  }
   tcreation = uts_wctime();
  //printf("Creadas las tareas\n");
  glt_yield();
    tyield = uts_wctime();

  //printf("Ejecutada la mia\n");
  for (i = 0; i < GET_NUM_THREADS; i++) {
    //pthread_join(thread_ids[i], &rval);
          //printf("joining tares %d\n",i);
#ifdef TASKLET
        glt_tasklet_join(&tasklet_ids[i]);
#else
        glt_ult_join(&ult_ids[i]);
#endif

  }
  //printf("join acabado\n");
  tjoin = uts_wctime();

  /* stop timing */
  t2 = uts_wctime();

  showStats(t2-t1);
  printf("GLT breakdown Threads: %d, Total: %f, Create: %f, Yield: %f, Join: %f\n",GET_NUM_THREADS,t2-t1,tcreation-t1,tyield-tcreation,tjoin-tyield);

  glt_finalize();
  return 0;
}
#endif /* GLT */
//#define GLT
#ifdef ARGOBOTS

struct glt_args {
	StealStack *ss;
	int         id;
};

/* Glt ParTreeSearch Wrapper */
void abt_spawn_search(void *arg)
{
	
	parTreeSearch(((struct glt_args*)arg)->ss);

}

int abt_main(int argc, char *argv[]) {
    
    int num_threads =1;
    int num_pools = num_threads;
    ABT_init(argc, argv);

    if (getenv("GLT_NUM_THREADS") != NULL) {
        num_threads = atoi(getenv("GLT_NUM_THREADS"));
    }
    
    if (getenv("GLT_NUM_POOLS") != NULL) {
        num_pools = atoi(getenv("GLT_NUM_POOLS"));
    }
    
    ABT_xstream * team = (ABT_xstream *) malloc(sizeof (ABT_xstream) * num_threads);
    ABT_pool * pools = (ABT_pool *) malloc(sizeof (ABT_pool) * num_pools);
    
    
    for (int i = 0; i < num_pools; i++) {
        ABT_pool_create_basic(ABT_POOL_FIFO, ABT_POOL_ACCESS_MPMC, ABT_TRUE,
            &pools[i]);
    }
    ABT_xstream_self(&team[0]);
    ABT_xstream_set_main_sched_basic(team[0], ABT_SCHED_DEFAULT,
            1, &pools[0]);
    
    for (int i = 1; i < num_threads; i++) {
        ABT_xstream_create_basic(ABT_SCHED_DEFAULT, 1,
                &pools[i % num_pools],
                ABT_SCHED_CONFIG_NULL, &team[i]);
        ABT_xstream_start(team[i]);
    }
  
  
  Node   root;

  double t1, t2, tcreation, tyield, tjoin;
  int    i, err;
  void  *rval;
  struct glt_args *args;
#ifdef TASKLET
    ABT_task *tasklet_ids;
#else
  ABT_thread *ult_ids;
#endif
  //printf("Init\n");
  uts_parseParams(argc, argv);
  uts_printParams();
  //cb_init();
  //glt_barrier_create(GET_NUM_THREADS, &barrier);
//printf("despues barrier\n");
  /* allocate stealstacks */
  
  
  
  for (i = 0; i < num_threads; i++) {
      //printf("antes de ss %d\n",i);

    stealStack[i] = ALLOC (sizeof(StealStack));
    ss_init(stealStack[i], MAXSTACKDEPTH);
  }
//printf("despues ss\n");

  /* initialize root node and push on thread 0 stack */
  uts_initRoot(&root, type);
  ss_push(stealStack[0], &root);
//printf("despues push\n");
#ifdef TASKLET
    tasklet_ids = malloc(sizeof(ABT_task)*num_threads);
#else
  ult_ids = malloc(sizeof(ABT_thread)*num_threads);
#endif
  args = malloc(sizeof(struct glt_args)*num_threads);
  //pthread_key_create(&pthread_thread_num, NULL);

  /* start timing */
  t1 = uts_wctime();

  for (i = 0; i < num_threads; i++) {
    ss_initState(stealStack[i]);
    args[i].ss = stealStack[i];
    args[i].id = i;
    //printf("creando tares %d\n",i);
    //err = pthread_create(&thread_ids[i], NULL, pthread_spawn_search, (void*)&args[i]);
    //void glt_ult_creation_to(void(*thread_func)(void *), void *arg, GLT_ult *new_ult, int dest)
#ifdef TASKLET
    //glt_tasklet_creation_to(glt_spawn_search, (void*)&args[i],&tasklet_ids[i],i);
    ABT_task_create(pools[i],abt_spawn_search,(void*)&args[i],&tasklet_ids[i]);
#else
    //glt_ult_creation_to(glt_spawn_search, (void*)&args[i],&ult_ids[i],i);
        ABT_thread_create(pools[i],abt_spawn_search,(void*)&args[i],ABT_THREAD_ATTR_NULL,&ult_ids[i]);

#endif
  }
      tcreation = uts_wctime();

  //printf("Creadas las tareas\n");
  ABT_thread_yield();
      tyield = uts_wctime();

  //printf("Ejecutada la mia\n");
  for (i = 0; i < GET_NUM_THREADS; i++) {
    //pthread_join(thread_ids[i], &rval);
          //printf("joining tares %d\n",i);
#ifdef TASKLET
        //glt_tasklet_join(&tasklet_ids[i]);
        ABT_task_free(&tasklet_ids[i]);
#else
//        glt_ult_join(&ult_ids[i]);
        ABT_thread_free(&ult_ids[i]);

#endif

  }
    tjoin = uts_wctime();

  //printf("join acabado\n");

  /* stop timing */
  t2 = uts_wctime();

  showStats(t2-t1);
    printf("ARGOBOTS breakdown Threads: %d, Total: %f, Create: %f, Yield: %f, Join: %f\n",GET_NUM_THREADS,t2-t1,tcreation-t1,tyield-tcreation,tjoin-tyield);

  //glt_finalize();
  ABT_finalize();
  return 0;
}
#endif /* Argobots */

#ifdef QTHREADS

struct glt_args {
	StealStack *ss;
	int         id;
};

/* Glt ParTreeSearch Wrapper */
void qth_spawn_search(void *arg)
{
	
	parTreeSearch(((struct glt_args*)arg)->ss);

}

int qth_main(int argc, char *argv[]) {
    
    
    qthread_initialize();

    int num_threads= qthread_num_shepherds (); 
  Node   root;

  double t1, t2, tcreation, tyield, tjoin;
  int    i, err;
  void  *rval;
  struct glt_args *args;

  aligned_t *ult_ids;

  //printf("Init\n");
  uts_parseParams(argc, argv);
  uts_printParams();
  //cb_init();
  //glt_barrier_create(GET_NUM_THREADS, &barrier);
//printf("despues barrier\n");
  /* allocate stealstacks */
  
  
  
  for (i = 0; i < num_threads; i++) {
      //printf("antes de ss %d\n",i);

    stealStack[i] = ALLOC (sizeof(StealStack));
    ss_init(stealStack[i], MAXSTACKDEPTH);
  }
//printf("despues ss\n");

  /* initialize root node and push on thread 0 stack */
  uts_initRoot(&root, type);
  ss_push(stealStack[0], &root);
//printf("despues push\n");

  ult_ids = malloc(sizeof(aligned_t)*num_threads);
  args = malloc(sizeof(struct glt_args)*num_threads);
  //pthread_key_create(&pthread_thread_num, NULL);

  /* start timing */
  t1 = uts_wctime();

  for (i = 0; i < num_threads; i++) {
    ss_initState(stealStack[i]);
    args[i].ss = stealStack[i];
    args[i].id = i;
    //printf("creando tares %d\n",i);
    //err = pthread_create(&thread_ids[i], NULL, pthread_spawn_search, (void*)&args[i]);
    //void glt_ult_creation_to(void(*thread_func)(void *), void *arg, GLT_ult *new_ult, int dest)

    //glt_ult_creation_to(glt_spawn_search, (void*)&args[i],&ult_ids[i],i);
        qthread_fork_to((void *)qth_spawn_search,(void*)&args[i],&ult_ids[i],i);


  }
  tcreation = uts_wctime();
  //printf("Creadas las tareas\n");
  qthread_yield();
  tyield = uts_wctime();
  //printf("Ejecutada la mia\n");
  for (i = 0; i < GET_NUM_THREADS; i++) {

        qthread_readFF(NULL, &ult_ids[i]);
  }
  tjoin = uts_wctime();
  //printf("join acabado\n");

  /* stop timing */
  t2 = uts_wctime();

  showStats(t2-t1);
  printf("QTHREAD breakdown Threads: %d, Total: %f, Create: %f, Yield: %f, Join: %f\n",GET_NUM_THREADS,t2-t1,tcreation-t1,tyield-tcreation,tjoin-tyield);
  //glt_finalize();
  qthread_finalize();
  return 0;
}
#endif /* Qthreads */

#ifdef MASSIVETHREADS

struct glt_args {
	StealStack *ss;
	int         id;
};

/* Glt ParTreeSearch Wrapper */
void myth_spawn_search(void *arg)
{
	
	parTreeSearch(((struct glt_args*)arg)->ss);

}

int myth_main(int argc, char *argv[]) {
    
    
    myth_init();

    num_workers = atoi(getenv("MYTH_WORKER_NUM"));
    int num_threads= GET_NUM_THREADS; 
  Node   root;
   
  double t1, t2, tcreation, tyield, tjoin;
  int    i, err;
  void  *rval;
  struct glt_args *args;

  myth_thread_t *ult_ids;

  //printf("Init con %d workers\n",GET_NUM_THREADS);
  uts_parseParams(argc, argv);
  uts_printParams();
  //cb_init();
  //glt_barrier_create(GET_NUM_THREADS, &barrier);
//printf("despues barrier\n");
  /* allocate stealstacks */
  
  
  
  for (i = 0; i < num_threads; i++) {
      //printf("antes de ss %d\n",i);

    stealStack[i] = ALLOC (sizeof(StealStack));
    ss_init(stealStack[i], MAXSTACKDEPTH);
  }
//printf("despues ss\n");

  /* initialize root node and push on thread 0 stack */
  uts_initRoot(&root, type);
  ss_push(stealStack[0], &root);
//printf("despues push\n");

  ult_ids = malloc(sizeof(myth_thread_t)*num_threads);
  args = malloc(sizeof(struct glt_args)*num_threads);
  //pthread_key_create(&pthread_thread_num, NULL);

  /* start timing */
  t1 = uts_wctime();

  for (i = 0; i < num_threads; i++) {
    ss_initState(stealStack[i]);
    args[i].ss = stealStack[i];
    args[i].id = i;
    //printf("creando tares %d\n",i);
    
        ult_ids[i] = myth_create((void *)myth_spawn_search,(void*)&args[i]);


  }
    tcreation = uts_wctime();

  //printf("Creadas las tareas\n");
  myth_yield(0);
    tyield = uts_wctime();

  //printf("Ejecutada la mia\n");
  for (i = 0; i < GET_NUM_THREADS; i++) {

        myth_join(ult_ids[i], NULL);
  }
  //printf("join acabado\n");
    tjoin = uts_wctime();

  /* stop timing */
  t2 = uts_wctime();

  showStats(t2-t1);
  printf("MASSIVETHREAD breakdown Threads: %d, Total: %f, Create: %f, Yield: %f, Join: %f\n",GET_NUM_THREADS,t2-t1,tcreation-t1,tyield-tcreation,tjoin-tyield);

  //glt_finalize();
  myth_fini();
  return 0;
}
#endif /* MASSIVETHREADS */

/*  Main() function for: Sequential, OpenMP, UPC, and Shmem
 *
 *  Notes on execution model:
 *     - under openMP, global vars are all shared
 *     - under UPC, global vars are private unless explicitly shared
 *     - UPC is SPMD starting with main, OpenMP goes SPMD after
 *       parsing parameters
 */
int main(int argc, char *argv[]) {
  Node root;
#ifdef GLT
  return glt_main(argc, argv);
#endif
#ifdef ARGOBOTS
    return abt_main(argc, argv);
#endif  
#ifdef QTHREADS
    return qth_main(argc, argv);
#endif
#ifdef MASSIVETHREADS
    return myth_main(argc, argv);
#endif
#ifdef __PTHREADS__
  return pthread_main(argc, argv);
#endif

#ifdef _SHMEM 
  start_pes(0);
#endif

  /* determine benchmark parameters (all PEs) */
  uts_parseParams(argc, argv);

#ifdef UTS_STAT
  if (stats)
    initHist();
#endif  

  /* cancellable barrier initialization (single threaded under OMP) */
  cb_init();

/********** SPMD Parallel Region **********/
#pragma omp parallel
  {
    double t1, t2, et;
    StealStack * ss;    

    /* show parameter settings */
    if (GET_THREAD_NUM == 0) {
      uts_printParams();
    }

    /* initialize stealstacks */
#ifdef _SHMEM
    {
      /* Shared allocation is a collective operation in Shmem.  These
       * need to be done all at once and in the same order on each PE.
       *
       * Note: Only our own stealstack will contain valid data as UTS runs.
       * For stats, we'll need to gather everyone else's stealstacks
       */
      int i;
      for (i = 0; i < GET_NUM_THREADS; i++) {
        stealStack[i] = (SHARED StealStack *) ALLOC (sizeof(StealStack));
        ss = (StealStack *) stealStack[i];	
        ss_init(ss, MAXSTACKDEPTH);
      }

      ss = stealStack[GET_THREAD_NUM];
    }

#else
    stealStack[GET_THREAD_NUM] = (SHARED StealStack *) ALLOC (sizeof(StealStack));
    ss = (StealStack *) stealStack[GET_THREAD_NUM];	
    ss_init(ss, MAXSTACKDEPTH);
#endif /* _SHMEM */
    
    /* initialize root node and push on thread 0 stack */
    if (GET_THREAD_NUM == 0) {
      initRootNode(&root, type);
      ss_push(ss, &root);
    }

    // line up for the start
#pragma omp barrier    
    BARRIER
    
    /* time parallel search */
    ss_initState(ss);
    t1 = uts_wctime();
    parTreeSearch(ss);
    t2 = uts_wctime();
    et = t2 - t1;

#ifdef TRACE
    startTime[GET_THREAD_NUM] = t1;
    ss->md->sessionRecords[SS_IDLE][ss->entries[SS_IDLE] - 1].endTime = t2;
#endif

#pragma omp barrier
    BARRIER

    /* display results */
    if (GET_THREAD_NUM == 0) {
      showStats(et);
    } 
  }
/********** End Parallel Region **********/

  return 0;
}
