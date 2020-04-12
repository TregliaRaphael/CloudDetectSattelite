#include <stdio.h>
#include <gtk/gtk.h>
#include "misc.h"

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
  guchar ucMeanPx; // uncolored Mean pixel
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
