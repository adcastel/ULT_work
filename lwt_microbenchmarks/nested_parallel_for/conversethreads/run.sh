#/bin/bash

rm *.dat

for i in {1..36}; do ./ct_return +p$i 100 >> ct_return_mode_nested_100_aux.dat; done
cat ct_return_mode_nested_100_aux.dat | grep Join >> ct_return_mode_nested_100.dat
for i in {1..36}; do ./ct_return +p$i 1000 >> ct_return_mode_nested_1000_aux.dat; done
cat ct_return_mode_nested_1000_aux.dat | grep Join >> ct_return_mode_nested_1000.dat



