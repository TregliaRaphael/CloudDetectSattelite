#include <stdio.h>
#include <gtk/gtk.h>

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
typedef struct{
  int TotPix;
  int NbLine;
  int NbCol;
  int NbChan;
} ImgDef;

unsigned Rgb2Gray(ImgDef imgDef, guchar* imgSrc, guchar* imgDst);





void ComputeImage(guchar *pucImaOrig, int NbLine, int NbCol, guchar *pucImaRes)
{
  ImgDef imgDef =
  {
    .TotPix = NbCol * NbLine,
    .NbLine = NbLine,
    .NbCol = NbCol,
    .NbChan = 3
  };

  printf("Segmentation de l'image.... A vous!\n");

  Rgb2Gray(imgDef, pucImaOrig, pucImaRes);
}



unsigned Rgb2Gray(ImgDef imgDef, guchar* imgSrc, guchar* imgDst)
{
  guchar ucMeanPx; // uncolored Mean pixel
  const uint Red = 0, Green = 1, Blue = 2;
  for(int iPx = 0; iPx < imgDef.TotPix; ++iPx) {
    iPx *= imgDef.NbChan;
    guchar* px = imgSrc + iPx;
    // we add a weight for each R,G, B. That weight comes from: https://scikit-image.org/docs/dev/auto_examples/color_exposure/plot_rgb_to_gray.html?fbclid=IwAR3PC3JwvHGVxgqGHkMl11nUfZz32NcZqIK7M4NfFS6ELyKIiJ2ah_u2NVU
    ucMeanPx = (px[Red] * 2125 + px[Green] * 7154 + px[Blue] * 721)/10000;
    for(int iChan = 0; iChan < imgDef.NbChan; ++iChan) {
       imgDst[iPx + iChan] = ucMeanPx;
    }
  }
}
