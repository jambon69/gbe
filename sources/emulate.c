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
  memset(memory + 0xff00, 0x00, 0x007f);
  memory[0xff05] = 0x00; // TIMA
  memory[0xff06] = 0x00; // TMA
  memory[0xff07] = 0x00; // TAC
  memory[0xff10] = 0x80; // NR10
  memory[0xff11] = 0xbf; // NR11
  memory[0xff12] = 0xf3; // NR12
  memory[0xff14] = 0xbf; // NR14
  memory[0xff16] = 0x3f; // NR21
  memory[0xff17] = 0x00; // NR22
  memory[0xff19] = 0xbf; // NR24
  memory[0xff1a] = 0x7f; // NR30
  memory[0xff1b] = 0xff; // NR31
  memory[0xff1c] = 0x9f; // NR32
  memory[0xff1e] = 0xbf; // NR33
  memory[0xff20] = 0xff; // NR41
  memory[0xff21] = 0x00; // NR42
  memory[0xff22] = 0x00; // NR43
  memory[0xff23] = 0xbf; // NR30
  memory[0xff24] = 0x77; // NR50
  memory[0xff25] = 0xf3; // NR51
  memory[0xff26] = 0xf1; // NR52
  memory[0xff40] = 0x91; // LCDC  -> LCD CONTROL
  memory[0xff41] = 0x85; // STAT  -> LCD STATUS
  memory[0xff42] = 0x00; // SCY   -> SCROLL Y
  memory[0xff43] = 0x00; // SCX   -> SCROLL X
  memory[0xff44] = 0x00; // LY  -> LCDC Y-COORDINATE
  memory[0xff45] = 0x00; // LYC -> LY COMPARE
  memory[0xff47] = 0xfc; // BGP
  memory[0xff48] = 0xff; // OBP0
  memory[0xff49] = 0xff; // OBP1
  memory[0xff4a] = 0x00; // WY
  memory[0xff4b] = 0x00; // WX

  /* Fill HRAM section 0xff80 -> 0xffff */
  memset(memory + 0xff80, 0x00, 0x007f);
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
  /* if gameboy -> a = 0x01 */
  registers.af = 0x01b0;
  registers.bc = 0x0013;
  registers.de = 0x00d8;
  registers.hl = 0x014d;
  registers.z_flag = 1;
  registers.n_flag = 0;
  registers.h_flag = 0;
  registers.c_flag = 1;
  registers.sp = 0xfffe;

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
