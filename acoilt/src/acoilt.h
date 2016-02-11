/* 
 * File:   acoilt.h
 * Author: adcastel
 *
 * Created on 11 de febrero de 2016, 16:20
 */




#ifndef ACOILT_H
#define	ACOILT_H

#ifdef ARGOBOTS
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
    
#endif
#include <stdio.h>


/* Main team structure*/





#endif	/* ACOILT_H */

