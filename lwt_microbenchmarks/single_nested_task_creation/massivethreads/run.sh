#/bin/bash

rm *.dat

export MYTH_BIND_WORKERS=1


export LD_LIBRARY_PATH=/home/adcastel/massivethreads_hf/lib:$LD_LIBRARY_PATH 
for i in {1..36}; do export MYTH_WORKER_NUM=$i; ./mt_hf 100 4 1 1 >> mt_hf_task_100_4.dat ; done
for i in {1..36}; do export MYTH_WORKER_NUM=$i; ./mt_hf 100 10 1 1 >> mt_hf_task_100_10.dat ; done


export LD_LIBRARY_PATH=/home/adcastel/massivethreads_wf/lib:$LD_LIBRARY_PATH 
for i in {1..36}; do export MYTH_WORKER_NUM=$i; ./mt_wf 100 4 1 1 >> mt_wf_task_100_4.dat ; done
for i in {1..36}; do export MYTH_WORKER_NUM=$i; ./mt_wf 100 10 1 1 >> mt_wf_task_100_10.dat ; done
