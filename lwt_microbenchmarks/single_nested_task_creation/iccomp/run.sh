#/bin/bash



export OMP_WAIT_POLICY=passive
export OMP_NESTED=true
export OMP_PROC_BIND=true

for i in {1..72}; do  OMP_NUM_THREADS=$i ./omp_task 100 4 1 1 >> icc_task_100_4.dat; done
for i in {1..72}; do  OMP_NUM_THREADS=$i ./omp_task 100 10 1 1 >> icc_task_100_10.dat; done


