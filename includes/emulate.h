/*
** Header file for emulates.c
*/

#ifndef EMULATES_H
#define EMULATES_H

#include "header.h"
#include "machine.h"
#include <stdio.h>

/*
** Structures
*/

/*
** Functions
*/
int emulates(FILE *fd, struct s_gbHeader *headers);
void nop(FILE *fd, unsigned char *operands);
void jp_nn(FILE *fd, unsigned char *operands);

#endif
