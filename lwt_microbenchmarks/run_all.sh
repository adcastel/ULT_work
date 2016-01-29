#!/bin/bash


#DIR="all_task_generator  for_loop  nested_parallel_for simple_task_generator  single_nested_task_creation"
DIR="simple_task_generator  single_nested_task_creation"

for d in $DIR
do

echo $d
cd $d
./run_all.sh
cd ..
done

