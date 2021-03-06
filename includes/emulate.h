/*
** Header file for emulates.c and all the assembly instructions
*/

#ifndef EMULATES_H
#define EMULATES_H

#include <stdio.h>

#include "args.h"
#include "header.h"
#include "machine.h"

#define BREAKPOINTS 20

/*
** Structures
*/

/*
** Functions
*/
int emulates(FILE *fd, struct s_gbHeader *headers, struct s_Args *args);

/* NOP */
void nop(FILE *fd, unsigned char *operands);

/* jump */
void jp_nn(FILE *fd, unsigned char *operands);
void jr_nz_n(FILE *fd, unsigned char *operands);

/* xor */
void xor_a(FILE *fd, unsigned char *operands);

/* load */
void ld_sp_nn(FILE *fd, unsigned char *operands);
void ld_hl_nn(FILE *fd, unsigned char *operands);
void ld_c_n(FILE *fd, unsigned char *operands);
void ld_b_n(FILE *fd, unsigned char *operands);
void ld_hl_a(FILE *fd, unsigned char *operands);
void ld_a_n(FILE *fd, unsigned char *operands);
void ldh_n_a(FILE *fd, unsigned char *operands);
void ldh_a_n(FILE *fd, unsigned char *operands);

/* dec */
void dec_c(FILE *fd, unsigned char *operands);
void dec_b(FILE *fd, unsigned char *operands);

/* comp */
void cp_a_n(FILE *fd, unsigned char *operands);

extern unsigned char memory[0xffff];

#endif
