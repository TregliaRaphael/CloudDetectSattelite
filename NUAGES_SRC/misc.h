#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void SetGuiMode();
int GetGuiMode();

void GUI(int argc, char **argv);

void automatics(char* self, int(*main)(int, char**));



#endif
