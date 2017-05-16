#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"arrays.h"

int** fill6px(int **l0, int nX, int nY,int VAL, int NODATA){
	/*Fills NODATA in X, Y and diagonal for up to 6 NODATA pixels*/
	/*Traverses across increasing X then increasing Y*/
	int i,j,k,l,xcount=0;
	int **output=ai2d(nX,nY);
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) output[i][j]=l0[i][j];

	for(i=6;i<nX;i++){
		for(j=6;j<nY;j++){
			xcount=0;
			/* Look for next pixel after NODATA in x*/
			if (l0[i-6][j-6] == VAL && l0[i-5][j-6] == NODATA
				&& l0[i-6][j-5] == NODATA
				&& l0[i-5][j-5] == NODATA){
				/*we have a candidate*/
				xcount=1;

				if(l0[i-6][j-4]==NODATA && l0[i-4][j-6]==NODATA && l0[i-4][i-4]==NODATA){
					xcount++;
				}
						
				if(l0[i-6][j-3]==NODATA && l0[i-3][j-6]==NODATA && l0[i-3][i-3]==NODATA){
					xcount++;
				}
						
				if(l0[i-6][j-2]==NODATA && l0[i-2][j-6]==NODATA && l0[i-2][i-2]==NODATA){
					xcount++;
				}
						
				if(l0[i-6][j-1]==NODATA && l0[i-1][j-6]==NODATA && l0[i-1][i-1]==NODATA){
					xcount++;
				}
				/*We reached an open-ended area, it is not a tree crown island */
				if(l0[i-6][j]==NODATA || l0[i][j-6]==NODATA || l0[i][i]==NODATA){
					xcount=0;
				}
			}
			/* we still have a candidate */
			if (xcount>=1){
				for(k=0;k<xcount;k++){
					for(l=0;l<xcount;l++){
						output[i-6+k][j-6+l]=VAL;
					}
				}
			}
		}
	}
	return(output);
}
int** dilate(int **l0, int nX, int nY,int VAL, int NODATA, int THRSHLD){
	int i,j,c,k,l;
	int **output=ai2d(nX,nY);
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) output[i][j]=l0[i][j];

	/*West to East pass*/
	for(i=1;i<nX-1;i++){
		for(j=1;j<nY-1;j++){
			if (l0[i][j] == VAL && l0[i+1][j] == NODATA){
				c=0;
				for(k=-1;k<=1;k++)
					for(l=-1;l<=1;l++)
						if(l0[i+k][j+l] == VAL) c++;
				if(c>THRSHLD) output[i][j]=VAL;
			}
		}
	}
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) l0[i][j]=output[i][j];
	/*East to West pass*/
	for(i=1;i<nX-1;i++){
		for(j=1;j<nY-1;j++){
			if (l0[i+1][j] == VAL && l0[i][j] == NODATA){
				c=0;
				for(k=-1;k<=1;k++)
					for(l=-1;l<=1;l++)
						if(l0[i+k][j+l] == VAL) c++;
				if(c>THRSHLD) output[i][j]=VAL;
			}
		}
	}
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) l0[i][j]=output[i][j];
	/*North (VAL) to South (NODATA) pass*/
	for(j=1;j<nY-1;j++){
		for(i=1;i<nX-1;i++){
			if (l0[i][j] == VAL && l0[i][j+1] == NODATA){
				c=0;
				for(k=-1;k<=1;k++)
					for(l=-1;l<=1;l++)
						if(l0[i+k][j+l] == VAL) c++;
				if(c>THRSHLD) output[i][j]=VAL;
			}
		}
	}
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) l0[i][j]=output[i][j];
	/*South (VAL) to North (NODATA) pass*/
	for(j=1;j<nY-1;j++){
		for(i=1;i<nX-1;i++){
			if (l0[i][j+1] == VAL && l0[i][j] == NODATA){
				c=0;
				for(k=-1;k<=1;k++)
					for(l=-1;l<=1;l++)
						if(l0[i+k][j+l] == VAL) c++;
				if(c>THRSHLD) output[i][j]=VAL;
			}
		}
	}
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) l0[i][j]=output[i][j];

	return(l0);
}

