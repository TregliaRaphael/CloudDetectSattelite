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

ImgPxClasse* baseInit(ImgInfo info, size_t nbClasses) {
  ImgPxClasse *iPxC = calloc(1, sizeof(ImgPxClasse));
  iPxC->nbLine = info.NbLine - 2; // minus the first and the last
  iPxC->nbCol = info.NbCol - 2; // minus the first and the last
  iPxC->nbClasses = nbClasses;
  iPxC->classes = calloc(nbClasses, sizeof(Classe));
  for (size_t i = 0; i < nbClasses; ++i) {
    Classe cls = {
        .massCenter = i * (255 / (nbClasses - 1)),
        .pixels = calloc(0, sizeof(guchar *)),
        .nbPx = 0
    }; // 0 and 255 are "center".
    iPxC->classes[i] = cls;
  }
  iPxC->img = calloc(iPxC->nbLine, sizeof(PixelClasse*));
  return iPxC;
}

void neighborsImg(ImgPxClasse *iPxC, PixelClasse *imgPx, guchar *neighbors[5]) {
  for (size_t i = 0; i < 5; ++i) { // i = sorted the neighbors
    size_t px = i;
    for (size_t j = i; j < 5; ++j) {
      if (*neighbors[j] < *neighbors[px]) {
        px = j;
      }
    }

    imgPx->sortedNeighbors[i] = neighbors[px]; // i set the neighbors (sorted)
    neighbors[px] = neighbors[i];
  }
}

void baseClasses(PixelClasse *imgPx, ImgPxClasse *iPxC) {
  imgPx->classe = &iPxC->classes[0]; // by default
  for (size_t i = 1; i < iPxC->nbClasses; ++i) {
    int nd = 0, od = 0; // new distance, old distance
    for (size_t j = 0; j < 5; ++j) {
      nd += abs(*imgPx->sortedNeighbors[j] - iPxC->classes[i].massCenter);
      od += (*imgPx->sortedNeighbors[j] - imgPx->classe->massCenter);
    }
    if (nd < od) {
      imgPx->classe = &iPxC->classes[i];
    }
  }
}

ImgPxClasse* NewImgPxClasse(ImgInfo info, size_t nbClasses, guchar *greyImg) {

 Pixel** FuckingImg = greyImg;
 FuckingImg[0][4];

 guchar ***img = calloc(info.NbLine, sizeof(guchar **));
 for (size_t i = 0; i < info.NbLine; ++i) {
   img[i] = calloc(info.NbCol, sizeof(guchar *));
 }
 {
   int line = 0, col = 0;
   for (size_t i = 0; i < info.TotPix * info.NbChan; i += info.NbChan) {
     /*int line = i / info.NbLine, col = i % info.NbLine; // [line][col] == line * NbLine + col
     if (img[line] == NULL) {
       img[line] = calloc(info.NbCol, sizeof(guchar *));
     }
     img[line][col] = &greyImg[i * info.NbChan]; // each px is over info.NbChan adr.*/

     img[line][col++] = &greyImg[i];
     if (col == info.NbCol) {
       ++line;
       col = 0;
     }
   }
 }
 ImgPxClasse *iPxC = baseInit(info, nbClasses);

 for (size_t i = 0, i_img = 1; i_img < info.NbLine - 1; ++i_img, ++i) {
   iPxC->img[i] = calloc(iPxC->nbCol, sizeof(PixelClasse));
   for (size_t j = 0, j_img = 1; j_img < info.NbCol - 1; ++j_img, ++j) {
     iPxC->img[i][j].pixel = img[i_img][j_img]; // no need to make a copy...

     guchar *neighbors[5] = { img[i_img][j_img], img[i_img - 1][j_img], img[i_img + 1][j_img], img[i_img][j_img - 1],  img[i_img][j_img + 1] };
     /*for (size_t k = 0; k < 5; ++k) { // i sort the neighbors
       size_t px = k;
       for (size_t l = k; l < 5; ++l) {
         if (*neighbors[l] < *neighbors[px]) {
           px = l;
         }
       }

       iPxC->img[i][j].sortedNeighbors[k] = neighbors[px]; // i set the neighbors (sorted)
       neighbors[px] = neighbors[k];
     }*/
     neighborsImg(iPxC, &iPxC->img[i][j], neighbors);
     // here the pixels and neighbors of them are grouped and sorted.
     // set default classe for all of them.
     /*iPxC->img[i][j].classe = &iPxC->classes[0]; // by default
     for (size_t k = 1; k < iPxC->nbClasses; ++k) {
       int nd = 0, od = 0; // new distance, old distance
       for (size_t l = 0; l < 5; ++l) {
         nd += abs(*iPxC->img[i][j].sortedNeighbors[l] - iPxC->classes[k].massCenter);
         od += (*iPxC->img[i][j].sortedNeighbors[l] - iPxC->img[i][j].classe->massCenter);
       }
       if (nd < od) {
         iPxC->img[i][j].classe = &iPxC->classes[k];
       }
     }*/
     baseClasses(&iPxC->img[i][j], iPxC);
     iPxC->img[i][j].classe->pixels = realloc(iPxC->img[i][j].classe->pixels, ++iPxC->img[i][j].classe->nbPx * sizeof(guchar *)); // check if failed ?
     iPxC->img[i][j].classe->pixels[iPxC->img[i][j].classe->nbPx - 1] = iPxC->img[i][j].pixel; // maybe useless, to see...
   }
 }
 // No need img anymore.
 for(size_t i = 0; i < info.NbLine; ++i) {
   free(img[i]); // i don't want to free the px, so i don't do: free(*img[i])
 }
 free(img);
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

  ImgPxClasse* iPxC = NewImgPxClasse(imgInf, 3, pucImaRes);
  int loopCount = 0;
  int changed = 1;
  while(loopCount++ < 500 && changed) changed = Update(iPxC);


  for (size_t i = 0; i < iPxC->classes[1].nbPx; ++i) {
    iPxC->classes[2].pixels[i][0] = 0;
    iPxC->classes[2].pixels[i][1] = 0;
    iPxC->classes[2].pixels[i][2] = 0;
  }
}



