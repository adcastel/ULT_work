INI=parallel_for
OBJECTS= openmp_$(INI)  
CC=gcc
CCFLAGS=-Wall -lm
OMP_LDFLAGS=-fopenmp

all:
	$(CC) $(CCFLAGS) $(INI).c -o openmp_$(INI) $(OMP_LDFLAGS)

look:

	$(CC) $(CCFLAGS) $(INI).c -fdump-tree-optimized $(OMP_LDFLAGS)

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
