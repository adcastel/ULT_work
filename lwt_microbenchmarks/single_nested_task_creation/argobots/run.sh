#/bin/bash

rm *.dat

for i in {1..72}; do ./argo_task_ult_pool $i 100 4 1 1 >> argo_ult_100_4.dat; done
for i in {1..72}; do ./argo_task_ult_pool $i 100 10 1 1 >> argo_ult_100_10.dat; done
for i in {1..72}; do ./argo_task_task_pool $i 100 4 1 1 >> argo_task_100_4.dat; done
for i in {1..72}; do ./argo_task_task_pool $i 100 10 1 1 >> argo_task_100_10.dat; done

for i in {1..72}; do ./argo_task_ult_single_pool $i 100 4 1 1 >> argo_ult_sp_100_4.dat; done
for i in {1..72}; do ./argo_task_ult_single_pool $i 100 10 1 1 >> argo_ult_sp_100_10.dat; done
for i in {1..72}; do ./argo_task_task_single_pool $i 100 4 1 1 >> argo_task_sp_100_4.dat; done
for i in {1..72}; do ./argo_task_task_single_pool $i 100 10 1 1 >> argo_task_sp_100_10.dat; done

