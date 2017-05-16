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

root=$(pwd)
make clean
make
for file in *.tif
do
	outfile=$(echo $file | sed 's/.tif/.csv/')
#	$root/../5_select_merged_crowns/select_merged_crowns $file $outfile
	mkdir -p $root/sep/
	for val in $(cat $outfile)
	do
		echo "clump,X,Y" > $root/sep/sep_$val$outfile
		$root/separate_crowns $file $val >> $root/sep/sep_$val$outfile &
		npids=$(pidof separate_crowns | wc -w)
		while(($npids > $nprocs))
		do
			sleep 2
			npids=$(pidof separate_crowns | wc -w)
		done
	done
	mkdir -p tmp/
	#Relocate prompt to SEP dir
	cd $root/sep/
	#Create a main sep file in $root
	echo "clump,X,Y" > $root/sep_$outfile
	for file in sep_*$outfile
	do
		cat $file | tail -n+2 >> $root/sep_$outfile
	done
	#Process sep files into TMP files
	for file in sep_*$outfile
	do
		rm -f $root/tmp/tmp_$file
		#if file is more than 2 lines (header + several solutions)
		if (( $(cat $root/sep/$file | wc -l) > 2 ))
		then
			python3 $root/merge_centers.py $file 2.5 $root/tmp/tmp_$file &
			else if (( $(cat $root/sep/$file | wc -l) == 2 ))
			then
				#If file is 2 lines (header + unique solution) don't process
				cp -f $file $root/tmp/tmp_$file
				else if (( $(cat $root/sep/$file | wc -l) == 1 ))
				then
					#If file is 1 line (header only) skip it
					echo  ""
				fi
			fi
		fi
		npids=$(pidof merge_centers.py | wc -w)
		while(($npids > $nprocs))
		do
			sleep 2
			npids=$(pidof merge_centers.py | wc -w)
		done
	done
	mkdir -p $root/merged/
	#Relocate prompt to TMP dir
	cd $root/tmp/
	#Create a main tmp file in $root
	echo "clump,X,Y" > $root/tmp_$outfile
	for file in tmp_*$outfile
	do
		cat $file | tail -n+2 >> $root/tmp_$outfile
	done
	#Process tmp files into final MERGED files 
	for file in tmp_*$outfile
	do
		rm -f $root/merged/merged_$file
		#If file is more than 2 lines (header + several solutions)
		if (( $(cat $root/tmp/$file | wc -l) > 2 )) 
		then
			python3 $root/merge_centers.py $file 2.0 $root/merged/merged_$file 
		else
			#If file is 2 lines (header + unique solution) don't process
			cp -f $root/tmp/$file $root/merged/merged_$file
		fi
		npids=$(pidof merge_centers.py | wc -w)
		while(($npids > $nprocs))
		do
			sleep 2
			npids=$(pidof merge_centers.py | wc -w)
		done
	done
	cd $root/merged/
	#Create a main merged file in $root
	echo "clump,X,Y" > $root/merged_$outfile
	for file in merged_*$outfile
	do
		cat $file | tail -n+2 >> $root/merged_$outfile
	done
done

