#/bin/bash

rm *.dat

for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 100 4 1 1 >> qthread_task_100_4.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 100 10 1 1 >> qthread_task_100_10.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 100 4 1 1  >> qthread_task_1_seph_100_4.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 100 10 1 1  >> qthread_task_1_seph_100_10.dat; done 


for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 100 4 1 1 >> qthread_ft_task_100_4.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 100 10 1 1 >> qthread_ft_task_100_10.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 100 4 1 1  >> qthread_ft_task_1_seph_100_4.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 100 10 1 1  >> qthread_ft_task_1_seph_100_10.dat; done 


