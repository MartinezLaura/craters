#!/bin/bash

list=$(ls 4*.csv)

for file in $list
do
	output=$(echo $file | sed 's/.csv//')
	v.in.ascii --overwrite input=$file output=m$output separator=comma skip=1 x=2 y=3 z=4 cat=1
done

list=$(ls 6*.csv)

for file in $list
do
	output=$(echo $file | sed 's/.csv//')
	v.in.ascii --overwrite input=$file output=m$output separator=comma skip=1 x=2 y=3 cat=1
done

#v.to.rast --overwrite input=uliveti_puglia@PERMANENT output=uliveti_puglia use=attr attribute_column=cat

#for file in m4_merged_2015_zona1 m4_merged_2015_zona2 m4_merged_2016_zona1 m4_merged_2016_zona2 m6_merged_2015_zona1 m6_merged_2015_zona2 m6_merged_2016_zona1 m6_merged_2016_zona2
#do
#	v.select --overwrite ainput=$file binput=uliveti_puglia output=s$file operator=within
#	v.out.ogr -e input=s$file output=s$file format=ESRI_Shapefile
#done


