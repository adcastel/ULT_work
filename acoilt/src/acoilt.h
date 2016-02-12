/* 
 * File:   acoilt.h
 * Author: adcastel
 *
 * Created on 11 de febrero de 2016, 16:20
 */




#ifndef ACOILT_H
#define	ACOILT_H



#ifdef ARGOBOTS
#include <stdio.h>
#include <stdlib.h>
#include <abt.h>



#define ACOILT_ult ABT_thread
#define ACOILT_tasklet ABT_task
#define ACOILT_thread ABT_xstream

typedef struct acoilt_team {
    ABT_xstream master;
    ABT_xstream *team;
    int num_xstreams;
    int num_pools;
    ABT_pool *pools;
} acoilt_team_t;


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

#define ACOILT_ult myth_thread_t
#define ACOILT_tasklet myth_thread_t
#define ACOILT_thread myth_thread_t
#define ACOILT_ult_attribute NULL

typedef struct acoilt_team {
    int num_workers;
} acoilt_team_t;

#endif

#ifdef QTHREADS
#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h>

#define ACOILT_ult aligned_t
#define ACOILT_tasklet aligned_t
#define ACOILT_thread aligned_t
#define ACOILT_ult_attribute NULL

typedef struct acoilt_team {
    int num_shepherds;
    int num_workers_per_shepherd;
} acoilt_team_t;

#endif

#define ACOILT_VERSION "You are using ACOILT 0.1v (adcastel@uji.es)"
/* Main team structure*/
void __attribute__((constructor)) acoilt_start(void);
void __attribute__((destructor)) acoilt_end(void);


void acoilt_init(int argc, char * argv[]);
void acoilt_finalize();

ACOILT_ult * acoilt_ult_malloc(int number_of_ult);
ACOILT_tasklet * acoilt_tasklet_malloc(int number_of_tasklets);

void acoilt_ult_creation(void(*thread_func)(void *), void *arg, ACOILT_ult *new_ult);
void acoilt_ult_creation_to(void(*thread_func)(void *), void *arg, ACOILT_ult *new_ult, int dest);

void acoilt_tasklet_creation(void(*thread_func)(void *), void *arg, ACOILT_tasklet *new_ult);
void acoilt_tasklet_creation_to(void(*thread_func)(void *), void *arg, ACOILT_tasklet *new_ult, int dest);

void acoilt_yield();
void acoilt_yield_to(ACOILT_ult ult);

void acoilt_ult_join(ACOILT_ult *ult);
void acoilt_tasklet_join(ACOILT_tasklet *tasklet);

int acoilt_get_thread_num();
int acoilt_get_num_threads();

#endif	/* ACOILT_H */

