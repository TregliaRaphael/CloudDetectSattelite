#include <stdio.h>
#include <gtk/gtk.h>
#include "compute.h"

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/


/*---------------------------------------
  Proto:


  But:

  Entrees:
          --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes � la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes � la suite des autres)


  Sortie:

  Rem:

  Voir aussi:

  ---------------------------------------*/


// Img* newImg(guchar* img, int nbLine, int nbCol, int nbChan) {
//   Img* img = NewAdr(sizeof(Img));
//   *img = {
//     .TotPix = nbCol * nbLine,
//     .NbLine = NbLine,
//     .NbCol = nbCol,
//     .NbChan = nbChan
//   };
//   return img;
// }

ImgPxClasse* NewImgPxClasse(ImgInfo info, size_t nbClasses, guchar *greyImg) {
 guchar **img = (guchar **)greyImg; // same adresse, different interpretation.

 ImgPxClasse *iPxC = calloc(1, sizeof(ImgPxClasse));
 iPxC->nbLine = info.NbLine - 2; // minus the first and the last
 iPxC->nbCol = info.NbCol - 2; // minus the first and the last
 iPxC->classes = calloc(nbClasses, sizeof(Classe));
 for (size_t i = 0; i < nbClasses; ++i) {
   Classe cls = {
     .massCenter = i * (255 / (nbClasses - 1)),
     NULL,
     0
   }; // 0 and 255 are "center".
   iPxC->classes[i] = cls;
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
     iPxC->img[i][j].classe = &iPxC->classes[0]; // by default
     for (size_t k = 1; k < iPxC->nbClasses; ++k) {
       int nd = 0, od = 0; // new distance, old distance
       for (size_t l = 0; l < 5; ++l) {
         nd += abs(*iPxC->img[i][j].sortedNeighbors[l] - iPxC->classes[k].massCenter);
         od += (*iPxC->img[i][j].sortedNeighbors[l] - iPxC->img[i][j].classe->massCenter);
       }
       if (nd < od) {
         iPxC->img[i][j].classe = &iPxC->classes[k];
       }
     }
     iPxC->img[i][j].classe->pixels = realloc(iPxC->img[i][j].classe->pixels, ++iPxC->img[i][j].classe->nbPx); // check if failed ?
     iPxC->img[i][j].classe->pixels[iPxC->img[i][j].classe->nbPx - 1] = iPxC->img[i][j].pixel; // maybe useless, to see...
   }
 }
 return iPxC;
}

unsigned Rgb2Gray(ImgInfo imgInf, guchar* imgSrc, guchar* imgDst);


void ComputeImage(guchar *pucImaOrig, int NbLine, int NbCol, guchar *pucImaRes)
{
  ImgInfo imgInf =
  {
    .TotPix = NbCol * NbLine,
    .NbLine = NbLine,
    .NbCol = NbCol,
    .NbChan = 3
  };

  printf("Segmentation de l'image.... A vous!\n");

  Rgb2Gray(imgInf, pucImaOrig, pucImaRes);


}



unsigned Rgb2Gray(ImgInfo imgInf, guchar* imgSrc, guchar* imgDst)
{
  guchar ucMeanPx;
  const uint Red = 0, Green = 1, Blue = 2;
  for(int iPx = 0; iPx < imgInf.TotPix; ++iPx) {
    iPx *= imgInf.NbChan;
    guchar* px = imgSrc + iPx;
    // we add a weight for each R,G, B. That weight comes from: https://scikit-image.org/docs/dev/auto_examples/color_exposure/plot_rgb_to_gray.html?fbclid=IwAR3PC3JwvHGVxgqGHkMl11nUfZz32NcZqIK7M4NfFS6ELyKIiJ2ah_u2NVU
    ucMeanPx = (px[Red] * 2125 + px[Green] * 7154 + px[Blue] * 721)/10000;
    for(int iChan = 0; iChan < imgInf.NbChan; ++iChan) {
       imgDst[iPx + iChan] = ucMeanPx;
    }
  }
}
