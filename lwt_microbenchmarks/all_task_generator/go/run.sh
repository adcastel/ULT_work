#/bin/bash

rm *.dat

for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 100 1 >> go_task_100.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 1000 1 >> go_task_1000.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 2000 1 >> go_task_2000.dat; done

for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 100 500 >> go_bigtask_100.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 1000 500 >> go_bigtask_1000.dat; done
for i in {1..72}; do  export GOMAXPROCS=$i; ./go_task 2000 500 >> go_bigtask_2000.dat; done
