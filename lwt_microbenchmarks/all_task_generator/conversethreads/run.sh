#/bin/bash

rm *.dat

for i in {1..72}; do ./ct_return +p$i 100 1 >> ct_return_mode_task_100_aux.dat; done
cat ct_return_mode_task_100_aux.dat | grep Join >> ct_return_mode_task_100.dat
for i in {1..72}; do ./ct_return +p$i 1000 1 >> ct_return_mode_task_1000_aux.dat; done
cat ct_return_mode_task_1000_aux.dat | grep Join >> ct_return_mode_task_1000.dat
for i in {1..72}; do ./ct_return +p$i 2000 1 >> ct_return_mode_task_2000_aux.dat; done
cat ct_return_mode_task_2000_aux.dat | grep Join >> ct_return_mode_task_2000.dat


for i in {1..72}; do ./ct_return +p$i 100 500 >> ct_return_mode_bigtask_100_aux.dat; done
cat ct_return_mode_bigtask_100_aux.dat | grep Join >> ct_return_mode_bigtask_100.dat
for i in {1..72}; do ./ct_return +p$i 1000 500 >> ct_return_mode_bigtask_1000_aux.dat; done
cat ct_return_mode_bigtask_1000_aux.dat | grep Join >> ct_return_mode_bigtask_1000.dat
for i in {1..72}; do ./ct_return +p$i 2000 500 >> ct_return_mode_bigtask_2000_aux.dat; done
cat ct_return_mode_bigtask_2000_aux.dat | grep Join >> ct_return_mode_bigtask_2000.dat

