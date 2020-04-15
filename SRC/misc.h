#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void SetGuiMode();
int GetGuiMode();

void GUI(int argc, char **argv);

static void printUsage(const char* prgName) {
  printf("Usage: \n\t- %s <filename...>\n\t- %s -e <filename>\n", prgName, prgName);
  printf("\t\t* <filename...>: a suit a filename on which %s will be execute\n", prgName);
  printf("\t\t* -e: return the integer percentage of clouds in filename <filename>, or 255 on error\n");
}

static void rescue(int argc, char **argv) {
  char rep = '\0';
  do {
    printf("Would you like to run the GUI executable ? (y/n)\n");
    scanf("%c", &rep);
    if (rep == 'y') {
      GUI(argc, argv);
      exit(0);
    } else if (rep == 'n') {
      printf("Ok, bye\n");
      exit(EXIT_FAILURE);
    }
    printf("\\33[2Ksorry, expected 'y' for yes or 'n' for no.\n");
  } while(1);
}


#endif
