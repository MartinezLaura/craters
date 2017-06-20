#!/bin/bash

#-----------------------------------------
#--Quickbird Processing chain--openMP code----
#-----------------------------------------
#./circles in > out.csv
#-----------------------------------------
#in: input raster file
#out: output csv file for shapefile creation
#-------------------------------------------------
#PARALLEL  OPENMP (CPU)
nworkers=7
export OMP_NUM_THREADS=$nworkers
set OMP_NUM_THREADS=$nworkers
export GOMP_DEBUG=0
#PARALLEL  OpenACC (GPU)
export ACC_DEVICE_TYPE=nvdia

#./circles three.tif three.csv
root=$(pwd)
listTif=$(ls clump.tif)
echo $listTif
for file in $listTif
do
	outfile=$(echo $file | sed 's/.tif/.csv/')
	$root/circles $file > $outfile
	#$root/circles $file 
done
