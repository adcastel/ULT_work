#/bin/bash

rm *.dat

export MYTH_BIND_WORKERS=1


export LD_LIBRARY_PATH=/home/adcastel/massivethreads_hf/lib:$LD_LIBRARY_PATH 
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_hf 1000 >> mt_hf_for_1000.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_hf 5000000 >> mt_hf_for_5000000.dat ; done


export LD_LIBRARY_PATH=/home/adcastel/massivethreads_wf/lib:$LD_LIBRARY_PATH 
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_wf 1000 >> mt_wf_for_1000.dat ; done
for i in {1..72}; do export MYTH_WORKER_NUM=$i; ./mt_wf 5000000 >> mt_wf_for_5000000.dat ; done
