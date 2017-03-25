#!/bin/bash

base=$(pwd)
cd ../
root=$(pwd)

inputPath=$root/dataOut/2_grass_process
outputPath=$root/dataOut/5_separate_crowns
mkdir -p $outputPath

#Compile locally the circles code
cd $base/separate_crowns
make clean
make

#Go to input directory
cd $inputPath
echo $inputPath
ls -aslh

for file in *.tif
do
	echo $file
	gfile=$(echo $file | sed 's/.tif/.csv/')
	time $base/5_separate_crowns/separate_crowns $inputPath/$file > $outputPath/$gfile
	#time $base/5_separate_crowns/separate_crowns $inputPath/$file 
	rm -f $outputPath/tmp_$outfile
	rm -f $outputPath/merged_$outfile
	python3 $base/5_separate_crowns/merge_centers.py $outputPath/$outfile 2.5 $outputPath/tmp_$outfile
	python3 $base/5_separate_crowns/merge_centers.py $outputPath/tmp_$outfile 2.0 $outputPath/merged_$outfile
done
