/*
** Header file for debugger.c
*/

#ifndef DEBBUGER_H
#define DEBUGGER_H

#define CONTINUE 0
#define BLOCK 1
#define BREAK 2

#include <stdio.h>

int get_user_input(long **breakpoints, FILE *fd);
int aff_instructions(unsigned char instruction, unsigned char operands[2], FILE *fd);
void get_x_next_instru(FILE *fd, int nb);

#endif
