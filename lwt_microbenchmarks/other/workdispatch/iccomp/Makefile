INI=omp_task
OBJECTS= openmp_$(INI)  
CC=icc
CCFLAGS=-Wall -O0 
OMP_LDFLAGS=-openmp

all:
	$(CC) $(CCFLAGS) $(INI).c -o $(INI) $(OMP_LDFLAGS)
	$(CC) $(CCFLAGS) parallel_for.c -o parallel_for $(OMP_LDFLAGS)


look:

	$(CC) $(CCFLAGS) $(INI).c -S -fsource-asm $(OMP_LDFLAGS)

clean:
	rm -rf *.o $(OBJECTS) *.*~ a.out *.optimized *.s
