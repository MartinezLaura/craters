#include<stdio.h>
/* fill6px */
int** fill6px(int **l0, int nX, int nY,int VAL, int NODATA);

/* Math Morpho */
int** mm_rmlonepix(int **l0, int nX, int nY, int VAL, int NODATA, int INV);
int** mm_rmfringepix(int **l0, int nX, int nY, int VAL, int NODATA,int npix, int INV);
int** mm_erode(int **l0, int nX, int nY, int VAL, int NODATA, int DIST2NN);
int** mm_dilate(int **l0, int nX, int nY, int VAL, int NODATA);
int** dilation(int **l0, int nX, int nY, int VAL, int NODATA, int THRSHLD);
int** erosion(int **l0, int nX, int nY, int VAL, int NODATA, int THRSHLD);

/* Circle detection and stat */
int ptintriangle(double ax,double ay,double bx,double by,double cx,double cy,double px,double py);
double cir3pts(double ax, double ay, double bx, double by, double cx, double cy, int type);

/* Data Access */
int conv1D2Drow(int nX, int rowcol);
int conv1D2Dcol(int row, int rowcol);
int conv2D1D(int nX, int col, int row);
int findPixelAcrossScales(int ref_rowcol, int ref_nX, int tgt_nX, int ref_nX_o_tgt_nX, int ref_nY_o_tgt_nY );

