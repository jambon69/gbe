/*
** Gameboy Emulator by Louis Giesen
*/

#include <stdio.h>
#include <string.h>

#include "emulate.h"
#include "registers.h"
#include "utils.h"

/* jp_nn instruction, jump to nn */
void jp_nn(FILE *fd, unsigned char *operands)
{
  int to_jump = operands[1] * 256 + operands[0]; // 256 is 16*16
  fseek(fd, to_jump, SEEK_SET);
}

/* nop instruction, does nothing */
void nop(FILE *fd, unsigned char *operands)
{
  (void)fd;
  (void)operands;
}

/*
** Starting the state machine
*/
int emulates(FILE *fd, struct s_gbHeader *headers)
{
  (void)headers;
  unsigned char instruction = 0x00;
  FILE *logFile = fopen("disassembly.txt", "w+");
  unsigned char operands[2];

  /*
  ** Call to fseek to get at position 0x100 which is the start of the program
  ** SEEK_SET is the start of the file
  */
  fseek(fd, 0x100, SEEK_SET);
  while (1)
    {
      /* Read instruction from the gb file */
      fread(&instruction, sizeof(unsigned char), 0x1, fd);

      /* Check if the instruction is known */
      if (instructions[instruction].disass == NULL) {
	fprintf(logFile, "UNKNOWN INSTRUCTION 0x%02x\n", instruction);
	fprintf(stderr, "UNKNOWN INSTRUCTION 0x%02x\n", instruction);
	return (-1);
      }

      /* Get the operands in the file (if needed) */
      fread(operands, sizeof(unsigned char), instructions[instruction].operandSize, fd);

      /* log the disassembly */
      fprintf(logFile, "%s", instructions[instruction].disass);
      for (int i = 0; i < instructions[instruction].operandSize; ++i) {
	fprintf(logFile, " 0x%02x", operands[i]);
      }
      fprintf(logFile, "\n");

      /* Call the corresponding function */
      instructions[instruction].funcptr(fd, operands);
    }
  return (0);
}
