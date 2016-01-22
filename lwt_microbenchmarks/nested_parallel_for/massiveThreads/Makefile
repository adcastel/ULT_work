INI=nested
OBJECTS= mt_wf mt_hf
CC=gcc
CCFLAGS=-Wall
WFINCLUDE=-I/home/adcastel/massivethreads_wf/include -L/home/adcastel/massivethreads_wf/lib -lmyth -lm
HFAINCLUDE=-I/home/adcastel/massivethreads_hf/include -L/home/adcastel/massivethreads_hf/lib -lmyth -lm
V=verbose
all:
	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_wf $(WFINCLUDE)  
	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_hf $(WFINCLUDE)  

info:
	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_wf $(WFINCLUDE) -DVERBOSE 
	$(CC) $(CCFLAGS) massivethreads_$(INI).c -o mt_hf $(WFINCLUDE) -DVERBOSE 

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s

