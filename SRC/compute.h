#ifndef COMPUTE_H
#define COMPUTE_H
#include <gtk/gtk.h>
#include <math.h>

#define LargeurImage 360

typedef struct{
  int TotPix;
  int NbLine;
  int NbCol;
  int NbChan;
} ImgInfo;

void ComputeImage(guchar *pucImaOrig, int NbLine, int NbCol, guchar *pucImaRes);

typedef struct {
    guchar red;
    guchar green;
    guchar blue;
} Pixel;

typedef struct {
	Pixel massCenter; // massCenter is an homogeneous px
	Pixel **pixels; // [...]*guchar
	size_t nbPx;
} Classe;

typedef struct {
	Pixel *sortedNeighbors[5]; // include itself.
	Pixel *pixel; // pixel mustn't be changed.
	Classe *classe;
} PixelClasse;

typedef struct {
	Classe *classes;
	size_t nbClasses;
	PixelClasse **img;
	size_t nbLine, nbCol;
} ImgPxClasse;


// TODO: partition this in subfunction and implement it in a .c file.
ImgPxClasse* NewImgPxClasse(ImgInfo info, size_t nbClasses, guchar *greyImg);

int Update(ImgPxClasse *iPxC); // return weather or not change has been occurred
#endif /* compute.h */
