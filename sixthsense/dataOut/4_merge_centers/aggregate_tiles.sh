#!/bin/bash

mkdir -p ../results

for file in 2015_zona1 2015_zona2 2016_zona1 2016_zona2
do
	list=$(ls $file*)
	echo "clump,X,Y" > ../results/4_merged_$file.csv
	for infile in $list
	do
		cat $infile | tail -n+2 >> ../results/4_merged_$file.csv
	done
done