int** dilation(int **l0, int nX, int nY, int VAL, int NODATA, int THRSHLD){
	int i,j,c,k,l;
	int **output=ai2d(nX,nY);
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) output[i][j]=l0[i][j];

	for(i=1;i<nX-1;i++){
		for(j=1;j<nY-1;j++){
			if (l0[i][j] == NODATA){
				c=0;
				for(k=-1;k<=1;k++)
					for(l=-1;l<=1;l++)
						if(k!=0 && l!=0 && l0[i+k][j+l] == VAL) c++;
				if(c>THRSHLD) output[i][j]=VAL;
			}
		}
	}
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) l0[i][j]=output[i][j];
	fi2d(output,nX);
	return(l0);
}


int** erosion(int **l0, int nX, int nY, int VAL, int NODATA, int THRSHLD){
	int i,j,c,k,l;
	int **output=ai2d(nX,nY);
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) output[i][j]=l0[i][j];

	for(i=1;i<nX-1;i++){
		for(j=1;j<nY-1;j++){
			if (l0[i][j] == VAL){
				c=0;
				for(k=-1;k<=1;k++)
					for(l=-1;l<=1;l++)
						if(l0[i+k][j+l] == NODATA) c++;
				/*c<=8 forces the code to not delete the last pixel*/
				if(c>THRSHLD && c<=8) output[i][j]=NODATA;
			}
		}
	}
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) l0[i][j]=output[i][j];
	fi2d(output,nX);
	return(l0);
}


int** whoaremyneighbours(int **l0, int i, int j){
	int **out=ai2d(i,j);
	out[0][0]=l0[i-1][j-1];
	out[0][1]=l0[i-1][j];
	out[0][2]=l0[i-1][j+1];
	out[1][0]=l0[i][j-1];
	out[1][1]=l0[i][j];
	out[1][2]=l0[i][j+1];
	out[2][0]=l0[i+1][j-1];
	out[2][1]=l0[i+1][j];
	out[2][2]=l0[i+1][j+1];
	return(out);
}

int** intersect2neighbours(int **out0, int **out1, int xshift, int yshift){
	int i,j;
	/*define output storage rule*/
	int **output=ai2d(3,3);
	for (i=0;i<3+xshift;i++){
		for (j=0;j<3+yshift;j++){
			output[i][j]=0;
		}
	}
	for (i=0;i<3+xshift;i++){
		for (j=0;j<3+yshift;j++){
			if (out0[i][j]==out1[i+xshift][j+yshift]
				&&(i+xshift<3)&&(j+yshift<3)
				&&(i+xshift>=0)&&(j+yshift>=0)){
				output[i][j]=1;
			}
		}
	}	
	return(output);
}

