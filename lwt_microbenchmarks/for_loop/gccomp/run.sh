#!/bin/bash
EXE=parallel_for
OMP_THREADS=32
ITS=50
NXSTREAMS=32
NPOOLS=$NSTREAMS
# Don't modify beyond this line


OMP_NUM_THREADS=$OMP_THREADS ./openmp_$EXE $ITS



