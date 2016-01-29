#/bin/bash

export OMP_NESTED=true
export OMP_PROC_BIND=true

export OMP_WAIT_POLICY=passive
rm *.dat
for i in {1..36}; do OMP_NUM_THREADS=$i ./nested_parallel_for 1000 >> omp_nested_pass_1000.dat; done
for i in {1..36}; do OMP_NUM_THREADS=$i ./nested_parallel_for 100 >> omp_nested_pass_100.dat; done


unset OMP_WAIT_POLICY

for i in {1..36}; do OMP_NUM_THREADS=$i ./nested_parallel_for 1000 >> omp_nested_def_1000.dat; done
for i in {1..36}; do OMP_NUM_THREADS=$i ./nested_parallel_for 100 >> omp_nested_def_100.dat; done


export OMP_WAIT_POLICY=passive
