#!/bin/bash
#################################################################
# Dockerfile main script
#################################################################
# Input arguments
#$1: Input Path for the original Tile to process
#$2: Output Path for the watershed of tree crowns made by Fiji 
#$3: InputFile name to be processed (r.g.b.nir, .tif)
#################################################################
: ${1?"Usage: $0 inputPath outputPath inputFile.tif"}
#################################################################
# Redirect GUI to Virtual Frame Buffer
Xvfb :1 -screen 0 1024x768x16 &> xvfb.log  &
export DISPLAY=:1.0
#################################################################
# PART 1: FIJI/ImageJ2
#################################################################
#-----------------------------------------------------------------
# For this script to work, 
# the file "proto.ijm" should be in the same dir
#-----------------------------------------------------------------
#Create Macro file
IJMbase=$1/../0_fiji_mask
echo "setBatchMode(true);" > $IJMbase/ijm.ijm 
echo "run(\"Bio-Formats Importer\", \"open=$1/$3 color_mode=Grayscale view=Hyperstack stack_order=XYCZT\");" >> $IJMbase/ijm.ijm
cat $IJMbase/proto.ijm >> $IJMbase/ijm.ijm
echo "saveAs(\"Tiff\", \"$2/$3\");" >> $IJMbase/ijm.ijm 
echo "run(\"Close\");" >> $IJMbase/ijm.ijm
echo "run(\"Quit\");" >> $IJMbase/ijm.ijm
#Output directory
mkdir -p $2
#Process The batch
$HOME/Fiji.app/ImageJ-linux64 -macro $IJMbase/ijm.ijm 
#Post-Process the images, adding images extents and projsys
prjsys=$(gdalinfo $1/$3 | grep EPSG | tail -n1 | sed "s/\ //g" | sed "s/AUTHORITY\[\"//" | sed "s/\",\"/:/" | sed "s/\"\]\]//")
ulx=$(gdalinfo $1/$3 | grep "Upper Left" | sed "s/\ //g" | sed "s/UpperLeft(//" | sed "s/\(.*\))(\(.*\)/\1/" | sed "s/\(.*\),\(.*\)/\1/")
uly=$(gdalinfo $1/$3 | grep "Upper Left" | sed "s/\ //g" | sed "s/UpperLeft(//" | sed "s/\(.*\))(\(.*\)/\1/" | sed "s/\(.*\),\(.*\)/\2/")
lrx=$(gdalinfo $1/$3 | grep "Lower Right" | sed "s/\ //g" | sed "s/LowerRight(//" | sed "s/\(.*\))(\(.*\)/\1/" | sed "s/\(.*\),\(.*\)/\1/")
lry=$(gdalinfo $1/$3 | grep "Lower Right" | sed "s/\ //g" | sed "s/LowerRight(//" | sed "s/\(.*\))(\(.*\)/\1/" | sed "s/\(.*\),\(.*\)/\2/")
echo "gdal_edit.py -a_srs $prjsys -a_ullr $ulx $uly $lrx $lry $2/$3"
gdal_edit.py -a_srs $prjsys -a_ullr $ulx $uly $lrx $lry $2/$3
#################################################################
