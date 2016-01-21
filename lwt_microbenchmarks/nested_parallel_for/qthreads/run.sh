#/bin/bash

rm *.dat

for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 100  >> qthread_nested_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 1000 >> qthread_nested_1000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 100  >> qthread_nested_1_seph_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 1000 >> qthread_nested_1_seph_1000.dat; done

for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 100  >> qthread_fork_to_nested_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 1000  >> qthread_fork_to_nested_1000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 100  >> qthread_fork_to_nested_1_seph_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 1000  >> qthread_fork_to_nested_1_seph_1000.dat; done



