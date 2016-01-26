#/bin/bash

rm *.dat

for i in {1..72}; do  export GOMAXPROCS=$i; ./go_for_loop 1000 >> go_for_1000.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_for_loop 5000000 >> go_for_5000000.dat; done
