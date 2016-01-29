INI=parallel_for
OBJECTS= openmp_$(INI)  
CC=icc
CCFLAGS=-Wall -O0
OMP_LDFLAGS=-openmp -lm

all:
	$(CC) $(CCFLAGS) $(INI).c -o openmp_$(INI) $(OMP_LDFLAGS)

look:

	$(CC) $(CCFLAGS) $(INI).c -fdump-tree-optimized $(OMP_LDFLAGS)

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
