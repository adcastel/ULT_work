INI=omp_task
OBJECTS= openmp_$(INI)  
CC=gcc
CCFLAGS=-Wall -lm -O0
OMP_LDFLAGS=-fopenmp

all:
	$(CC) $(CCFLAGS) $(INI).c -o $(INI) $(OMP_LDFLAGS)

info:
	$(CC) $(CCFLAGS) $(INI).c -o $(INI) $(OMP_LDFLAGS) -DVERBOSE

look:

	$(CC) $(CCFLAGS) $(INI).c -fdump-tree-optimized $(OMP_LDFLAGS)

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
