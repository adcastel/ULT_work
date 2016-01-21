INI=nested_parallel_for
OBJECTS= $(INI)  
CC=icc
CCFLAGS=-Wall -O0 
OMP_LDFLAGS=-fopenmp -lm

all:
	$(CC) $(CCFLAGS) $(INI).c -o $(INI) $(OMP_LDFLAGS)

look:

	$(CC) $(CCFLAGS) $(INI).c -fdump-tree-optimized $(OMP_LDFLAGS)

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
