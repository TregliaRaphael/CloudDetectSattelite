#include "compute.h"
#include <stdlib.h>

ImgPxClasse* baseInit(ImgInfo info, size_t nbClasses) {
  ImgPxClasse *iPxC = calloc(1, sizeof(ImgPxClasse));
  iPxC->nbLine = info.NbLine - 2; // minus the first and the last
  iPxC->nbCol = info.NbCol - 2; // minus the first and the last
  iPxC->nbClasses = nbClasses;
  iPxC->classes = calloc(nbClasses, sizeof(Classe));
  for (size_t i = 0; i < nbClasses; ++i) {
    Classe cls = {
        .massCenter = i * (255 / (nbClasses - 1)),
        .pixels = calloc(info.TotPix, sizeof(guchar *)),
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

  Pixel *tmp = greyImg;
  Pixel **img = calloc(info.NbLine, sizeof(Pixel*));

  for (size_t i = 0; i < info.NbLine; ++i) {
    img[i] = &tmp[i * info.NbCol];
  }

  ImgPxClasse *iPxC = baseInit(info, nbClasses);

  for (size_t i = 0, i_img = 1; i_img < info.NbLine - 1; ++i_img, ++i) {
    iPxC->img[i] = calloc(iPxC->nbCol, sizeof(PixelClasse));
    for (size_t j = 0, j_img = 1; j_img < info.NbCol - 1; ++j_img, ++j) {
      iPxC->img[i][j].pixel = &img[i_img][j_img]; // no need to make a copy...

      guchar *neighbors[5] = { &img[i_img][j_img], &img[i_img - 1][j_img], &img[i_img + 1][j_img], &img[i_img][j_img - 1],  &img[i_img][j_img + 1] };
      neighborsImg(iPxC, &iPxC->img[i][j], neighbors);
      // here the pixels and neighbors of them are grouped and sorted.
      // set default classe for all of them.
      baseClasses(&iPxC->img[i][j], iPxC);
      //iPxC->img[i][j].classe->pixels = realloc(iPxC->img[i][j].classe->pixels, ++iPxC->img[i][j].classe->nbPx * sizeof(guchar *)); // check if failed ?
      iPxC->img[i][j].classe->pixels[iPxC->img[i][j].classe->nbPx++] = iPxC->img[i][j].pixel; // maybe useless, to see...
    }
  }
  // No need img anymore.
  free(img);
  return iPxC;
}

void DeleteImgPxClasse(ImgPxClasse *iPxC) {
  for (size_t i = 0; i < iPxC->nbClasses; ++i) {
    free(iPxC->classes[i].pixels);
  }
  free(iPxC->classes);
  for (size_t i = 0; i < iPxC->nbLine; ++i) {
    free(iPxC->img[i]);
  }
  free(iPxC->img);
  free(iPxC);
}