#include<stdio.h>
#include<math.h>


/*int ptintriangle(double ax,double ay,double bx,double by,double cx,double cy,double px,double py){
	double area = 0.5 *(-by*cx + ay*(-bx + cx) + ax*(by - cy) + bx*cy);
	double s = 1/(2*area)*(ay*cx - ax*cy + (cy - ay)*px + (ax - cx)*py);
	double t = 1/(2*area)*(ax*cy - ay*bx + (ay - by)*px + (bx - ax)*py);
	*//*if s, t and 1-s-t are all positive, then point is inside triangle*//*
	int return_val = 0;
	if(s>=0 && t>=0 && (1-s-t)>=0)
		return_val = 1;
	return(return_val);
}*/
int point_in_triangle(double point_x, double point_y, double point_z,
                         double t1_x, double t1_y, double t1_z, double t2_x,
                         double t2_y, double t2_z, double t3_x, double t3_y,
                         double t3_z)
{
    /*printf("point_in_triangle: sand=%5.3f clay=%5.3f silt=%5.3f",point_x,point_y,point_z);*/
    int answer;
    double answer1_x, answer1_y, answer1_z;
    double answer2_x, answer2_y, answer2_z;
    double answer3_x, answer3_y, answer3_z;

    /* Consider three points forming a trinagle from a given soil class boundary ABC */
    /* Consider F an additional point in space */
    double af1, af2, af3;       /*Points for vector AF */
    double bf1, bf2, bf3;       /*Points for vector BF */
    double cf1, cf2, cf3;       /*Points for vector CF */
    double ab1, ab2, ab3;       /*Points for vector AB */
    double bc1, bc2, bc3;       /*Points for vector BC */
    double ca1, ca2, ca3;       /*Points for vector CA */

    /* Create vectors AB, BC and CA */
    ab1 = (t2_x - t1_x);
    ab2 = (t2_y - t1_y);
    ab3 = (t2_z - t1_z);
    bc1 = (t3_x - t2_x);
    bc2 = (t3_y - t2_y);
    bc3 = (t3_z - t2_z);
    ca1 = (t1_x - t3_x);
    ca2 = (t1_y - t3_y);
    ca3 = (t1_z - t3_z);
    /* Create vectors AF, BF and CF */
    af1 = (point_x - t1_x);
    af2 = (point_y - t1_y);
    af3 = (point_z - t1_z);
    bf1 = (point_x - t2_x);
    bf2 = (point_y - t2_y);
    bf3 = (point_z - t2_z);
    cf1 = (point_x - t3_x);
    cf2 = (point_y - t3_y);
    cf3 = (point_z - t3_z);
    /* Calculate the following CrossProducts: */
    /* AFxAB */ 
    answer1_x = (af2 * ab3) - (af3 * ab2);
    answer1_y = (af3 * ab1) - (af1 * ab3);
    answer1_z = (af1 * ab2) - (af2 * ab1);
    /* G_message("answer(AFxAB)= %f %f %f",answer1_x, answer1_y, answer1_z); */
    /*BFxBC */
    answer2_x = (bf2 * bc3) - (bf3 * bc2);
    answer2_y = (bf3 * bc1) - (bf1 * bc3);
    answer2_z = (bf1 * bc2) - (bf2 * bc1);
    /* G_message("answer(BFxBC)= %f %f %f",answer2_x, answer2_y, answer2_z); */
    /*CFxCA */
    answer3_x = (cf2 * ca3) - (cf3 * ca2);
    answer3_y = (cf3 * ca1) - (cf1 * ca3);
    answer3_z = (cf1 * ca2) - (cf2 * ca1);
    /* G_message("answer(CFxCA)= %f %f %f",answer3_x, answer3_y, answer3_z); */
    answer = 0; /*initialize value */
    if ((int)answer1_x >= 0 && (int)answer2_x >= 0 && (int)answer3_x >= 0) {
        answer++;
    }
    else if ((int)answer1_x <= 0 && (int)answer2_x <= 0 &&
             (int)answer3_x <= 0) {
        answer--;
    }
    if ((int)answer1_y >= 0 && (int)answer2_y >= 0 && (int)answer3_y >= 0) {
        answer++;
    }
    else if ((int)answer1_y <= 0 && (int)answer2_y <= 0 &&
             (int)answer3_y <= 0) {
        answer--;
    }
    if ((int)answer1_z >= 0 && (int)answer2_z >= 0 && (int)answer3_z >= 0) {
        answer++;
    }
    else if ((int)answer1_z <= 0 && (int)answer2_z <= 0 &&
             (int)answer3_z <= 0) {
        answer--;
    }
    if (answer == 3 || answer == -3) {
        answer = 1;
    }
    else {
        answer = 0;
    }
    return(answer);
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
		if (!point_in_triangle(cex,cey,0,ax,ay,0,bx,by,0,cx,cy,0)){
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
