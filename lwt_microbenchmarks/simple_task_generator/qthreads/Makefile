INI=task
OBJECTS= qthreads qthreads_fork_to
CC=gcc
CCFLAGS=-Wall
AINCLUDE=-I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthread -lm
V=verbose
all:
	$(CC) $(CCFLAGS) qthreads_$(INI).c -o qthreads $(AINCLUDE)  
	$(CC) $(CCFLAGS) qthreads_$(INI).c -o qthreads_fork_to $(AINCLUDE) -DFORKTO 

info:
	$(CC) $(CCFLAGS) qthreads_$(INI).c -o qthreads $(AINCLUDE) -DVERBOSE 
	$(CC) $(CCFLAGS) qthreads_$(INI).c -o qthreads_fork_to $(AINCLUDE) -DFORKTO -DVERBOSE 

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s

