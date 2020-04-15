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

/**
 * @brief Pixel is a struct which represent a pixel and its 3 chan.
 * It is useful to navigate using pointer arithmetic.
 */
typedef struct {
    guchar red;
    guchar green;
    guchar blue;
} Pixel;


typedef struct {
	guchar massCenter; // massCenter is an homogeneous px
	guchar **pixels; // [...]*guchar
	size_t nbPx;
} Classe;

typedef struct {
	guchar *sortedNeighbors[5]; // include itself.
	guchar *pixel; // pixel mustn't be changed.
	Classe *classe;
} PixelClasse;

typedef struct {
	Classe *classes;
	size_t nbClasses;
	PixelClasse **img;
	size_t nbLine, nbCol;
} ImgPxClasse;


ImgPxClasse* NewImgPxClasse(ImgInfo info, size_t nbClasses, guchar *greyImg);

void DeleteImgPxClasse(ImgPxClasse *iPxC);

/// @brief update the classes of the ImgPxClasse, and so there linked pixels
/// @return 1 if it has change, else 0
int Update(ImgPxClasse *iPxC);
#endif /* compute.h */
