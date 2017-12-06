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

/* jump */
void jp_nn(FILE *fd, unsigned char *operands);

/* xor */
void xor_a(FILE *fd, unsigned char *operands);

/* load */
void ld_hl_nn(FILE *fd, unsigned char *operands);
void ld_c_n(FILE *fd, unsigned char *operands);
void ld_b_n(FILE *fd, unsigned char *operands);
void ld_hl_a(FILE *fd, unsigned char *operands);
void dec_b(FILE *fd, unsigned char *operands);
void jr_nz_n(FILE *fd, unsigned char *operands);

#endif
