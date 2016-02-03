#/bin/bash

rm *.dat

for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 100 1 >> go_task_100.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 1000 1 >> go_task_1000.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 100 500 >> go_btask_100.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 1000 500 >> go_btask_1000.dat; done
