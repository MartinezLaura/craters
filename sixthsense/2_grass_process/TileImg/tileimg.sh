#!/bin/bash
dest=sub_$2\_$3
mkdir -p $dest

nX=$(gdalinfo $1 | grep "Size is" | sed "s/Size is \(.*\), \(.*\)/\1/") 
nY=$(gdalinfo $1 | grep "Size is" | sed "s/Size is \(.*\), \(.*\)/\2/") 
echo "nX="$nX "nY="$nY

total=$(echo $2"*"$3 | bc)
echo "cut in "$2" x "$3"="$total

echo "New X size is" $(echo $nX "* 1.0 /" $2 | bc) 
echo "New Y size is" $(echo $nY "* 1.0 /" $3 | bc) 

#Extension for the output files
count=0
#dimensions of tiles are fixed
xsize=$(echo $nX "* 1.0 /" $2 | bc) 
ysize=$(echo $nY "* 1.0 /" $3 | bc)
tcountX=$2
tcountY=$3

for (( X=0 ; X < tcountX ; X++)) 
do 
	for (( Y=0 ; Y < tcountY ; Y++)) 
	do
		xoff=$(echo $X"*"$xsize|bc)
		yoff=$(echo $Y"*"$ysize|bc)
		echo "gdal_translate -srcwin "$xoff $yoff $xsize $ysize $1 $dest/$(echo $1 | sed "s/.tif//")"_"$count".tif"
		gdal_translate -srcwin $xoff $yoff $xsize $ysize $1 $dest/$(echo $1 | sed "s/.tif//")_$count.tif
		count=$(echo $count"+1" | bc)
	done
done
