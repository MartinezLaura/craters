#!/bin/bash
#-----------------------------------------
#--Quickbird Processing chain--openMP code----
#-----------------------------------------
#./separate_crowns in inclumpval >> out.csv
#-----------------------------------------
#in: input raster file
#inclumpval: clump value where count is 200-2000
#out: output csv file with clumps having 200-2000 count
#-----------------------------------------

nprocs=$(cat /proc/cpuinfo | grep processor | wc -l)

base=$(pwd)
cd ../
root=$(pwd)

cd $base/
make clean
make

inputPath=$root/dataOut/2_grass_process
inputCSVPath=$root/dataOut/5_select_merged_crowns
outputPath=$root/dataOut/6_separate_crowns
mkdir -p $outputPath

#Go to input directory
cd $inputPath
echo $inputPath
ls -aslh

#list input TIF files
listTif=$(ls *.tif)
echo $listTif

#Relocate to $root/dataOut/5_select_merged_crowns
cd $inputCSVPath

#Start process for each TIF
for file in $listTif
do
	CSVfile=$(echo $file | sed 's/.tif/.csv/')
#	$root/../5_select_merged_crowns/select_merged_crowns $file $CSVfile
	mkdir -p $outputPath/sep/
	#Start process for each clump for that TIF
	for val in $(cat $inputCSVPath/$CSVfile)
	do
		echo "clump,X,Y" > $outputPath/sep/sep_$val$CSVfile
		$base/separate_crowns $inputPath/$file $val >> $outputPath/sep/sep_$val$CSVfile &
		npids=$(pidof separate_crowns | wc -w)
		while (( $npids > $nprocs ))
		do
			sleep 2
			npids=$(pidof separate_crowns | wc -w)
		done
	done
	mkdir -p $outputPath/tmp/
	#Relocate prompt to SEP dir
	cd $outputPath/sep/
	#Create a main sep file in $outputPath
	echo "clump,X,Y" > $outputPath/sep_$CSVfile
	for file in sep_*$CSVfile
	do
		cat $file | tail -n+2 >> $outputPath/sep_$CSVfile
	done
	#Process sep files into TMP files
	for file in sep_*$CSVfile
	do
		rm -f $outputPath/tmp/tmp_$file
		#if file is more than 2 lines (header + several solutions)
		if (( $(cat $outputPath/sep/$file | wc -l) > 2 ))
		then
			python3 $base/merge_centers.py $file 2.5 $outputPath/tmp/tmp_$file &
			else if (( $(cat $outputPath/sep/$file | wc -l) == 2 ))
			then
				#If file is 2 lines (header + unique solution) don't process
				cp -f $outputPath/sep/$file $outputPath/tmp/tmp_$file
				else if (( $(cat $outputPath/sep/$file | wc -l) == 1 ))
				then
					#If file is 1 line (header only) skip it
					echo  ""
				fi
			fi
		fi
		npids=$(pidof merge_centers.py | wc -w)
		while (( $npids > $nprocs ))
		do
			sleep 2
			npids=$(pidof merge_centers.py | wc -w)
		done
	done
	mkdir -p $outputPath/merged/
	#Relocate prompt to TMP dir
	cd $outputPath/tmp/
	#Create a main tmp file in $root
	echo "clump,X,Y" > $outputPath/tmp_$CSVfile
	for file in tmp_*$CSVfile
	do
		cat $file | tail -n+2 >> $outputPath/tmp_$CSVfile
	done
	#Process tmp files into final MERGED files
	for file in tmp_*$CSVfile
	do
		rm -f $outputPath/merged/merged_$file
		#If file is more than 2 lines (header + several solutions)
		if (( $(cat $outputPath/tmp/$file | wc -l) > 2 ))
		then
			python3 $base/merge_centers.py $file 2.0 $outputPath/merged/merged_$file
		else
			#If file is 2 lines (header + unique solution) don't process
			cp -f $outputPath/tmp/$file $outputPath/merged/merged_$file
		fi
		npids=$(pidof merge_centers.py | wc -w)
		while(($npids > $nprocs))
		do
			sleep 2
			npids=$(pidof merge_centers.py | wc -w)
		done
	done
	cd $outputPath/merged/
	#Create a main merged file in $root
	echo "clump,X,Y" > $outputPath/merged_$CSVfile
	for file in merged_*$CSVfile
	do
		cat $file | tail -n+2 >> $outputPath/merged_$CSVfile
	done
done
#Aggregate tiles together
cd $root/dataOut/6_separate_crowns
bash $base/aggregate_tiles.sh
#Push raw results to user
cd $root/dataOut/6_separate_crowns
bash $base/raw_stats.sh
