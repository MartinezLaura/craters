#!/bin/bash
base=$(pwd)
cd ../
root=$(pwd)

inputPath=$root/dataOut/1_fillpx
outputPath=$root/dataOut/2_grass_process
mkdir -p $outputPath
#Generate GISRC
mkdir -p $outputPath/grassdata

cd $inputPath
echo $inputPath
ls -aslh
#Process gap filling with mask/fillpx
for file in $(ls *.tif)
do
	gbase=$outputPath/grassdata/tmplocation
	grass73 --text -c $inputPath/$file $gbase -e
	gbaseP=$outputPath/grassdata/tmplocation/PERMANENT
	gfile=$(echo $file | sed 's/.tif//')
	grass73 $gbaseP --exec r.in.gdal --overwrite input=$file output=$gfile
	grass73 $gbaseP --exec r.null --overwrite map=$gfile setnull=255
#	#Remove polygons smaller than 2m2, try to make inner island independent
	grass73 $gbaseP --exec r.to.vect -s -v --overwrite input=$gfile output=v_$gfile type=area
	grass73 $gbaseP --exec v.clean -c --overwrite input=v_$gfile output=clean_$gfile tool=rmbridge,chbridge,rmarea threshold=0,1.0,4
#	#Assign area to each tree crown
	grass73 $gbaseP --exec v.to.rast --overwrite input=clean_$gfile output=clean_$gfile use=cat memory=3000
#	#Give each separate area a single ID
	grass73 $gbaseP --exec r.clump -d --overwrite input=clean_$gfile output=clumped_$gfile title="from filled tree crowns" threshold=0.5 minsize=16
	#Remove the last clump (background value)
	value=$(grass73 $gbaseP --exec r.clump -g input=clean_$gfile threshold=0.5 minsize=16)
	#Write value as ../3_circles/maxval.txt
	echo $value > $outputPath/maxval_$gfile.txt
	#Set NULL values from clump maxval
	grass73 $gbaseP --exec r.null map=clumped_$gfile setnull=$value
	#Remove areas less than 15 (pixels?)
	grass73 $gbaseP --exec r.area --overwrite input=clumped_$gfile output=area_$gfile lesser=15
	#Clump again to renumber areas
	grass73 $gbaseP --exec r.clump -d --overwrite input=area_$gfile output=clumped_area_$gfile
	#Output to dataOut dir
	grass73 $gbaseP --exec r.out.gdal --overwrite input=clumped_area_$gfile output=$outputPath/$file type=Int32 nodata=-1
#	#clean up by destroying GRASS tmp location
	rm -Rf $outputPath/grassdata/tmplocation
	#mv  $outputPath/grassdata/tmplocation $outputPath/grassdata/tmp_$gfile
done

#clean up by destroying the grassdata directory
rm -Rf $outputPath/grassdata

