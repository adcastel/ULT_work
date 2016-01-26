#/bin/bash

rm *.dat

export LD_LIBRARY_PATH=/home/adcastel/qthreads/lib:$LD_LIBRARY_PATH

for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 5000000  >> qthread_for_5000000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 1000 >> qthread_for_1000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 5000000  >> qthread_for_1_seph_5000000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 1000 >> qthread_for_1_seph_1000.dat; done

for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 5000000  >> qthread_ft_for_5000000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 1000 >> qthread_ft_for_1000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 5000000  >> qthread_ft_for_1_seph_5000000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 1000 >> qthread_ft_for_1_seph_1000.dat; done


