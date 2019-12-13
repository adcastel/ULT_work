INI=task
OBJECTS= pthread_task
CC=gcc
CCFLAGS=-Wall
#WFINCLUDE=-I/opt/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -lm
HFINCLUDE=-lpthread -lm

V=verbose
all:
	$(CC) $(CCFLAGS) pthreads_$(INI).c -o pthread_task $(HFINCLUDE)  

info:

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s