int** mm_fillisles(int **l0, int nX, int nY, int VAL, int NODATA, int INV){
	int i,j,count;
	int ** out0=ai2d(3,3);
	int ** out1=ai2d(3,3);
	int ** out2=ai2d(3,3);
	out2[0][0]=0;
	/* inverse: NODATA pixels will be processed instead of VAL*/
	if(INV==1){
		i=NODATA;
		NODATA=VAL;
		VAL=i;
	}
	/* Follow pixel VAL with right side NODATA towards East */
	/* If we find back the original pixel then we have an island */
	/* We go South always until bottom, then move one step Eastward */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (l0[i][j] == VAL && l0[i+1][j] == NODATA){
				/*We have a candidate, let us tag it -VAL*/
				l0[i][j]=-VAL;
				/*Initialize the search counter to 0*/
				count=0;
				/*Now, let us move neighbour to neighbour */
				/*Until we come back to the tagged pixel */
				/*Or not */
				while (!count){
					out0 = whoaremyneighbours(l0,i,j);
					out1 = whoaremyneighbours(l0,i+1,j);
					out2 = intersect2neighbours(out0,out1,0,0);
					/*Round robin search*/	 
					

					/*We found back pixel i,j with value = -VAL*/
					count=1;
				}
				/* Now we can fill every pixels inside it */
			}
		}
	}
	/* cleaning procedure for identified pixels */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (l0[i][j] == VAL){
				if (i>0 && l0[i-1][j]==VAL+1) l0[i-1][j] = NODATA;
				if (j>0 && l0[i][j-1]==VAL+1) l0[i][j-1] = NODATA;
				if (i+1<nX && l0[i+1][j]==VAL+1) l0[i+1][j] = NODATA;
				if (j+1<nY && l0[i][j+1]==VAL+1) l0[i][j+1] = NODATA;
			}
		}
	}
	return(l0);
}

int** mm_rmfringepix(int **l0, int nX, int nY, int VAL, int NODATA, int npix, int INV){
	int i,j,k;
	int **output=ai2d(nX,nY);
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) output[i][j]=l0[i][j];
	/* inverse: NODATA pixels will be removed if 3+ surrounding pixels have VAL*/
	if(INV==1){
		i=NODATA;
		NODATA=VAL;
		VAL=i;
	}
	/* remove pixels of value VAL all around the frame of the image */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			for(k=0;k<=npix;k++){
				if (i==0+npix || i==nX-1-npix || j==0+npix || j==nY-1-npix){
					output[i][j]=NODATA;
				}
			}
		}
	}
	return(output);
}

int** mm_rmlonepix(int **l0, int nX, int nY, int VAL, int NODATA, int INV){
	int i,j,count;
	int **output=ai2d(nX,nY);
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) output[i][j]=l0[i][j];
	/* inverse: NODATA pixels will be removed if 3+ surrounding pixels have VAL*/
	if(INV==1){
		i=NODATA;
		NODATA=VAL;
		VAL=i;
	}
	/* remove pixels of value VAL surrounded on 3+ sides out of 4 by value NODATA */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (l0[i][j] == VAL){
				count=0;
				if (i>0 && l0[i-1][j]==NODATA) count++;
				if (j>0 && l0[i][j-1]==NODATA) count++;
				if (i+1<nX && l0[i+1][j]==NODATA) count++;
				if (j+1<nY && l0[i][j+1]==NODATA) count++;
				if(count>=2) output[i][j]=NODATA+1;
			}
		}
	}
	/* cleaning procedure for identified pixels */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (l0[i][j] == VAL){
				if (i>0 && output[i-1][j]==NODATA+1) output[i-1][j] = NODATA;
				if (j>0 && output[i][j-1]==NODATA+1) output[i][j-1] = NODATA;
				if (i+1<nX && output[i+1][j]==NODATA+1) output[i+1][j] = NODATA;
				if (j+1<nY && output[i][j+1]==NODATA+1) output[i][j+1] = NODATA;
			}
		}
	}
	/* Just in case of not completely clean */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (output[i][j] == NODATA+1) output[i][j]=NODATA;
		}
	}
	return(output);
}

