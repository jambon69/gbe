/*
** Gameboy Emulator by Louis Giesen
** Jump assembly instructions
*/

#include <stdio.h>
#include "registers.h"

/* jump if zflag is reset */
void jr_nz_n(FILE *fd, unsigned char *operands)
{
  /* if z flag is reset */
  if (registers.z_flag == 0) {
    fseek(fd, (char)operands[0], SEEK_CUR);
  }
}

/* jp_nn instruction, jump to nn */
void jp_nn(FILE *fd, unsigned char *operands)
{
  int to_jump = operands[1] * 256 + operands[0]; // 256 is 16*16
  fseek(fd, to_jump, SEEK_SET);
}
