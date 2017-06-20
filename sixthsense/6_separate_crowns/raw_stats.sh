#!/bin/bash
base=$(pwd)
cd ../
root=$(pwd)

cd $root/dataOut/results

for file in *.csv
do
	linenumber=$(cat $file | wc -l)
	echo $file ": " $linenumber
done
