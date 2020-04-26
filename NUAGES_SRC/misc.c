#include "misc.h"

#include <dirent.h>
#include <string.h>

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

void automatics(char* self, int(*main)(int, char**)) {
  DIR *d = opendir(".");
  struct dirent *dir;
  if (!d) return perror("error in opening current directory");
  while ((dir = readdir(d))) {
    if (strcmp(self, dir->d_name) == 0) continue;
    if (dir->d_name[0] == '.') continue;
    char *argv[2] = {self, dir->d_name};
    if (main(2, argv)) return perror("error during compute");
    printf("\n");
  }
}