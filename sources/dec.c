/*
** Gameboy Emulator y Louis Giesen
** dec assembly instructions
*/

#include <stdio.h>
#include "registers.h"

/* Decrement register c */
void dec_c(FILE *fd, unsigned char *operands)
{
  (void)fd;
  (void)operands;
  --registers.c;
  /* if result of a math operation is zero, z_flag is set */
  if (registers.c == 0)
    registers.z_flag = 1;
  else
    registers.z_flag = 0;
  /* If substraction was performed, n_flag is set */
  registers.n_flag = 1;
}

/* Decrement register b */
void dec_b(FILE *fd, unsigned char *operands)
{
  (void)fd;
  (void)operands;
  --registers.b;
  /* if restult of a math operation is zero, z_flag is set */
  if (registers.b == 0)
    registers.z_flag = 1;
  else
    registers.z_flag = 0;
  /* If substraction was performed, n_flag is set */
  registers.n_flag = 1;
}
