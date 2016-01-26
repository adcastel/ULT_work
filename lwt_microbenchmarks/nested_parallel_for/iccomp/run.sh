#/bin/bash



export OMP_WAIT_POLICY=passive

for i in {1..72}; do OMP_NUM_THREADS=$i ./nested_parallel_for 100 >> omp_nested_100.dat; done
for i in {1..72}; do OMP_NUM_THREADS=$i ./nested_parallel_for 1000 >> omp_nested_1000.dat; done




