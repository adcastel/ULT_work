#/bin/bash

rm *.dat
MIN=1
MAX=72

export LD_LIBRARY_PATH=/home/adcastel/argobots/lib:$LD_LIBRARY_PATH



for i in {1..72}; do ./argo_task_pool_parallel_for $i 1000 >> argo_task_pool_1000.dat; done
for i in {1..72}; do ./argo_task_pool_parallel_for $i 5000000 >> argo_task_pool_5000000.dat; done

for i in {1..72}; do ./argo_ult_pool_parallel_for $i 1000 >> argo_ult_pool_1000.dat; done
for i in {1..72}; do ./argo_ult_pool_parallel_for $i 5000000 >> argo_ult_pool_5000000.dat; done

for i in {1..72}; do ./argo_task_single_pool_parallel_for $i 1000 >> argo_task_sp_1000.dat; done
for i in {1..72}; do ./argo_task_single_pool_parallel_for $i 5000000 >> argo_task_sp_5000000.dat; done

for i in {1..72}; do ./argo_ult_single_pool_parallel_for $i 1000 >> argo_ult_sp_1000.dat; done
for i in {1..72}; do ./argo_ult_single_pool_parallel_for $i 5000000 >> argo_ult_sp_5000000.dat; done


