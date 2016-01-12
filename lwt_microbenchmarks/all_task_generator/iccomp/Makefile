INI=omp_task
OBJECTS= openmp_$(INI)  
CC=icc
CCFLAGS=-Wall  -O0
OMP_LDFLAGS=-openmp -lm

all:
	$(CC) $(CCFLAGS) $(INI).c -o $(INI) $(OMP_LDFLAGS)

info:
	$(CC) $(CCFLAGS) $(INI).c -o $(INI) $(OMP_LDFLAGS) -DVERBOSE

look:

	$(CC) $(CCFLAGS) $(INI).c -fdump-tree-optimized $(OMP_LDFLAGS)

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
