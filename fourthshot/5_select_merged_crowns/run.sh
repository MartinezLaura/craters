#!/bin/bash
#-----------------------------------------
#--Quickbird Processing chain--openMP code----
#-----------------------------------------
#./select_merged_crowns in out.csv
#-----------------------------------------
#in: input raster file
#out: output csv file with clumps having 200-2000 count
#-----------------------------------------

root=$(pwd)
make clean
make
listTif=$(ls *.tif)
echo $listTif
for file in $listTif
do
	outfile=$(echo $file | sed 's/.tif/.csv/')
	time $root/select_merged_crowns $file $outfile
done
