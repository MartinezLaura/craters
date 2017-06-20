#!/bin/bash
base=$(pwd)
cd ../
root=$(pwd)
inputPath=$root/dataIn/
outputPath=$root/dataOut/0_fiji_mask
mkdir -p $outputPath

cd $inputPath
echo $inputPath
ls -aslh
for file in $(ls 201[56]_zona[12].tif)
do
	echo "bash $base/run.sh $inputPath $outputPath $file" 
	bash $base/run.sh $inputPath $outputPath $file 
done

