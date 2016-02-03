#/bin/bash


for f in $(ls *.dat)
do
echo "$f"
cut -d" " -f4 $f
read
done

