#!/bin/bash
EXE=parallel_for
OMP_THREADS=32
NTASKS=5000000
NXSTREAMS=32
NPOOLS=$NSTREAMS
# Don't modify beyond this line


./argo_task_$EXE $NXSTREAMS $NTASKS 
./argo_thread_$EXE $NXSTREAMS $NTASKS 



