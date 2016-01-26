INI=parallel_for
OBJECTS= argo_task_$(INI) argo_task_pool_$(INI) argo_task_single_pool_$(INI) argo_task_ws_$(INI) argo_ult_$(INI) argo_ult_pool_$(INI) argo_ult_single_pool_$(INI) argo_ult_ws_$(INI) 
CC=gcc
CCFLAGS=-Wall
AINCLUDE=-I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -lm
V=verbose
all:
#	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_task_$(INI) $(AINCLUDE)  
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_task_pool_$(INI) $(AINCLUDE)  -DPOOL
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_task_single_pool_$(INI) $(AINCLUDE)  -DPOOL -DSINGLEPOOL
#	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_task_ws_$(INI) $(AINCLUDE)  -DWORKSTEALING -DPOOL
#	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_$(INI) $(AINCLUDE) -DULT
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_pool_$(INI) $(AINCLUDE)  -DULT -DPOOL
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_single_pool_$(INI) $(AINCLUDE)  -DULT -DPOOL -DSINGLEPOOL
#	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_ws_$(INI) $(AINCLUDE)  -DULT -DWORKSTEALING -DPOOL

info:
#	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_task_$(INI) $(AINCLUDE) -DVERBOSE 
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_task_pool_$(INI) $(AINCLUDE)  -DPOOL -DVERBOSE
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_task_single_pool_$(INI) $(AINCLUDE)  -DPOOL -DSINGLEPOOL -DVERBOSE
#	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_task_ws_$(INI) $(AINCLUDE)  -DWORKSTEALING -DPOOL -DVERBOSE
#	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_$(INI) $(AINCLUDE) -DULT -DVERBOSE
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_pool_$(INI) $(AINCLUDE)  -DULT -DPOOL -DVERBOSE
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_single_pool_$(INI) $(AINCLUDE)  -DULT -DSINGLEPOOL -DVERBOSE
#	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_ws_$(INI) $(AINCLUDE)  -DULT -DWORKSTEALING -DPOOL -DVERBOSE

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
