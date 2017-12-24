/*
** Gameboy Emulator by Louis Giesen
** Load assembly instructions
*/

#include <stdio.h>
#include "emulate.h"
#include "registers.h"

void ld_sp_nn(FILE *fd, unsigned char *operands)
{
  (void)fd;
  registers.sp = operands[1] * 256 + operands[0];
}

/* load memory spot $FF00+n into register A */
void ldh_a_n(FILE *fd, unsigned char *operands)
{
  (void)fd;
  registers.a = memory[0xFF00 + operands[0]];
}

/* load register a at memory spot $FF00+n */
void ldh_n_a(FILE *fd, unsigned char *operands)
{
  (void)fd;
  memory[0xFF00 + operands[0]] = registers.a;
}

/* load n into register A*/
void ld_a_n(FILE *fd, unsigned char *operands)
{
  (void)fd;
  registers.a = operands[0];
}

/* load a into hl */
void ld_hl_a(FILE *fd, unsigned char *operands)
{
  (void)fd;
  (void)operands;
  registers.hl = registers.a;
}

/* load n into B register */
void ld_b_n(FILE *fd, unsigned char *operands)
{
  (void)fd;
  registers.b = operands[0];
}

/* load n into C register */
void ld_c_n(FILE *fd, unsigned char *operands)
{
  (void)fd;
  registers.c = operands[0];
}

/* ld nn into HL register */
void ld_hl_nn(FILE *fd, unsigned char *operands)
{
  (void)fd;
  registers.hl = operands[1] * 256 + operands[0];
}
