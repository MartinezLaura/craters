#!/bin/bash
#-----------------------------------------
#--Quickbird Processing chain--openMP code----
#-----------------------------------------
#./select_lone_crowns in out.csv
#-----------------------------------------
#in: input raster file
#out: output csv file with clumps having < 200 count
#-----------------------------------------

#Get path
base=$(pwd)
cd ../
root=$(pwd)

#Clean and compile executable
cd $base
make clean
make

#Get input path and generate output path
inputPath=$root/dataOut/2_grass_process
outputPath=$root/dataOut/3_select_lone_crowns
mkdir -p $outputPath

#Go to inputPath
cd $inputPath

#Get a list of tif files in inputPath
listTif=$(ls *.tif)
echo $listTif

#Start processing the tif files
for file in $listTif
do
	#Create a CSV file name from tif name
	outfile=$(echo $file | sed 's/.tif/.csv/')
	#Process the creation of clump list values for crowns
	time $base/select_lone_crowns $inputPath/$file $outputPath/$outfile
done
