#/bin/bash

rm *.dat

export LD_LIBRARY_PATH=/home/adcastel/qthreads/lib:$LD_LIBRARY_PATH

for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 100 1  >> qthread_task_100.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 1000 1 >> qthread_task_1000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 100 1  >> qthread_task_1_seph_100.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 1000 1 >> qthread_task_1_seph_1000.dat; done

for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 100 1 >> qthread_ft_task_100.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 1000 1 >> qthread_ft_task_1000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 100  1 >> qthread_ft_task_1_seph_100.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 1000 1 >> qthread_ft_task_1_seph_1000.dat; done

for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 100 500  >> qthread_btask_100.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 1000 500 >> qthread_btask_1000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 100 500  >> qthread_btask_1_seph_100.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 1000 500 >> qthread_btask_1_seph_1000.dat; done

for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 100 500 >> qthread_ft_btask_100.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 1000 500 >> qthread_ft_btask_1000.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 100  500 >> qthread_ft_btask_1_seph_100.dat; done 
for i in {1..72}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 1000 500 >> qthread_ft_btask_1_seph_1000.dat; done

