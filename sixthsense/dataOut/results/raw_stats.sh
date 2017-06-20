#!/bin/bash

for file in *.csv
do
	linenumber=$(cat $file | wc -l)
	echo $file ": " $linenumber
done