unsigned Rgb2Gray(ImgInfo imgInf, guchar* imgSrc, guchar* imgDst)
{
  guchar ucMeanPx;
  const uint Red = 0, Green = 1, Blue = 2;
  for(int iPx = 0; iPx < imgInf.TotPix * imgInf.NbChan; iPx += imgInf.NbChan) {
    guchar* px = imgSrc + iPx;
    // we add a weight for each R,G, B. That weight comes from: https://scikit-image.org/docs/dev/auto_examples/color_exposure/plot_rgb_to_gray.html?fbclid=IwAR3PC3JwvHGVxgqGHkMl11nUfZz32NcZqIK7M4NfFS6ELyKIiJ2ah_u2NVU
    ucMeanPx = (px[Red] * 2125 + px[Green] * 7154 + px[Blue] * 721)/10000;
    for(int iChan = 0; iChan < imgInf.NbChan; ++iChan) {
       imgDst[iPx + iChan] = ucMeanPx;
    }
  }
}

guchar pxMedian(Classe *cls) {
  return *cls->pixels[cls->nbPx / 2]; // the median
}

// return weather or not classe px has changed.
int linkPxClass(ImgPxClasse *iPxC, PixelClasse *imgPx) {
  int changed = 0;
  for (size_t i = 0; i < iPxC->nbClasses; ++i) {
//    free(iPxC->classes[i].pixels);
//    iPxC->classes[i].pixels = NULL;
    iPxC->classes[i].nbPx = 0;
    int nd = 0, od = 0;
    for (size_t px = 0; px < 5; ++px) {
      od += abs(*imgPx->sortedNeighbors[px] - imgPx->classe->massCenter);
      nd += abs(*imgPx->sortedNeighbors[px] - iPxC->classes[i].massCenter);
    }
    if (nd < od) {
      changed = 1;
      imgPx->classe = &iPxC->classes[i];
    }
  }
  imgPx->classe->pixels = realloc(imgPx->classe->pixels, ++imgPx->classe->nbPx * sizeof(guchar*));
  imgPx->classe->pixels[imgPx->classe->nbPx - 1] = imgPx->pixel;
  return changed;
}

int Update(ImgPxClasse *iPxC) {

  int changed = 0;
  // compute the new center, it represents an hypothetical pixel which's neighbours value are homogeneous
  for (size_t i = 0; i < iPxC->nbClasses; ++i) {
    int old = iPxC->classes[i].massCenter;
    iPxC->classes[i].massCenter = pxMedian(&iPxC->classes[i]);
    changed = changed || old != iPxC->classes[i].massCenter; // if it has changed, then changed will be != 0
  }

  for (size_t i = 0; i < iPxC->nbLine; ++i) {
    for (size_t j = 0; j < iPxC->nbCol; ++j) {
      changed = linkPxClass(iPxC, &iPxC->img[i][j]) || changed;
    }
  }

  return changed;
}
