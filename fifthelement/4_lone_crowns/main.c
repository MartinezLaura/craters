#include<stdio.h>
#include<gdal.h>
#include<omp.h>
#include"rs.h"
#include"arrays.h"

#define ARRAY_SIZE 202 
#define STATS_ON 0 

void usage()
{
	printf( "-----------------------------------------\n");
	printf( "--Quickbird Processing chain--openMP code----\n");
	printf( "-----------------------------------------\n");
	printf( "./lone_crowns in inclumpval >> out.csv\n");
	printf( "-----------------------------------------\n");
	printf( "in: input raster file\n");
	printf(	"inclumpval: clump value where count is < 200\n");
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
	int j,k,m,n,errMax=0;
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
	int count=0;
	float **map = af2d(nX,nY);
	for(col=0;col<nX;col++){
		err=GDALRasterIO(hB,GF_Read,col,0,1,nY,map[col],1,nY,GDT_Float32,0,0);
	}
	int ccount=0;
	int radmaxccount=0;
	int radmax=0;
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
	#pragma omp parallel for default(none)\
		shared(nY,nX,map,val,ar_row,ar_col,count) \
		private(row,col)
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
					count++;
				}
			}
		}
	}
	/*fprintf(fp,"clump,X,Y,radius\n");*/
	/*2 - create 3x loop to process all pixels into cir3pts()*/
	/* and collect info (x,y,rd) into one output array*/
	int **center =  ai2d(1*count*count*count, 3);
	for(j=0;j<1*count*count*count;j++){
		for(k=0;k<3;k++){
			center[j][k]=0;
		}
	}
	ccount=0;
	radmaxccount=0;
	radmax=0;
	#pragma omp parallel for default(none) \
		shared(count,ccount,ar_col,ar_row,center) \
		private(j,m,n,radmax,radmaxccount)
	for(j=0;j<count;j++){
	    for(m=j+1;m<count;m++){
		for(n=m+1;n<count;n++){
		    /*printf("%d %d %d\n",j,m,n);*/
		    if(cir3pts(ar_col[j],ar_row[j],ar_col[m],ar_row[m],ar_col[n],ar_row[n],0)!=0){
			/*printf("%d %d %d\n",j,m,n);*/
			center[ccount][0]=cir3pts(ar_col[j],ar_row[j],ar_col[m],ar_row[m],ar_col[n],ar_row[n],0);
			center[ccount][1]=cir3pts(ar_col[j],ar_row[j],ar_col[m],ar_row[m],ar_col[n],ar_row[n],1);
			center[ccount][2]=cir3pts(ar_col[j],ar_row[j],ar_col[m],ar_row[m],ar_col[n],ar_row[n],2);
			if(center[ccount][2]>radmax){
				radmax=center[ccount][2];
				radmaxccount=ccount;
			}
			ccount++;
		    }
		}
   	    }
	}
	col=center[radmaxccount][0];
	row=center[radmaxccount][1];
	lon=geomx[0]+geomx[1]*col+geomx[2]*row+0.5*geomx[1];
	lat=geomx[3]+geomx[4]*col+geomx[5]*row+0.5*geomx[5];
	printf("%d,%f,%f,%f\n",(int)val,lon,lat,geomx[1]*center[radmaxccount][2]/1000.0);
	/*Free Memory*/
	for (j=0;j<(1*count*count*count);j++){
			free(center[j]);
	}
	if( center != NULL) free( center );
	if( ar_col != NULL) free ( ar_col );
	if( ar_row != NULL) free ( ar_row );
	ff2d(map,nX);
	GDALClose(hD);
	return(EXIT_SUCCESS);
}
