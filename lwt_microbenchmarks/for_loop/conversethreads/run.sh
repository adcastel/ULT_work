#/bin/bash

rm *.dat

for i in {1..72}; do ./ct_return 1000 +p$i >> ct_return_mode_nested_100_aux.dat; done
cat ct_return_mode_nested_100_aux.dat | grep Join >> ct_return_mode_for_1000.dat
for i in {1..72}; do ./ct_return 5000000 +p$i >> ct_return_mode_nested_1000_aux.dat; done
cat ct_return_mode_nested_1000_aux.dat | grep Join >> ct_return_mode_for_5000000.dat



