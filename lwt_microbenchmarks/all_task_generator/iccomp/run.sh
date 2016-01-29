#/bin/bash


rm *.dat
export OMP_WAIT_POLICY=passive
export OMP_NESTED=true
export OMP_PROC_BIND=true

for i in {1..72}; do  OMP_NUM_THREADS=$i ./omp_task 100 1 >> icc_passive_all_task_100.dat; done
for i in {1..72}; do  OMP_NUM_THREADS=$i ./omp_task 1000 1 >> icc_passive_all_task_1000.dat; done
for i in {1..72}; do  OMP_NUM_THREADS=$i ./omp_task 2000 1 >> icc_passive_all_task_2000.dat; done
for i in {1..72}; do  OMP_NUM_THREADS=$i ./omp_task 100 500 >> icc_passive_all_bigtask_100.dat; done
for i in {1..72}; do  OMP_NUM_THREADS=$i ./omp_task 1000 500 >> icc_passive_all_bigtask_1000.dat; done
for i in {1..72}; do  OMP_NUM_THREADS=$i ./omp_task 2000 500 >> icc_passive_all_bigtask_2000.dat; done


