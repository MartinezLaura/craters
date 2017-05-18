#include<stdio.h>
/*Create 1D arrays*/
int* ai1d(int X);
float* af1d(int X);
double* ad1d(int X);
/*Free 2D arrays*/
void fi2d (int **A, int X);
void ff2d (float **A, int X);
void fd2d (double **A, int X);
/*Create 2D arrays*/
int** ai2d(int X, int Y);
float** af2d(int X, int Y);
double** ad2d(int X, int Y);
/*Realloc 2D arrays*/
int** rai2d(int** A,int X, int Y);
float** raf2d(int** A, int X, int Y);
double** rad2d(int** A, int X, int Y);
/*Struct Array*/
//Ai2d array2D = { 2, 5 };
//array2D.data = malloc(array2D.X * array2D.Y * sizeof *array2D.data);
//array2D[col][row] becomes array2D[col * array2D.Y + row]
//Ai3d array3D = { 2, 5, 7 };
//array3D.data = malloc(array3D.X * array3D.Y * array3D.Z * sizeof *array3D.data);
//array3D[col][row][elv] becomes array3D[col * (array3D.Y * array3D.Z) + row * array3D.Z + elv]
typedef struct { int X; int Y; int* data;} Ai2d;
typedef struct { int X; int Y; int Z; int* data;} Ai3d;
typedef struct { int X; int Y; float* data;} Af2d;
typedef struct { int X; int Y; int Z; float* data;} Af3d;
typedef struct { int X; int Y; double* data;} Ad2d;
typedef struct { int X; int Y; int Z; double* data;} Ad3d;
