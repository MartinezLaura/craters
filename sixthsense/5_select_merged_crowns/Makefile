select_merged_crowns: main.c arrays.c
	gcc -o select_merged_crowns main.c arrays.c -g -I/usr/include/gdal/ -L/usr/lib -lgdal -lm -Wall -fopenmp -O2 -mavx

clean:
	rm -f select_merged_crowns
