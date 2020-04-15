#include <stdio.h>
#include <gtk/gtk.h>
#include "compute.h"
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

void rgb2Gray(ImgInfo imgInf, guchar* imgSrc, guchar* imgDst);


void ComputeImage(guchar *pucImaOrig, int NbLine, int NbCol, guchar *pucImaRes)
{
  ImgInfo imgInf =
  {
    .TotPix = NbCol * NbLine,
    .NbLine = NbLine,
    .NbCol = NbCol,
    .NbChan = 3
  };

  rgb2Gray(imgInf, pucImaOrig, pucImaRes);

  const unsigned int nbClass = GetNbClass();

  ImgPxClasse* iPxC = NewImgPxClasse(imgInf, nbClass, pucImaRes);

  while(Update(iPxC));

  int nb = 0;
  Pixel R = {
      .red = 200,
      .green = 0,
      .blue = 0
  };
  for (size_t i = 0; i < iPxC->nbClasses; ++i) {
    if (iPxC->classes[i].massCenter >= 150) {
      for (size_t j = i; j < iPxC->nbClasses; ++j) {
        if (GetGuiMode()) {
          for (size_t i = 0; i < iPxC->classes[j].nbPx; ++i) {
            *(Pixel*) iPxC->classes[j].pixels[i] = R;
          }
        }
        nb += iPxC->classes[j].nbPx;
      }
      break;
    }
  }

  for (size_t i = 0; i < iPxC->classes[nbClass - 1].nbPx && iPxC->classes[nbClass - 1].massCenter > 100; ++i) {
    *(Pixel*)iPxC->classes[nbClass - 1].pixels[i] = R;
  }

  if (nb == 0 && iPxC->classes[nbClass - 1].massCenter > 100) nb = iPxC->classes[nbClass - 1].nbPx;
  float percent = (100 * (float)nb) / imgInf.TotPix;
  printf("there is: %f%% cloud on the image", percent);
  DeleteImgPxClasse(iPxC);
}


/// @brief classic function to transform colored into greyscale.
/// @param imgInfo: information of the image
/// @param imgSrc: base image (colored or not)
/// @param imgDst: result image (grey scale)
void rgb2Gray(ImgInfo imgInf, guchar* imgSrc, guchar* imgDst)
{
  guchar ucMeanPx;
  const uint Red = 0, Green = 1, Blue = 2;
  for(int iPx = 0; iPx < imgInf.TotPix * imgInf.NbChan; iPx += imgInf.NbChan) {
    guchar* px = imgSrc + iPx;
    ucMeanPx = (px[Red] + px[Green] + px[Blue]) / 3;
    for(int iChan = 0; iChan < imgInf.NbChan; ++iChan) {
       imgDst[iPx + iChan] = ucMeanPx;
    }
  }
}

int comp(const void *elem1, const void *elem2) {
  return **(guchar**)elem1 - **(guchar**)elem2;
}

/// @brief compute the median pixel of the cls Classe.
guchar pxMedian(Classe *cls) {
  if (cls->nbPx == 0)
    return cls->massCenter;
  qsort(cls->pixels, cls->nbPx, sizeof(guchar*), comp);
  return *cls->pixels[cls->nbPx / 2]; // the median
}

/// @brief link the pixels to there new classes, and add them to there new classes array.
void linkPxClass(ImgPxClasse *iPxC, PixelClasse *imgPx) {
  for (size_t i = 0; i < iPxC->nbClasses; ++i) {
    int nd = 0, od = 0;
    for (size_t px = 0; px < 5; ++px) {
      od += abs(*imgPx->sortedNeighbors[px] - imgPx->classe->massCenter);
      nd += abs(*imgPx->sortedNeighbors[px] - iPxC->classes[i].massCenter);
    }
    if (nd < od) {
      imgPx->classe = &iPxC->classes[i]; // change class util a better one is found
    }
  }
  imgPx->classe->pixels[imgPx->classe->nbPx++] = imgPx->pixel; // add pixel to its better class
}

int Update(ImgPxClasse *iPxC) {
  int changed = 0;
  // compute the new center, it represents an hypothetical pixel whom's neighbours value are homogeneous
  for (size_t i = 0; i < iPxC->nbClasses; ++i) {
    int old = iPxC->classes[i].massCenter;
    iPxC->classes[i].massCenter = pxMedian(&iPxC->classes[i]);
    changed = changed || old != iPxC->classes[i].massCenter; // if it has changed, then changed will be != 0
  }

  if (!changed) return 0;
  for (size_t i = 0; i < iPxC->nbClasses; ++i) {
    iPxC->classes[i].nbPx = 0;
  }
  for (size_t i = 0; i < iPxC->nbLine; ++i) {
    for (size_t j = 0; j < iPxC->nbCol; ++j) {
      linkPxClass(iPxC, &iPxC->img[i][j]); // need to update the link of pixels and set classes array.
    }
  }

  return 1;
}
