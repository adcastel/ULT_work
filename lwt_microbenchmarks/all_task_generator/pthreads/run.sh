#/bin/bash

rm *.dat

export MYTH_BIND_WORKERS=1


export LD_LIBRARY_PATH=/home/adcastel/massivethreads_hf/lib:$LD_LIBRARY_PATH 
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_hf 100 1 >> mt_hf_task_100.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_hf 1000 1 >> mt_hf_task_1000.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_hf 2000 1 >> mt_hf_task_2000.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_hf 100 500 >> mt_hf_bigtask_100.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_hf 1000 500 >> mt_hf_bigtask_1000.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_hf 2000 500 >> mt_hf_bigtask_2000.dat ; done


export LD_LIBRARY_PATH=/home/adcastel/massivethreads_wf/lib:$LD_LIBRARY_PATH 
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_wf 100 1 >> mt_wf_task_100.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_wf 1000 1 >> mt_wf_task_1000.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_wf 2000 1 >> mt_wf_task_2000.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_wf 100 500 >> mt_wf_bigtask_100.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_wf 1000 500 >> mt_wf_bigtask_1000.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_wf 2000 500 >> mt_wf_bigtask_2000.dat ; done
