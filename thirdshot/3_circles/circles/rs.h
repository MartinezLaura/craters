#include<stdio.h>

int point_in_triangle(double point_x, double point_y, double point_z,
                         double t1_x, double t1_y, double t1_z, double t2_x,
                         double t2_y, double t2_z, double t3_x, double t3_y,
                         double t3_z);
/*int ptintriangle(double ax,double ay,double bx,double by,double cx,double cy,double px,double py);*/
double cir3pts(double ax, double ay, double bx, double by, double cx, double cy, int type);

int conv1D2Drow(int nX, int rowcol);
int conv1D2Dcol(int row, int rowcol);
int conv2D1D(int nX, int col, int row);
int findPixelAcrossScales(int ref_rowcol, int ref_nX, int tgt_nX, int ref_nX_o_tgt_nX, int ref_nY_o_tgt_nY );

