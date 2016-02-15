/* 
 * File:   accalt.h
 * Author: adcastel
 *
 * Created on 11 de febrero de 2016, 16:20
 */




#ifndef accalt_H
#define	accalt_H



#ifdef ARGOBOTS
#include <stdio.h>
#include <stdlib.h>
#include <abt.h>



#define ACCALT_ult ABT_thread
#define ACCALT_tasklet ABT_task
#define ACCALT_thread ABT_xstream

typedef struct accalt_team {
    ABT_xstream master;
    ABT_xstream *team;
    int num_xstreams;
    int num_pools;
    ABT_pool *pools;
} accalt_team_t;


#endif
#ifdef MASSIVETHREADS
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <myth.h>

#define ACCALT_ult myth_thread_t
#define ACCALT_tasklet myth_thread_t
#define ACCALT_thread myth_thread_t
#define ACCALT_ult_attribute NULL

typedef struct accalt_team {
    int num_workers;
} accalt_team_t;

#endif

#ifdef QTHREADS
#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h>

#define ACCALT_ult aligned_t
#define ACCALT_tasklet aligned_t
#define ACCALT_thread aligned_t
#define ACCALT_ult_attribute NULL

typedef struct accalt_team {
    int num_shepherds;
    int num_workers_per_shepherd;
} accalt_team_t;

#endif

#define ACCALT_VERSION "You are using accalt 0.1v (adcastel@uji.es)"
/* Main team structure*/
void __attribute__((constructor)) accalt_start(void);
void __attribute__((destructor)) accalt_end(void);


void accalt_init(int argc, char * argv[]);
void accalt_finalize();

ACCALT_ult * accalt_ult_malloc(int number_of_ult);
ACCALT_tasklet * accalt_tasklet_malloc(int number_of_tasklets);

void accalt_ult_creation(void(*thread_func)(void *), void *arg, ACCALT_ult *new_ult);
void accalt_ult_creation_to(void(*thread_func)(void *), void *arg, ACCALT_ult *new_ult, int dest);

void accalt_tasklet_creation(void(*thread_func)(void *), void *arg, ACCALT_tasklet *new_ult);
void accalt_tasklet_creation_to(void(*thread_func)(void *), void *arg, ACCALT_tasklet *new_ult, int dest);

void accalt_yield();
void accalt_yield_to(ACCALT_ult ult);

void accalt_ult_join(ACCALT_ult *ult);
void accalt_tasklet_join(ACCALT_tasklet *tasklet);

int accalt_get_thread_num();
int accalt_get_num_threads();

#endif	/* accalt_H */

