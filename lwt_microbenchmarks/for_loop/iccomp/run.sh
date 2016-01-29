#/bin/bash

export OMP_NESTED=true
export OMP_PROC_BIND=true

export OMP_WAIT_POLICY=passive
rm *.dat
for i in {1..72}; do OMP_NUM_THREADS=$i ./openmp_parallel_for 1000 >> omp_nested_pass_1000.dat; done
for i in {1..72}; do OMP_NUM_THREADS=$i ./openmp_parallel_for 5000000 >> omp_nested_pass_5000000.dat; done


#unset OMP_WAIT_POLICY
#
#for i in {1..72}; do OMP_NUM_THREADS=$i ./openmp_parallel_for 1000 >> omp_nested_pass_1000.dat; done
#for i in {1..72}; do OMP_NUM_THREADS=$i ./openmp_parallel_for 5000000 >> omp_nested_pass_5000000.dat; done


#export OMP_WAIT_POLICY=passive
