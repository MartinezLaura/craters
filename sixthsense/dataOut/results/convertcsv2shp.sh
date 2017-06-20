#!/bin/bash


#process the csv to shp conversion
for file in *.csv
do
	#create dbf
	ogr2ogr -f "ESRI Shapefile" . $file
	srslayer=$(echo $file | sed 's/.csv//')
	vrt=$(echo $file | sed 's/.csv/.vrt/')
	#create .vrt
	echo "<OGRVRTDataSource>" > $vrt
	echo "  <OGRVRTLayer name=\"output\">" >> $vrt
	echo "      <SrcDataSource relativeToVRT=\"1\">.</SrcDataSource>" >> $vrt
	echo "      <SrcLayer>$srslayer</SrcLayer>" >> $vrt
	echo "      <GeometryType>wkbPoint</GeometryType>" >> $vrt
	echo "      <LayerSRS>EPSG:32633</LayerSRS>" >> $vrt
	echo "      <GeometryField encoding=\"PointFromColumns\" x=\"X\" y=\"Y\"/>" >> $vrt
	echo "  </OGRVRTLayer>" >> $vrt
	echo "</OGRVRTDataSource>" >> $vrt
	#Attach .vrt to data from .dbf
	ogr2ogr -f "ESRI Shapefile" . $vrt
done

#Intersect with olive fields
for file in *.vrt
do
	mask="uliveti_puglia.shp"
	output=$(echo $file | sed 's/.vrt//')
	ogr2ogr -clipsrc $mask clip_$output $file
done
