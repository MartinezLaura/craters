#include<stdio.h>
#include<gdal.h>
#include<omp.h>
#include"rs.h"
#include"arrays.h"

#define ARRAY_SIZE 10000 
#define THRSHLD 1
#define DIST2NN 0

void usage()
{
	printf( "-----------------------------------------\n");
	printf( "--Quickbird Processing chain--openMP code----\n");
	printf( "-----------------------------------------\n");
	printf( "./separate_crowns in inclumpval >> out.csv\n");
	printf( "-----------------------------------------\n");
	printf( "in: input raster file\n");
	printf(	"inclumpval: clump value where count is 200-2000\n");
	printf(	"out: output csv file with clumps having 200-2000 count\n");
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
	int i,j,errMax=0;
	if(errMax==0) errMax=1;
	//Loading the input/output files names
	char *in = argv[1];
	int val = atoi(argv[2]);
	GDALAllRegister();
	GDALDatasetH hD = GDALOpen(in,GA_ReadOnly);
	if(hD==NULL){
		printf("Unable to load input file\n");
		return(EXIT_FAILURE);
	}
	GDALRasterBandH hB = GDALGetRasterBand(hD,1);
	int nX 	= GDALGetRasterBandXSize(hB);
	int nY 	= GDALGetRasterBandYSize(hB);
	double 	geomx[6]={0.0};
	if(GDALGetGeoTransform(hD,geomx)==CE_None){
		//printf( "Origin (ULx,ULy) = (%.6f,%.6f)\n", geomx[0], geomx[3] );
		//printf( "Pixel Size = (%.6f,%.6f)\n", geomx[1], geomx[5] );
		//printf( "Rot0 = (%.6f,%.6f)\n", geomx[2], geomx[4] );
		//printf("lon=%f\n", geomx[0]+geomx[1]*col+geomx[2]*row);
		//printf("lat=%f\n", geomx[3]+geomx[4]*col+geomx[5]*row);
	}
	/*from GDAL API get max value*/
	int count=0;
	float **map = af2d(nX,nY);
	for(col=0;col<nX;col++){
		err=GDALRasterIO(hB,GF_Read,col,0,1,nY,map[col],1,nY,GDT_Float32,0,0);
	}
	int ar_row_max=0;
	int ar_row_min=0;
	int ar_col_max=0;
	int ar_col_min=0;
	int ar_nX,ar_nY;
	int s2m_col,s2m_row;
	double lat,lon;
	/* Declare geoloc storage arrays */
	int *ar_col =  ai1d(ARRAY_SIZE);
	int *ar_row =  ai1d(ARRAY_SIZE);
	for(j=0;j<ARRAY_SIZE;j++){
		ar_col[j]=0.0;
		ar_row[j]=0.0;
	}	
	/*loop through the set of 3 pixels in the clumps*/
	/*1 - create a(x,y) array for all pix in clump*/
	/*Extract all pixel locations for a given clump*/
	count=0;
	for(row=0;row<nY-2;row++){
		//printf("clump %d, count %d @ %d/%d\n",(int) val,count,row,nY);
		for(col=1;col<nX-1;col++){
			/* If clump member is found add it to array */
			if((int)map[col][row+1]==val){
				/* Only if one at least of its 4 neighbours is NODATA */
				if(map[col][row]==NODATA
				||map[col][row+2]==NODATA
				||map[col-1][row]==NODATA
				||map[col+1][row]==NODATA){
					ar_col[count]=col;
					ar_row[count]=row;
					/*gather boundary box coordinates*/
					if(ar_col_max<col) ar_col_max=col+1;
					if(ar_col_min>col) ar_col_min=col-1;
					if(ar_row_max<row) ar_row_max=row+1;
					if(ar_row_min>row) ar_row_min=row-1;
				}
			}
		}
	}
	/*Create the subset image from boundary info*/
	ar_nX=ar_col_max-ar_col_min;
	ar_nY=ar_row_max-ar_row_min;
	int **subin=ai2d(ar_nX+1, ar_nY+1);
	/*Create a temporary sub image*/
	int **subout=ai2d(ar_nX+1, ar_nY+1);
	/*Fill images with zero*/
	for(col=0;col<ar_nX+1;col++){
		for(row=0;row<ar_nY+1;row++){
			subin[col][row]=0;
			subout[col][row]=0;
		}
	}
	/*Closing Operator is Dilation+Erosion ( @ -> O )*/
	for(col=ar_col_min;col<=ar_col_max;col++){
		for(row=ar_row_min;row<=ar_row_max;row++){
			subin[col-ar_col_min][row-ar_row_min]=map[col][row];
		}
	}
	/*Closing Operator is Dilation+Erosion ( @ -> O )*/
	/*Repeat 10 times to remove inner islands*/
	for(i=0;i<4;i++){
		/*1 - Load the sub image to the dilation function*/
		subout=dilation(subin,ar_nX+1,ar_nY+1,val,NODATA,THRSHLD);
		for(col=0;col<ar_nX+1;col++){
			for(row=0;row<ar_nY+1;row++){
				subin[col][row]=subout[col][row];
			}
		}
	}
	for(i=0;i<5;i++){
		/*1 - Load the sub image to the dilation function*/
		subout=dilation(subin,ar_nX+1,ar_nY+1,val,NODATA,THRSHLD);
		for(col=0;col<ar_nX+1;col++){
			for(row=0;row<ar_nY+1;row++){
				subin[col][row]=subout[col][row];
			}
		}
		/*2 - Load the sub image to the erosion function*/
		subout=mm_erode(subin,ar_nX+1,ar_nY+1,val,NODATA,DIST2NN);
		for(col=0;col<ar_nX+1;col++){
			for(row=0;row<ar_nY+1;row++){
				subin[col][row]=subout[col][row];
			}
		}
	}/*END OF REPEAT CLOSING OPERATOR*/
	/*Erosion function will leave only a point at the center of masses ( O -> . )*/
	/*Repeat 10 times to reduce mass to point*/
	for(i=0;i<10;i++){
		/*Load the sub image to the erosion function*/
		subout=mm_erode(subin,ar_nX+1,ar_nY+1,val,NODATA,DIST2NN);
		for(col=0;col<ar_nX+1;col++){
			for(row=0;row<ar_nY+1;row++){
				subin[col][row]=subout[col][row];
			}
		}
	}/*END OF REPEAT EROSION FUNCTION*/
	/*export the lone pixels to STDOUT*/
	for(col=0;col<ar_nX+1;col++){
		for(row=0;row<ar_nY+1;row++){
			if(subin[col][row]==val){
				/* convert from sub to map row/col */
				s2m_col=col+ar_col_min;
				s2m_row=row+ar_row_min;
				lon=geomx[0]+geomx[1]*s2m_col+geomx[2]*s2m_row+0.5*geomx[1];
				lat=geomx[3]+geomx[4]*s2m_col+geomx[5]*s2m_row+0.5*geomx[5];
				printf("%d,%f,%f\n",val,lon,lat);
			}
		}
	}
	/*Free Memory*/
	if( ar_col != NULL) free ( ar_col );
	if( ar_row != NULL) free ( ar_row );
	ff2d(map,nX);
	GDALClose(hD);
	return(EXIT_SUCCESS);
}
