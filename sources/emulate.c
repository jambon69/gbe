/*
** Gameboy Emulator by Louis Giesen
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "emulate.h"
#include "registers.h"
#include "utils.h"

unsigned char memory[0xffff];

void get_five_next_instru(FILE *fd)
{
  unsigned char instruction = 0x00;
  unsigned char operands[2];
  unsigned long save = ftell(fd);
  unsigned int fd_pos;

  for (int k = 0; k < 5; ++k) {
    /* Save pos of fd */
    fd_pos = (unsigned int)ftell(fd);

    /* Read instruction from the gb file */
    fread(&instruction, sizeof(unsigned char), 0x1, fd);

    /* Check if the instruction is known */
    if (instructions[instruction].disass == NULL) {
      fprintf(stdout, "UNKNOWN INSTRUCTION 0x%02x\n", instruction);
      continue;
    }

    /* Get the operands in the file (if needed) */
    fread(operands, sizeof(unsigned char), instructions[instruction].operandSize, fd);

    /* opcode + disassembly */
    fprintf(stdout, "%04x|\t%02x", fd_pos, instruction);
    for (int i = 0; i < instructions[instruction].operandSize; ++i) {
      fprintf(stdout, "%02x", operands[i]);
    }

    fprintf(stdout, "\t");
    fprintf(stdout, "%s", instructions[instruction].disass);
    for (int i = 0; i < instructions[instruction].operandSize; ++i) {
      fprintf(stdout, " 0x%02x", operands[i]);
    }
    fprintf(stdout, "\n");
  }
  fseek(fd, save, SEEK_SET);
}

/*
** Debugging function
** TODO: PUT NCURSES (Beautifier)
*/
int aff_instructions(unsigned char instruction, unsigned char operands[2], FILE *fd)
{
  system("clear");
  /* Registers + Flags */
  fprintf(stdout, "-------------------------------\n");
  fprintf(stdout, "-------------REGISTERS---------\n");
  fprintf(stdout, "-------------------------------\n");
  fprintf(stdout, "AF: %02x%02x\n", registers.a, registers.f);
  fprintf(stdout, "BC: %02x%02x\n", registers.b, registers.c);
  fprintf(stdout, "DE: %02x%02x\n", registers.d, registers.e);
  fprintf(stdout, "HL: %02x%02x\n", registers.h, registers.l);
  fprintf(stdout, "SP: %04x\n", registers.sp);
  fprintf(stdout, "PC: %04x\n", registers.pc);
  fprintf(stdout, "-------------------------------\n");
  fprintf(stdout, "--------------FLAGS------------\n");
  fprintf(stdout, "-------------------------------\n");
  fprintf(stdout, "Z: %04x\n", registers.z_flag);
  fprintf(stdout, "C: %04x\n", registers.c_flag);
  fprintf(stdout, "-------------------------------\n");

  /* opcode + disassembly */
  fprintf(stdout, "%04x|\t%02x", registers.pc, instruction);
  for (int i = 0; i < instructions[instruction].operandSize; ++i) {
    fprintf(stdout, "%02x", operands[i]);
  }

  fprintf(stdout, "\t");
  fprintf(stdout, "%s", instructions[instruction].disass);
  for (int i = 0; i < instructions[instruction].operandSize; ++i) {
    fprintf(stdout, " 0x%02x", operands[i]);
  }
  fprintf(stdout, "\n");
  /* Get the five next instructions (debugging purpose) */
  get_five_next_instru(fd);
  fprintf(stdout, "-------------------------------\n");

  /* Returns something with return of getchar */
  getchar();
  return (0);
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

      /* /\* Check if the instruction is known *\/ */
      /* if (instructions[instruction].disass == NULL) { */
      /* 	fprintf(logFile, "UNKNOWN INSTRUCTION 0x%02x\n", instruction); */
      /* 	fprintf(stderr, "UNKNOWN INSTRUCTION 0x%02x\n", instruction); */
      /* 	return (-1); */
      /* } */

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
