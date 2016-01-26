#!/bin/bash


DIR="argobots qthreads massivethreads gccomp iccomp go conversethreads"

for d in $DIR
do

echo $d
cd $d
./run.sh
cd ..
done

