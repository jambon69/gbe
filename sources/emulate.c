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

  /* Fill program in ROM0/ROM1 section 0x0000 -> 0x7fff */
  fseek(fd, 0, SEEK_SET);
  fread(prog, sizeof(char), 0x7fff, fd);
  memcpy(memory, prog, 0x7fff);

  /* Fill VRA0 section 0x8000 -> 0x9ffff */
  memset(memory + 0x7fff, 0x00, 0x1fff);

  /* Fill SRA0 section 0xa000 -> 0xbfff */
  memset(memory + 0xa000, 0xff, 0x1fff);

  /* Fill WRA0 section 0xc000 -> 0xcfff */
  memset(memory + 0xc000, 0x00, 0x0fff);

  /* Fill WRA1 section 0xd000 -> 0xdfff */
  memset(memory + 0xd000, 0x00, 0x0fff);

  /* Fill ECH0 section 0xe000 -> 0xefff */
  memset(memory + 0xe000, 0x00, 0x0fff);

  /* Fill ECH1 section 0xf000 -> 0xfdff */
  memset(memory + 0xf000, 0x00, 0x0dff);

  /* Fill OAM section 0xfe00 -> 0xfe9a */
  memset(memory + 0xfe00, 0x00, 0x009a);

  /* Fill ---- section 0xfe9b -> 0xfeff */
  memset(memory + 0xfe9b, 0x00, 0x0064);

  /* Fill I/O section 0xff00 -> 0xff7f */
  memset(memory + 0xff00, 0xff, 0x007f);

  /* Fill HRAM section 0xff80 -> 0xffff */
  memset(memory + 0xff80, 0x00, 0x007f);

  /* hexDumpBuffer(memory, 0xffff); */
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
int emulates(FILE *fd, struct s_gbHeader *headers, struct s_Args *args)
{
  (void)headers;
  unsigned char instruction = 0x00;
  FILE *logFile = fopen("disassembly.txt", "w+");
  unsigned char operands[2];
  long *breakpoints;
  int block = 0;

  /* fill breakpoint 0 at 0x100 (start of the program) */
  if ((breakpoints = malloc(sizeof(long) * BREAKPOINTS)) == NULL)
    return (-1);
  breakpoints[0] = 0x0;

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
  if (args->debug == 1) { 
    /* As long as we chose breakpoints or print stack, we continue to ask */
    while ((block = get_user_input(&breakpoints, fd)) == BREAK);
  }
  while (1)
    {
      /* set pc at the new offset */
      registers.pc = (short)ftell(fd);

      /* Read instruction from the gb file */
      fread(&instruction, sizeof(unsigned char), 0x1, fd);

      /* Check if the instruction is known */
      if (instructions[instruction].disass == NULL) {
	if (args->log == 1) {
	  fprintf(logFile, "UNKNOWN INSTRUCTION 0x%02x\n", instruction);
	}
      	fprintf(stderr, "UNKNOWN INSTRUCTION 0x%02x\n", instruction);
      	return (-1);
      }

      /* Get the operands in the file (if needed) */
      fread(operands, sizeof(unsigned char), instructions[instruction].operandSize, fd);

      /* log the disassembly */
      if (args->log == 1) {
	fprintf(logFile, "%s", instructions[instruction].disass);
	for (int i = 0; i < instructions[instruction].operandSize; ++i) {
	  fprintf(logFile, " 0x%02x", operands[i]);
	}
	fprintf(logFile, "\n");
      }

      if (args->debug == 1) {
	if (block == BLOCK) {
	  /* Debugger then ask for new instruction */
	  aff_instructions(instruction, operands, fd);
	  while ((block = get_user_input(&breakpoints, fd)) == BREAK);
	}
	else {
	  /* check for breakpoints */
	  for (long i = 1; i < breakpoints[0] + 1; ++i) {
	    if (breakpoints[i] == registers.pc) {
	      /* Debugger then ask for new instruction */
	      aff_instructions(instruction, operands, fd);
	      while ((block = get_user_input(&breakpoints, fd)) == BREAK);
	      break ;
	    }
	  }
	}
      }

      /* Call the corresponding function */
      instructions[instruction].funcptr(fd, operands);
    }
  free(breakpoints);
  return (0);
}
