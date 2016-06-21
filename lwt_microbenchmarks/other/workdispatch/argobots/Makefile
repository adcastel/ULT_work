INI=argo_task
OBJECTS= argo_task_ult argo_task_ult_pool argo_task_ult_single_pool argo_task_task argo_task_task_pool argo_task_task_single_pool 
CC=gcc
CCFLAGS=-Wall  
AINCLUDE=-I/home/adcastel/argobots_opt/include -L/home/adcastel/argobots_opt/lib -labt -lm
V=verbose
all:
#	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult $(AINCLUDE) -DULT 
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult_pool $(AINCLUDE)  -DPOOL -DULT
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult_single_pool $(AINCLUDE)  -DPOOL -DSINGLEPOOL -DULT
#	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task $(AINCLUDE)
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task_pool $(AINCLUDE)  -DPOOL
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task_single_pool $(AINCLUDE)  -DPOOL -DSINGLEPOOL 

info:
#	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult $(AINCLUDE) -DULT -DVERBOSE 
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult_pool $(AINCLUDE)  -DPOOL -DULT -DVERBOSE
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult_single_pool $(AINCLUDE)  -DPOOL -DSINGLEPOOL -DULT -DVERBOSE
#	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task $(AINCLUDE) -DVERBOSE
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task_pool $(AINCLUDE)  -DPOOL -DVERBOSE
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task_single_pool $(AINCLUDE)  -DPOOL -DSINGLEPOOL -DVERBOSE

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
