INI=task
OBJECTS= pth
CC=gcc
CCFLAGS=-Wall
WFINCLUDE=-I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -lm
HFINCLUDE=-lpthread -lm
V=verbose
all:
#	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_wf $(WFINCLUDE)  
	$(CC) $(CCFLAGS) pthreads_$(INI).c -o pth $(HFINCLUDE)  

info:
	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_wf $(WFINCLUDE) -DVERBOSE 
	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_hf $(WFINCLUDE) -DVERBOSE 

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s

