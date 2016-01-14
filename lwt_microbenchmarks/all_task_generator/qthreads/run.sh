#/bin/bash

rm *.dat

for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 100 1 >> qthread_task_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 1000 1 >> qthread_task_1000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 2000 1 >> qthread_task_2000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 100 1 >> qthread_task_1_seph_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 1000 1 >> qthread_task_1_seph_1000.dat; done
#for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 2000 1 >> qthread_task_1_seph_2000.dat; done

for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 100 1 >> qthread_fork_to_task_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 1000 1 >> qthread_fork_to_task_1000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 2000 1 >> qthread_fork_to_task_2000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 100 1 >> qthread_fork_to_task_1_seph_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 1000 1 >> qthread_fork_to_task_1_seph_1000.dat; done
#for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 2000 1 >> qthread_fork_to_task_1_seph_2000.dat; done

for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 100 500 >> qthread_bigtask_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 1000 500 >> qthread_bigtask_1000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads 2000 500 >> qthread_bigtask_2000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 100 500 >> qthread_bigtask_1_seph_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 1000 500 >> qthread_bigtask_1_seph_1000.dat; done
#for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads 2000 500 >> qthread_bigtask_1_seph_2000.dat; done

for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 100 500 >> qthread_fork_to_bigtask_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 1000 500 >> qthread_fork_to_bigtask_1000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=$i; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1; ./qthreads_fork_to 2000 500 >> qthread_fork_to_bigtask_2000.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 100 500 >> qthread_fork_to_bigtask_1_seph_100.dat; done 
for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 1000 500 >> qthread_fork_to_bigtask_1_seph_1000.dat; done
#for i in {1..36}; do export QTHREAD_NUM_SHEPHERDS=1; export QTHREAD_NUM_WORKERS_PER_SHEPHERD=$i; ./qthreads_fork_to 2000 500 >> qthread_fork_to_bigtask_1_seph_2000.dat; done


