/*
** Gameboy Emulator by Louis Giesen
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "debugger.h"
#include "emulate.h"
#include "registers.h"
#include "utils.h"

unsigned char memory[0xffff];

void fill_stack(FILE *fd)
{
  long save_fd = ftell(fd);
  char prog[0x7fff];

  fseek(fd, 0, SEEK_SET);
  fread(prog, sizeof(char), 0x7fff, fd);
  
  memcpy(memory, prog, 0x7fff);
  fseek(fd, save_fd, SEEK_SET);
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
int emulates(FILE *fd, struct s_gbHeader *headers, unsigned short debug)
{
  (void)headers;
  unsigned char instruction = 0x00;
  FILE *logFile = fopen("disassembly.txt", "w+");
  unsigned char operands[2];

  /* fill the stack */
  fill_stack(fd);
  
  /*
  ** Set the registers
  ** (MAY WANT TO DO THIS SOMEWHERE ELSE
  */ 
  registers.z_flag = 1;
  registers.c_flag = 1;

  /*
  ** Call to fseek to get at position 0x100 which is the start of the program
  ** SEEK_SET is the start of the file
  */
  fseek(fd, 0x100, SEEK_SET);
  while (1)
    {
      /* set pc at the new offset */
      registers.pc = (short)ftell(fd);

      /* Read instruction from the gb file */
      fread(&instruction, sizeof(unsigned char), 0x1, fd);

      /* Check if the instruction is known */
      if (instructions[instruction].disass == NULL) {
      	/* fprintf(logFile, "UNKNOWN INSTRUCTION 0x%02x\n", instruction); */
      	fprintf(stderr, "UNKNOWN INSTRUCTION 0x%02x\n", instruction);
      	return (-1);
      }

      /* Get the operands in the file (if needed) */
      fread(operands, sizeof(unsigned char), instructions[instruction].operandSize, fd);

      /* /\* log the disassembly *\/ */
      /* fprintf(logFile, "%s", instructions[instruction].disass); */
      /* for (int i = 0; i < instructions[instruction].operandSize; ++i) { */
      /* 	fprintf(logFile, " 0x%02x", operands[i]); */
      /* } */
      /* fprintf(logFile, "\n"); */

      /* Debugger */
      if (debug == 1) {
	aff_instructions(instruction, operands, fd);
      }

      /* Call the corresponding function */
      instructions[instruction].funcptr(fd, operands);
    }
  return (0);
}
