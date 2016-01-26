#/bin/bash

rm *.dat

for i in {1..72}; do ./ct_return +p$i 100 4 1 1 >> ct_return_mode_task_100_4_aux.dat; done
cat ct_return_mode_task_100_4_aux.dat | grep Join >> ct_return_mode_task_100_4.dat
for i in {1..72}; do ./ct_return +p$i 100 10 1 1 >> ct_return_mode_task_100_10_aux.dat; done
cat ct_return_mode_task_100_10_aux.dat | grep Join >> ct_return_mode_task_100_10.dat


