INI=argo_nested
OBJECTS=  $(INI)_ult_pool $(INI)_ult_single_pool $(INI)_task_pool $(INI)_task_single_pool 
CC=gcc
CCFLAGS=-Wall
AINCLUDE=-I/home/adcastel/argobots_opt/include -L/home/adcastel/argobots_opt/lib -labt -lm
V=verbose
all:
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult_pool $(AINCLUDE)  -DPOOL -DULT
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult_single_pool $(AINCLUDE)  -DPOOL -DSINGLEPOOL -DULT
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task_pool $(AINCLUDE)  -DPOOL
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task_single_pool $(AINCLUDE)  -DPOOL -DSINGLEPOOL 

info:
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult_pool $(AINCLUDE)  -DPOOL -DULT -DVERBOSE
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_ult_single_pool $(AINCLUDE)  -DPOOL -DSINGLEPOOL -DULT -DVERBOSE
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task_pool $(AINCLUDE)  -DPOOL -DVERBOSE
	$(CC) $(CCFLAGS) $(INI).c -o $(INI)_task_single_pool $(AINCLUDE)  -DPOOL -DSINGLEPOOL -DVERBOSE

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
