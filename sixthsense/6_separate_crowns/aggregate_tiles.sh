#!/bin/bash
base=$(pwd)
cd ../
root=$(pwd)

mkdir -p $root/dataOut/results
cd $root/dataOut/6_separate_crowns


for file in 2015_zona1 2015_zona2 2016_zona1 2016_zona2
do
	list=$(ls merged_$file*)
	echo "clump,X,Y" > $root/dataOut/results/6_merged_$file.csv
	for infile in $list
	do
		cat $infile | tail -n+2 >> $root/dataOut/results/6_merged_$file.csv
	done
done
