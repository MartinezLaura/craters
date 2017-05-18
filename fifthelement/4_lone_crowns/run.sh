#!/bin/bash

#-----------------------------------------
#--Quickbird Processing chain--openMP code----
#-----------------------------------------
#./lone_crowns in inclumpval >> out.csv
#-----------------------------------------
#in: input raster file
#inclumpval: clump value where count is < 200
#out: output csv file with clumps having < 200 count
#-----------------------------------------

#Get number of available processors
nprocs=$(cat /proc/cpuinfo | grep processor | wc -l)

#get working directory info
base=$(pwd)
cd ../
root=$(pwd)

#Get input paths and create output path
inputPath=$root/dataOut/2_grass_process
inputCSVPath=$root/dataOut/3_select_lone_crowns
outputPath=$root/dataOut/4_lone_crowns
mkdir -p $outputPath

#Compile locally the code
cd $base
make clean
make

#Go to input directory
cd $inputPath
echo $inputPath
ls -aslh

#Process
for file in *.tif
do
	#Create a CSV file name from tif name
	outfile=$(echo $file | sed 's/.tif/.csv/')
	#loop through the clump values in the file
	for val in $(cat $inputCSVPath/$outfile)
	do
		echo "clump,X,Y,radius" > $outputPath/lone_$val$outfile
		$base/lone_crowns $inputPath/$file $val >> $outputPath/lone_$val$outfile &
		npids=$(pidof lone_crowns | wc -w)
		while (( $npids > $nprocs ))
		do
			sleep 1
			npids=$(pidof lone_crowns | wc -w)
		done
	done
done
