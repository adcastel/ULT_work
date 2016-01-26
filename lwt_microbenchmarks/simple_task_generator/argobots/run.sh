#/bin/bash

rm *.dat

for i in {1..72}; do ./argo_task_ult_pool $i 100 1 >> argo_ult_all_task_100.dat; done
for i in {1..72}; do ./argo_task_ult_pool $i 1000 1 >> argo_ult_all_task_1000.dat; done
for i in {1..72}; do ./argo_task_ult_pool $i 2000 1 >> argo_ult_all_task_2000.dat; done
for i in {1..72}; do ./argo_task_ult_single_pool $i 100 1 >> argo_ult_sp_all_task_100.dat; done
for i in {1..72}; do ./argo_task_ult_single_pool $i 1000 1 >> argo_ult_sp_all_task_1000.dat; done
for i in {1..72}; do ./argo_task_ult_single_pool $i 2000 1 >> argo_ult_sp_all_task_2000.dat; done
for i in {1..72}; do ./argo_task_task_pool $i 100 1 >> argo_task_all_task_100.dat; done
for i in {1..72}; do ./argo_task_task_pool $i 1000 1 >> argo_task_all_task_1000.dat; done
for i in {1..72}; do ./argo_task_task_pool $i 2000 1 >> argo_task_all_task_2000.dat; done
for i in {1..72}; do ./argo_task_task_single_pool $i 100 1 >> argo_task_sp_all_task_100.dat; done
for i in {1..72}; do ./argo_task_task_single_pool $i 1000 1 >> argo_task_sp_all_task_1000.dat; done
for i in {1..72}; do ./argo_task_task_single_pool $i 2000 1 >> argo_task_sp_all_task_2000.dat; done

for i in {1..72}; do ./argo_task_ult_pool $i 100 500 >> argo_ult_all_bigtask_100.dat; done
for i in {1..72}; do ./argo_task_ult_pool $i 1000 500 >> argo_ult_all_bigtask_1000.dat; done
for i in {1..72}; do ./argo_task_ult_pool $i 2000 500 >> argo_ult_all_bigtask_2000.dat; done
for i in {1..72}; do ./argo_task_ult_single_pool $i 100 500 >> argo_ult_sp_all_bigtask_100.dat; done
for i in {1..72}; do ./argo_task_ult_single_pool $i 1000 500 >> argo_ult_sp_all_bigtask_1000.dat; done
for i in {1..72}; do ./argo_task_ult_single_pool $i 2000 500 >> argo_ult_sp_all_bigtask_2000.dat; done
for i in {1..72}; do ./argo_task_task_pool $i 100 500 >> argo_task_all_bigtask_100.dat; done
for i in {1..72}; do ./argo_task_task_pool $i 1000 500 >> argo_task_all_bigtask_1000.dat; done
for i in {1..72}; do ./argo_task_task_pool $i 2000 500 >> argo_task_all_bigtask_2000.dat; done
for i in {1..72}; do ./argo_task_task_single_pool $i 100 500 >> argo_task_sp_all_bigtask_100.dat; done
for i in {1..72}; do ./argo_task_task_single_pool $i 1000 500 >> argo_task_sp_all_bigtask_1000.dat; done
for i in {1..72}; do ./argo_task_task_single_pool $i 2000 500 >> argo_task_sp_all_bigtask_2000.dat; done



