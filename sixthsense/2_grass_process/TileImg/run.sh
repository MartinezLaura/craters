#!/bin/bash

row=2
col=4

mkdir -p orig/

for file in *.tif
do
	bash tileimg.sh $file $row $col
	mv $file orig/
done

mv sub_$row\_$col/*.* .
rm -Rf sub_$row\_$col
