#!/usr/bin/env bash 
sum=0
nf=$(awk 'END { print NF }' < input.txt)
for ((i=1; i<=$nf; i++))
do
    result=$(awk '{ printf "%s\t", $"'$i'"}' < input.txt | awk 'END { for (i = 1; i <= NF - 2; i++) { printf "%s%s", $i, $NF } ; printf "%s\n", $(NF - 1) }' | bc -lq)
    sum=$(($sum + $result))
done
echo $sum
