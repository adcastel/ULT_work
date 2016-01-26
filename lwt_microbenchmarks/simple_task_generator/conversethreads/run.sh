#/bin/bash

rm *.dat

for i in {1..72}; do ./ct_return +p$i 100 1 >> ct_return_mode_nested_100_aux.dat; done
cat ct_return_mode_nested_100_aux.dat | grep Join >> ct_return_mode_task_100.dat
for i in {1..72}; do ./ct_return +p$i 1000 1 >> ct_return_mode_nested_1000_aux.dat; done
cat ct_return_mode_nested_1000_aux.dat | grep Join >> ct_return_mode_task_1000.dat

for i in {1..72}; do ./ct_return +p$i 100 500 >> ct_return_mode_nested_100_aux.dat; done
cat ct_return_mode_nested_100_aux.dat | grep Join >> ct_return_mode_btask_100.dat
for i in {1..72}; do ./ct_return +p$i 1000 500 >> ct_return_mode_nested_1000_aux.dat; done
cat ct_return_mode_nested_1000_aux.dat | grep Join >> ct_return_mode_btask_1000.dat


