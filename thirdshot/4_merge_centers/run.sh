#!/bin.bash

base=$(pwd)
cd ../
root=$(pwd)

inputPath=$root/dataOut/3_circles
outputPath=$root/dataOut/4_merge_centers
mkdir -p $outputPath

#Go to input directory
cd $inputPath
echo $inputPath
ls -aslh

for file in *.csv
do
	python3 $base/merge_centers.py $inputPath/$file 2.0 $outputPath/$file
done

