/*
** Gameboy emulator by Louis Giesen
** cmp assembly instructions
*/

#include <stdio.h>
#include "registers.h"

/* Compare a with n */
void cp_a_n(FILE *fd, unsigned char *operands)
{
  (void)fd;
  if (registers.a == operands[0]) {
    registers.z_flag = 1;
  }
  else {
    registers.z_flag = 0;
  }
  if (registers.a < operands[0]) {
    registers.c_flag = 1;
  }
  else {
    registers.c_flag = 0;
  }
}
