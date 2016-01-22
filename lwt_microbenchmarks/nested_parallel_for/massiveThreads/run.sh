#/bin/bash

rm *.dat

export MYTH_BIND_WORKERS=1


export LD_LIBRARY_PATH=/home/adcastel/massivethreads_hf/lib:$LD_LIBRARY_PATH 
for i in {1..36}; do export MYTH_WORKER_NUM=$i; ./mt_hf 100 >> mt_hf_nested_100.dat ; done
for i in {1..36}; do export MYTH_WORKER_NUM=$i; ./mt_hf 1000 >> mt_hf_nested_1000.dat ; done


export LD_LIBRARY_PATH=/home/adcastel/massivethreads_wf/lib:$LD_LIBRARY_PATH 
for i in {1..36}; do export MYTH_WORKER_NUM=$i; ./mt_wf 100 1 >> mt_wf_nested_100.dat ; done
for i in {1..36}; do export MYTH_WORKER_NUM=$i; ./mt_wf 1000 1 >> mt_wf_nested_1000.dat ; done
