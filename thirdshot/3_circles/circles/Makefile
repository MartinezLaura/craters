circles: main.c rs.c arrays.c
	gcc -o circles main.c rs.c arrays.c -I/usr/include/gdal/ -L/usr/lib -lgdal -lm -Wall -fopenmp -O2 -mavx

clean:
	rm -f circles
