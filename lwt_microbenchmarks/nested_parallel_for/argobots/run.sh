#/bin/bash

rm *.dat

for i in {1..72}; do ./argo_nested_ult_pool $i 100 >> argo_ult_pool_100.dat; done
for i in {1..72}; do ./argo_nested_ult_single_pool $i 100 >> argo_ult_sp_100.dat; done

for i in {1..72}; do ./argo_nested_task_pool $i 100 >> argo_task_pool_100.dat; done
for i in {1..72}; do ./argo_nested_task_single_pool $i 100 >> argo_task_sp_100.dat; done

for i in {1..72}; do ./argo_nested_ult_pool $i 1000 >> argo_ult_pool_1000.dat; done
for i in {1..72}; do ./argo_nested_ult_single_pool $i 1000 >> argo_ult_sp_1000.dat; done

for i in {1..72}; do ./argo_nested_task_pool $i 1000 >> argo_task_pool_1000.dat; done
for i in {1..72}; do ./argo_nested_task_single_pool $i 1000 >> argo_task_sp_1000.dat; done
