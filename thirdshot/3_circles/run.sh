#!/bin/bash
nworkers=7
export OMP_NUM_THREADS=$nworkers
set OMP_NUM_THREADS=$nworkers

base=$(pwd)
cd ../
root=$(pwd)

inputPath=$root/dataOut/2_grass_process
outputPath=$root/dataOut/3_circles
mkdir -p $outputPath

#Compile locally the circles code
cd $base/circles
make clean
make

#Go to input directory
cd $inputPath
echo $inputPath
ls -aslh

for file in *.tif
do
	gfile=$(echo $file | sed 's/.tif//')
	$base/circles/circles $inputPath/$file > $outputPath/$gfile.csv
done
