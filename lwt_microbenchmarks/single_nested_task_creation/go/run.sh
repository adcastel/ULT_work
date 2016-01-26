#/bin/bash

rm *.dat

for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 100 4 1 1 >> go_task_100_4.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 100 10 1 1>> go_task_100_10.dat; done
