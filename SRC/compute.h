
#include <gtk/gtk.h>
#include <math.h>

#define LargeurImage 360

typedef struct{
  int TotPix;
  int NbLine;
  int NbCol;
  int NbChan;
} ImgInfo;

void ComputeImage(guchar *pucImaOrig,
		  guint uiNbCol,
		  guint uiNbLine,
		  guchar *pucImaRes);

typedef struct {
	int massCenter;
	guchar **pixels; // [...]*guchar
	size_t nbPx;
} Classe;

typedef struct {
	const guchar *sortedNeighbors[5]; // include itself.
	const guchar *pixel; // pixel mustn't be changed.
	Classe *classe;
} PixelClasse;

typedef struct {
	Classe *classes;
	size_t nbClasses;
	PixelClasse **img;
	size_t nbLine, nbCol;
} ImgPxClasse;


// TODO: partition this in subfunction and implement it in a .c file.
ImgPxClasse* NewImgPxClasse(ImgInfo info, size_t nbClasses, guchar *greyImg) {
	guchar **img = (guchar **)greyImg; // same adresse, different interpretation.

	ImgPxClasse *iPxC = calloc(1, sizeof(ImgPxClasse));
	iPxC->nbLine = info.NbLine - 2; // minus the first and the last
	iPxC->nbCol = info.NbCol - 2; // minus the first and the last
	iPxC->classes = calloc(nbClasses, sizeof(Classe));
	for (size_t i = 0; i < nbClasses; ++i) {
		iPxC->classes[i] = {
			.massCenter = i * (255 / (nbClasses - 1)),
			NULL,
			0
		}; // 0 and 255 are "center".
	}
	iPxC->img = calloc(iPxC->nbLine, sizeof(Classe*));

	for (size_t i = 1; i < info.NbLine - 1; ++i) {
		iPxC->img[i] = calloc(iPxC->nbCol, sizeof(PixelClasse));
		for (size_t j = 1; j < info.NbCol - 1; ++j) {
			iPxC->img[i][j].pixel = &img[i][j]; // no need to make a copy...

			guchar *neighbors[5] = { &img[i][j], &img[i - 1][j], &img[i + 1][j], &img[i][j - 1],  &img[i][j + 1] };
			for (size_t k = 0; k < 5; ++k) { // i sort the neighbors
				size_t px = k;
				for (size_t l = k; l < 5; ++l) {
					if (*neighbors[l] < *neighbors[px]) {
						px = l;
					}
				}

				iPxC->img[i][j].sortedNeighbors[k] = neighbors[px]; // i set the neighbors (sorted)
				neighbors[px] = neighbors[k];
			}
			// here the pixels and neighbors of them are grouped and sorted.
			// set default classe for all of them.
			iPxC->img[i][j].classe = &iPxC->classe[0]; // by default
			for (size_t k = 1; k < iPxC->nbClasses; ++k) {
				int nd = 0, od = 0; // new distance, old distance
				for (size_t l = 0; l < 5; ++l) {
					nd += abs(*iPxC->img[i][j].sortedNeighbors[l] - iPxC->classes[k].massCenter);
					od += (*iPxC->img[i][j].sortedNeighbors[l] - *iPxC->img[i][j].classe->massCenter);
				}
				if (nd < od) {
					iPxC->img[i][j].classe = &iPxC->classe[k];
				}
			}
			iPxC->img[i][j].classe->pixels = realloc(iPxC->img[i][j].classe->pixels, ++iPxC->img[i][j].classe->nbPx); // check if failed ?
			iPxC->img[i][j].classe->pixels[iPxC->img[i][j].classe->nbPx - 1] = iPxC->img[i][j].pixel; // maybe useless, to see...
		}
	}
	return iPxC;
}
