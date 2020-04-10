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
	      (les lignes sont mises les unes à la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes à la suite des autres)


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

unsigned RgbToGrayScale(ImgDef imgDef, guchar* imgSrc, guchar* imgDst);





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

  RgbToGrayScale(imgDef, pucImaOrig, pucImaRes);
}



unsigned RgbToGrayScale(ImgDef imgDef, guchar* imgSrc, guchar* imgDst)
{
  guchar ucMeanPix;

  for(int iNumPix = 0; iNumPix < imgDef.TotPix * imgDef.NbChan; iNumPix += imgDef.NbChan){
    /*moyenne sur les composantes RVB */
    guchar* pixel = imgSrc + iNumPix;
    ucMeanPix = (*pixel * 2125 + *(pixel+1) * 7154 + *(pixel+2) * 721)/10000;
    /* sauvegarde du resultat */
    for(int iNumChannel = 0; iNumChannel < imgDef.NbChan; iNumChannel++)
      *(imgDst + iNumPix + iNumChannel)= ucMeanPix;
  }
}
