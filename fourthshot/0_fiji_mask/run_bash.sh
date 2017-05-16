#!/bin/bash
echo "Run Fiji on the dataset"
base=$(pwd)
cd ../
root=$(pwd)
inputPath=$root/dataIn/
outputPath=$root/dataOut/0_fiji_mask
mkdir -p $outputPath

cd $inputPath
echo $inputPath
ls -aslh
for file in $(ls *.tif)
do
	echo "bash $base/run.sh $inputPath $outputPath $file" 
	bash $base/run.sh $inputPath $outputPath $file 
done

