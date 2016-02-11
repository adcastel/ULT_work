

#ifdef ARGOBOTS
#include <abt.h>

#define ULT 

#endif


void acoilt_init()
ABT_init() // Argobots
Create ES //environment variable?
Create Pools //environment variable?
Create Scheduler //Default?
myth_init() //MassiveThreads
qthread_initialize() //qthreads
ConverseInit() // Converse in return mode
Initialize message functions. They are unknown at the beginning
Go //No init function
call the GOMAXPROCS function

Library finalization. This function finalizes the library

acoilt_finalize()
ABT_finalize() // Argobots
Join ES
Free ES
myth_fini() //MassiveThreads
Qthreads // No finalize function
CsdExitScheduler() // ConverseThreads
GO //No finalize function

ULT allocation. This function allocates memory for a given number of ULTs

acoilt_ult_malloc()

Tasklet allocation.  This function allocates memory for a given number of Tasklets

acoilt_tasklet_malloc()
call to acoilt_ult_malloc if tasklets are not supported

ULT creation. This function creates a ULT into the current thread queue

acoilt_ult_creation()
ABT_thread_create(self_pool) //Argobots
myth_create() //MassiveThreads
qthread_fork() //Qthreads
CthCreate() + CthAwaken() // Converse
go + pointer // GO 




ULT creation to. This function creates a ULT into a given thread queue

acoilt_ult_creation_to()
ABT_thread_create(pool) //Argobots
acoilt_ult_creation() //MassiveThreads
qthread_fork_to() // Qthreads
acoilt_ult_creation() //ConverseThreads
acoilt_ult_creation() //GO

Tasklet creation. This function creates a Tasklet into the current thread queue

acoilt_tasklet_creation()
call to acoilt_ult_creation() if tasklets are not supported
ABT_task_create(self_pool) //Argobots
CmiSetHandler() + CmiSyncSend() //ConverseThreads

Tasklet creation to. This function creates a Tasklet into a given thread queue

acoilt_tasklet_creation_to()
call to acoilt_ult_creation_to() if tasklets are not supported
ABT_thread_create(pool) //Argobots
CmiSetHandler() + CmiSyncSend() //ConverseThreads

Yield function. This function gives the control to the scheduler and the current thread is queued

acoilt_yield()
ABT_thread_yield() // Argobots
myth_yield() //MassiveThreads
qthread_yield() //Qthreads
CthYield() //ConverseThreads
GO // No yield function

Yield to function. This function gives the control to a given thread and the current thread is queued

acoilt_yield_to()
ABT_thread_yield_to() // Argobots
acoilt_yield() //MassiveThreads
acoilt_yield() //Qthreads
CthYieldPrio() //ConverseThreads
GO // No yield_to function

ULT join. This function joins a given ULT

acoilt_ult_join()
ABT_thread_free() // Argobots
myth_join() //MassiveThreads
qthread_readFF() //Qthreads
Not supported in ConverseThreads. CmiNodeBarrier()?
Channel //GO

Tasklet join.  This function joins a given Tasklet

acoilt_tasklet_join()
call to acoilt_ult_join if tasklets are not supported
ABT_task_free() // Argobots
Not supported in ConverseThreads. CmiNodeBarrier()?


Structures

ULT
ABT_thread //Argobots
myth_thread_t //MassiveThreads
aligned_t //Qthreads
CthThread //ConverseThreads
goroutine //GO
	
Tasklet
ABT_task //Argobots
Message //ConverseThreads

