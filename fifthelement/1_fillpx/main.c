/*###############################################################################
#
# Project:  Open Source RS Programming Starter Book
# Purpose:  fill V & H one pixel gap
# Author:   Yann Chemin, <yann.chemin@gmail.com>
#
###############################################################################
# Public Domain
###############################################################################*/
#include<stdio.h>
#include<gdal.h>

void usage()
{
	printf( "-----------------------------------------\n");
	printf( "--Landsat Processing chain--Serial code----\n");
	printf( "-----------------------------------------\n");
	printf( "./fillpx inMASK outMERGED\n");
	printf( "-----------------------------------------\n");
	printf( "inMASK is split tree crown mask (output from Fiji)\n");
	printf(	"outMERGED is the mask output were tree crowns have been unsplit [0-1]\n\n");
	return;
}

int main( int argc, char *argv[] )
{
      	if( argc < 2 ) {
		usage();
		return(EXIT_FAILURE);
	}
	int row,col, err=0;
	err=1;
	char	*inB1; 	                /* Input file names */
	char	*maskF;         	/* Output file name */

	inB1 	= argv[1];
	maskF	= argv[2];

	GDALAllRegister();
	GDALDatasetH hD1;	//Read Band
	hD1 = GDALOpen(inB1,GA_ReadOnly);
	if(hD1==NULL){
		printf("Unable to load input file\n");
		exit(EXIT_FAILURE);
	}
	GDALDriverH hDr1 = GDALGetDatasetDriver(hD1);
	GDALDatasetH hDOut1 = GDALCreateCopy( hDr1, maskF,hD1,FALSE,NULL,NULL,NULL);
	GDALRasterBandH hBOut1 = GDALGetRasterBand(hDOut1,1);
	GDALRasterBandH hB1 = GDALGetRasterBand(hD1,1);
	int nX 	= GDALGetRasterBandXSize(hB1);
	int nY 	= GDALGetRasterBandYSize(hB1);
	short int *l1,*l2,*l3,*l4,*l5;//Mask Band row1,2,3
	short int *lOut1,*lOut2,*lOut3,*lOut4; //Output Band row
	l1 = (short int *) malloc(sizeof(short int)*nX);
	l2 = (short int *) malloc(sizeof(short int)*nX);
	l3 = (short int *) malloc(sizeof(short int)*nX);
	l4 = (short int *) malloc(sizeof(short int)*nX);
	l5 = (short int *) malloc(sizeof(short int)*nX);
	lOut1 = (short int *) malloc(sizeof(short int)*nX);
	lOut2 = (short int *) malloc(sizeof(short int)*nX);
	lOut3 = (short int *) malloc(sizeof(short int)*nX);
	lOut4 = (short int *) malloc(sizeof(short int)*nX);
	/*horizontal*/
	for(row=0;row<nY;row++){
		err=GDALRasterIO(hB1,GF_Read,0,row,nX,1,l1,nX,1,GDT_Byte,0,0);
		err=GDALRasterIO(hBOut1,GF_Write,0,row,nX,1,l1,nX,1,GDT_Byte,0,0);
		err=GDALRasterIO(hBOut1,GF_Read,0,row,nX,1,lOut1,nX,1,GDT_Byte,0,0);
		for(col=0;col<nX-2;col++){
			if(l1[col]==0&&(l1[col+1]!=0)&&l1[col+2]==0)
					lOut1[col+1]=0;
		}
		err=GDALRasterIO(hBOut1,GF_Write,0,row,nX,1,lOut1,nX,1,GDT_Byte,0,0);
	}
	/*vertical*/
	for(row=0;row<nY-3;row++){
		err=GDALRasterIO(hBOut1,GF_Read,0,row+0,nX,1,l1,nX,1,GDT_Byte,0,0);
		err=GDALRasterIO(hBOut1,GF_Read,0,row+1,nX,1,l2,nX,1,GDT_Byte,0,0);
		err=GDALRasterIO(hBOut1,GF_Read,0,row+2,nX,1,l3,nX,1,GDT_Byte,0,0);
		err=GDALRasterIO(hBOut1,GF_Read,0,row+1,nX,1,lOut2,nX,1,GDT_Byte,0,0);
		err=GDALRasterIO(hBOut1,GF_Read,0,row+2,nX,1,lOut3,nX,1,GDT_Byte,0,0);
		err=GDALRasterIO(hBOut1,GF_Read,0,row+3,nX,1,l4,nX,1,GDT_Byte,0,0);
		for(col=0;col<nX;col++){
			/* vertical gap filling */
			if(l1[col]==0&&(l2[col]!=0||l3[col]!=0)&&l4[col]==0){
					lOut2[col]=0;
					lOut3[col]=0;
			}
		}
		err=GDALRasterIO(hBOut1,GF_Write,0,row+1,nX,1,lOut2,nX,1,GDT_Byte,0,0);
		err=GDALRasterIO(hBOut1,GF_Write,0,row+2,nX,1,lOut3,nX,1,GDT_Byte,0,0);
	}
	if( l1 != NULL ) free( l1 );
	if( l2 != NULL ) free( l2 );
	if( l3 != NULL ) free( l3 );
	if( l4 != NULL ) free( l4 );
	if( lOut1 != NULL ) free( lOut1 );
	if( lOut2 != NULL ) free( lOut2 );
	if( lOut3 != NULL ) free( lOut3 );
	if( lOut4 != NULL ) free( lOut4 );
	GDALClose(hD1);
	GDALClose(hDOut1);
	return(EXIT_SUCCESS);
}

