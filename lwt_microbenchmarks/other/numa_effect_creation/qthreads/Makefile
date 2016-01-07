INI=creation_toall
OBJECTS= qthread 
CC=gcc
CCFLAGS=-Wall
AINCLUDE=-I/home/adcastel/qthreads/include -L/home/adcastel/qthreads/lib -lqthreads -lm
V=verbose
all:
	$(CC) $(CCFLAGS) qthreads_$(INI).c -o qthreads $(AINCLUDE)  

info:
	$(CC) $(CCFLAGS) qthreads_$(INI).c -o qthreads $(AINCLUDE) -DVERBOSE 

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s

