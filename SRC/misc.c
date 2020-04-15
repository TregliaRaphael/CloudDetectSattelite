#include "misc.h"

#ifndef GUIPREV
#define GUIPREV
int priv_gui = 0;
#endif

void SetGuiMode() {
  priv_gui = 1;
}

int GetGuiMode() {
  return priv_gui;
}