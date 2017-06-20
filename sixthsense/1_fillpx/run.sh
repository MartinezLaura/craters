#!/bin/bash
make clean
make

base=$(pwd)
cd ../
root=$(pwd)

inputPath=$root/dataOut/0_fiji_mask
outputPath=$root/dataOut/1_fillpx
mkdir -p $outputPath

cd $inputPath
echo $inputPath
ls -aslh
for file in $(ls 201[56]_zona[12].tif)
do
	echo "$base/fillpx $inputPath/$file $outputPath/$file"
	$base/fillpx $inputPath/$file $outputPath/$file
done
