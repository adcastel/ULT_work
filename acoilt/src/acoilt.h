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

#define ULT ABT_thread
#define Tasklet ABT_task

typedef struct acoilt_team {
    ABT_xstream master;
    ABT_xstream *team;
    int num_xstreams;
    int num_pools;
    ABT_pool *pools;
}acoilt_team_t;


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
#endif

#ifdef QTHREADS
#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h>
#endif

#define ACOILT_VERSION "You are using ACOILT 0.1v (adcastel@uji.es)"
/* Main team structure*/
void __attribute__((constructor)) acoilt_start(void);

void __attribute__((destructor)) acoilt_end(void);


void acoilt_init(int argc, char * argv[]);
void acoilt_finalize();




#endif	/* ACOILT_H */