int** mm_erode(int **l0, int nX, int nY, int VAL, int NODATA, int DIST2NN){
	int i,j,k;
	int **output=ai2d(nX,nY);
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) output[i][j]=l0[i][j];
	/* If not set by user, default cleaning is 1 pixel width */
	/* DIST2NN: distance 2 nearest neighbour for cleaning */
	if(DIST2NN==-1) DIST2NN=0;
	/* erode */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (l0[i][j] == VAL){
				/* Force isolated pixel to survive */
				if( (i>0 && l0[i-1][j]==NODATA) &&
				(j>0 && l0[i][j-1]==NODATA) &&
				(i+1<nX && l0[i+1][j]==NODATA) &&
				(j+1<nY && l0[i][j+1]==NODATA) &&
				(i-1>0 && j-1>0 && l0[i-1][j-1] == NODATA) &&
				(i+1<nX && j-1>0 && l0[i+1][j-1] == NODATA) &&
				(i-1>0 && j+1<nY && l0[i-1][j+1] == NODATA) &&
				(i+1<nX && j+1<nY && l0[i+1][j+1] == NODATA) )
				{
					output[i][j]=l0[i][j];
				}
				else {
					if (i>0 && l0[i-1][j]==NODATA) output[i][j] = NODATA+1;
					if (j>0 && l0[i][j-1]==NODATA) output[i][j] = NODATA+1;
					if (i+1<nX && l0[i+1][j]==NODATA) output[i][j] = NODATA+1;
					if (j+1<nY && l0[i][j+1]==NODATA) output[i][j] = NODATA+1;
				}
			}
		}
	}
	/* cleaning procedure for identified pixels */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (l0[i][j] == VAL){
				for(k=0;k<DIST2NN;k++){
				/* clean k pixel(s) away */
				// [i-k,j] left neighbours
				if (i>(0+k) && output[i-k][j]==NODATA+1) output[i-k][j] = NODATA;
				// [i,j-k] top neighbours
				if (j>(0+k) && output[i][j-k]==NODATA+1) output[i][j-k] = NODATA;
				// [i+k,j] right neighbours
				if ((i+1+k)<nX && output[i+k][j]==NODATA+1) output[i+k][j] = NODATA;
				// [i,j+k] bottom neighbours
				if ((j+1+k)<nY && output[i][j+k]==NODATA+1) output[i][j+k] = NODATA;
				/* Diagonal neighbours */
				// [i-k,j-k] top left neighbours
				if (i>(0+k) && j>(0+k) && output[i-k][j-k]==NODATA+1) output[i-k][j-k] = NODATA;
				// [i+k,j-k] top right neighbours
				if (j>(0+k) && (i+1+k)<nX && output[i+k][j-k]==NODATA+1) output[i+k][j-k] = NODATA;
				// [i+k,j+k] bottom right neighbours
				if ((i+1+k)<nX && (j+1+k)<nY && output[i+k][j+k]==NODATA+1) output[i+k][j+k] = NODATA;
				// [i-k,j+k] bottom left neighbours
				if ((j+1+k)<nY && i>(0+k) && output[i-k][j+k]==NODATA+1) output[i-k][j+k] = NODATA;
				}
			}
		}
	}
	/* Just in case of not completely clean */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (output[i][j] == NODATA+1) output[i][j]=NODATA;
		}
	}
	for(i=0;i<nX;i++) for(j=0;j<nY;j++) l0[i][j]=output[i][j];
	fi2d(output,nX);
	return(l0);
}

int** mm_dilate(int **l0, int nX, int nY, int VAL, int NODATA){
	int i,j;
	/* dilate */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (l0[i][j] == VAL){
				if (i>0 && l0[i-1][j]==NODATA) l0[i-1][j] = VAL+1;
				if (j>0 && l0[i][j-1]==NODATA) l0[i][j-1] = VAL+1;
				if (i+1<nX && l0[i+1][j]==NODATA) l0[i+1][j] = VAL+1;
				if (j+1<nY && l0[i][j+1]==NODATA) l0[i][j+1] = VAL+1;
			}
		}
	}
	/* cleaning procedure for identified pixels */
	for(i=0;i<nX;i++){
		for(j=0;j<nY;j++){
			if (l0[i][j] == VAL){
				if (i>0 && l0[i-1][j]==VAL+1) l0[i-1][j] = VAL;
				if (j>0 && l0[i][j-1]==VAL+1) l0[i][j-1] = VAL;
				if (i+1<nX && l0[i+1][j]==VAL+1) l0[i+1][j] = VAL;
				if (j+1<nY && l0[i][j+1]==VAL+1) l0[i][j+1] = VAL;
			}
		}
	}
	return(l0);
}

