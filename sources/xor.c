/*
** Gameboy Emulator by Louis Giesen
** Xor assembly instructions
*/

#include <stdio.h>
#include "registers.h"

/* xor a instruction */
void xor_a(FILE *fd, unsigned char *operands)
{
  (void)fd;
  (void)operands;
  registers.a = registers.a ^ registers.a;
}
