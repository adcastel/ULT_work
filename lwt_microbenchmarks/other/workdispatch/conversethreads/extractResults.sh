#/bin/bash

rm *aux*.dat

for f in $(ls *.dat)
do
echo "$f"
cut -d" " -f3 $f
read
done

