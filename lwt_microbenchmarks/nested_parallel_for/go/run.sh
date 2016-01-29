#/bin/bash

rm *.dat

for i in {1..36}; do  export GOMAXPROCS=$i; ./go_nested 100 >> go_nested_100.dat; done
for i in {1..36}; do  export GOMAXPROCS=$i; ./go_nested 1000 >> go_nested_1000.dat; done
