INI=numa_creation
OBJECTS= argo_mine_$(INI) argo_core_$(INI) argo_proc_$(INI) argo_other_$(INI) argo_ult_mine_$(INI) argo_ult_core_$(INI) argo_ult_proc_$(INI) argo_ult_other_$(INI) argo_task_creation argo_ult_creation
CC=gcc
CCFLAGS=-Wall
AINCLUDE=-I/home/adcastel/argobots/include -L/home/adcastel/argobots/lib -labt -lm
V=verbose
all:
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_mine_$(INI) $(AINCLUDE) -DMINE 
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_core_$(INI) $(AINCLUDE) -DCORE
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_proc_$(INI) $(AINCLUDE) -DPROC
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_other_$(INI) $(AINCLUDE) -DOTHER
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_mine_$(INI) $(AINCLUDE) -DMINE -ULT
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_core_$(INI) $(AINCLUDE) -DCORE -ULT
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_proc_$(INI) $(AINCLUDE) -DPROC -ULT
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_ult_other_$(INI) $(AINCLUDE) -DOTHER -ULT

info:
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_mine_$(INI) $(AINCLUDE) -DMINE -DVERBOSE
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_core_$(INI) $(AINCLUDE) -DCORE -DVERBOSE
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_proc_$(INI) $(AINCLUDE) -DPROC -DVERBOSE
	$(CC) $(CCFLAGS) argo_$(INI).c -o argo_other_$(INI) $(AINCLUDE) -DOTHER -DVERBOSE

create:
	$(CC) $(CCFLAGS) argo_creation_toall.c -o argo_task_creation $(AINCLUDE)
	$(CC) $(CCFLAGS) argo_creation_toall.c -o argo_ult_creation $(AINCLUDE) -DULT
clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
