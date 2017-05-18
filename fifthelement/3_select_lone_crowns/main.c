#include<stdio.h>
#include<omp.h>
#include<gdal.h>
#include"arrays.h"

#define MAXARRAYSIZE 100000

void usage()
{
	printf( "-----------------------------------------\n");
	printf( "--Quickbird Processing chain--openMP code----\n");
	printf( "-----------------------------------------\n");
	printf( "./select_merged_crowns in out.csv\n");
	printf( "-----------------------------------------\n");
	printf( "in: input raster file\n");
	printf(	"out: output csv file with clumps having < 200 count\n");
	return;
}

int main(int argc, char * argv[])
{
      	if( argc < 2 ) {
		usage();
		return(EXIT_FAILURE);
	}
	//for four.tif test image
	//const int NODATA=255;
	//for clump.tif test image
	//const int NODATA=-2147483648;
	//For chain process
	const int NODATA=-1; 
	int err=0,row=0,col=0;
	if(err==0) err=1;
	int errMax=0,val;
	if(errMax==0) errMax=1;
	double clumpValMinMax[2];/*Min then Max*/
	//Loading the input/output files names
	char *in = argv[1];
	char *out = argv[2];
	FILE *csvfile;
	csvfile=fopen(out,"w");
	int i,array[MAXARRAYSIZE];

	GDALAllRegister();
	GDALDatasetH hD = GDALOpen(in,GA_ReadOnly);
	if(hD==NULL){
		printf("Unable to load input file\n");
		return(EXIT_FAILURE);
	}
	GDALRasterBandH hB = GDALGetRasterBand(hD,1);
	/*Get clump max number in the image*/
	clumpValMinMax[0] = GDALGetRasterMinimum(hB,&errMax);
	clumpValMinMax[1] = GDALGetRasterMaximum(hB,&errMax);
	if(!(errMax)){
		GDALComputeRasterMinMax(hB,TRUE, clumpValMinMax);
	}
	int nX 	= GDALGetRasterBandXSize(hB);
	int nY 	= GDALGetRasterBandYSize(hB);
	/*from GDAL API get max value*/
	int count=0; /*for clump members counting*/
	int ccount=0; /*for reporting array index*/
	const int max=clumpValMinMax[1];
	const int min=clumpValMinMax[0];
	float **map = af2d(nX,nY);
	for(col=0;col<nX;col++){
		err=GDALRasterIO(hB,GF_Read,col,0,1,nY,map[col],1,nY,GDT_Float32,0,0);
	}
	#pragma omp parallel for default(none) \
		shared(nY,nX,map,csvfile,array,ccount) \
		private(col,row,count)
	for(val=min;val<max;val++){
		count=0;
		for(row=0;row<nY-2;row++){
			for(col=1;col<nX-1;col++){
				if((int)map[col][row+1]==val){
					if(map[col][row]==NODATA
					||map[col][row+2]==NODATA
					||map[col-1][row]==NODATA
					||map[col+1][row]==NODATA){
						count++;
					}
				}
			}
		}
		if (count > 3 && count < 200){
			array[ccount]=(int) val;
			ccount++;
		}
	}
	for (i=0;i<ccount;i++){
		fprintf(csvfile,"%d\n",(int) array[i]);
	}
	fclose(csvfile);	
	GDALClose(hD);
	return(EXIT_SUCCESS);
}
