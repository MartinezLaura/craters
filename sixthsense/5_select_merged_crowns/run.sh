#!/bin/bash

#-----------------------------------------
#--Quickbird Processing chain--openMP code----
#-----------------------------------------
#./select_merged_crowns in out.csv
#-----------------------------------------
#in: input raster file
#out: output csv file with clumps having 200-2000 count
#-----------------------------------------

nprocs=$(cat /proc/cpuinfo | grep processor | wc -l)

base=$(pwd)
cd ../
root=$(pwd)

inputPath=$root/dataOut/2_grass_process
outputPath=$root/dataOut/5_select_merged_crowns
mkdir -p $outputPath

#Go to input directory
cd $inputPath
echo $inputPath
ls -aslh

listTif=$(ls *.tif)
echo $listTif

for file in *.tif
do
	outfile=$(echo $file | sed 's/.tif/.csv/')
	$base/select_merged_crowns $inputPath/$file $outputPath/$outfile
	npids=$(pidof select_merged_crowns | wc -w)
	while (( $npids > $nprocs ))
	do
		sleep 2
		npids=$(pidof select_merged_crowns | wc -w)
	done
done
