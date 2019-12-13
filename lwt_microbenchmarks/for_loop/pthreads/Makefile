INI=for_loop
OBJECTS= pthreads_for_loop 
CC=gcc
CCFLAGS=-Wall
HFINCLUDE=-lpthread -lm
V=verbose
all:
#	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_wf $(WFINCLUDE)  
	$(CC) $(CCFLAGS) pthreads_$(INI).c -o pthreads_for_loop $(HFINCLUDE)  

info:
	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_wf $(WFINCLUDE) -DVERBOSE 
	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_hf $(WFINCLUDE) -DVERBOSE 

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s