int ptintriangle(double ax,double ay,double bx,double by,double cx,double cy,double px,double py){
	double area = 0.5 *(-by*cx + ay*(-bx + cx) + ax*(by - cy) + bx*cy);
	double s = 1/(2*area)*(ay*cx - ax*cy + (cy - ay)*px + (ax - cx)*py);
	double t = 1/(2*area)*(ax*cy - ay*bx + (ay - by)*px + (bx - ax)*py);
	/*if s, t and 1-s-t are all positive, then point is inside triangle*/
	int return_val = 0;
	if(s>=0 && t>=0 && (1-s-t)>=0)
		return_val = 1;
	return(return_val);
}

double cir3pts(double ax, double ay, double bx, double by, double cx, double cy, int type){
	/*use as*/
	/*cir3pts(ax, ay, bx, by, cx, cy, type)*/
	/*type=0 => centerx */
	/*type=1 => centery */
	/*type=2 => radius */
	double a = bx - ax;
	double b = by - ay;
	double c = cx - ax;
	double d = cy - ay;
	double e = a*(ax + bx) + b*(ay + by);
	double f = c*(ax + cx) + d*(ay + cy);
	double g = 2*(a*(cy - by) - b*(cx - bx));
	double cex, cey, rd;
	if(g != 0){
		/*we have a candidate*/
		cex = (d*e - b*f)/g;
		cey = (a*f - c*e)/g;
		rd = sqrt((ax - cex)*(ax - cex) + (ay - cey)*(ay - cey));
	} else {
		/*fail gracefully*/
		cex = 0;
		cey = 0;
		rd = 0;
	}
	/*Check if barycentric equation return the point inside the triangle*/
	/*if it fails rewrite the output to fail gracefully*/
	if (rd==0){
		/*do nothing*/
	} else {
		if (!ptintriangle(ax,ay,bx,by,cx,cy,cex,cey)){
			cex=0;
			cey=0;
			rd=0;
		}
	}
	int return_val = 0;
	if(type==0) return_val=cex;
	if(type==1) return_val=cey;
	if(type==2) return_val=rd*1000;
	return(return_val);
}

int conv1D2Drow(int nX, int rowcol){
	return((int) floor((double) rowcol/ (double) nX));
}

int conv1D2Dcol(int row, int rowcol){
	return(rowcol-row);
} 

int conv2D1D(int nX, int col, int row){
	return(row*nX+col);
}

//Multi-scale pixel retrieval 
//both input are flattened from 2D to 1D
//Go back to 2D, reference raster coordinates is used
//then find the target scaled coordinates
//Convert back from 2D to 1D for target
//Return 1D target coordinate
// ref_nX_o_tgt_nX is ratio of nX (Reference LU over target model data)
// ref_nY_o_tgt_nY is ratio of nY (Reference LU over target model data)
int findPixelAcrossScales(int ref_rowcol, int ref_nX, int tgt_nX, int ref_nX_o_tgt_nX, int ref_nY_o_tgt_nY ){
	//Where is LU raster reference pixel?
	//at rowcol
	//convert LU raster reference from 1D to 2D
	int ref_row = conv1D2Drow(ref_nX,ref_rowcol);
	int ref_col = conv1D2Dcol(ref_row,ref_rowcol);
	//convert ref coordinates across scales to target
	int tgt_col = (int) (ref_col/ref_nX_o_tgt_nX);
	int tgt_row = (int) (ref_row/ref_nY_o_tgt_nY);
	//convert 2D to 1D for target raster
	int tgt_rowcol = conv2D1D(tgt_nX,tgt_col,tgt_row);
	//Return the target 1D coordinate cooresponding to ref_rowcol
	return(tgt_rowcol);
}
