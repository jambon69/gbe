/*
** Gameboy Emulator by Louis Giesen
** Debugger (GDB-like)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugger.h"
#include "emulate.h"
#include "machine.h"
#include "registers.h"

#define DBG_USAGE "|s: print stack|n: next instruction|c: continue until next bp|b: add bp|d: disassembly|"
#define DEBUG_OPTIONS 3

/*
** add breakpoint
*/
void add_breakpoint(char *follow, long **breakpoints)
{
  (*breakpoints)[0] = (*breakpoints)[0] + 1;
  (*breakpoints)[(*breakpoints)[0]] = atol(follow);
}

/*
** Special action following user input
*/
int get_user_input(long **breakpoints, FILE *fd)
{
  (void)fd;
  char *buffer = NULL;
  size_t len = 0;
  int sread = 0;
  static int ret_value = CONTINUE;

  fprintf(stdout, "\n---------------------------------------------------------------------------------------\n");
  fprintf(stdout, "%s\n", DBG_USAGE);
  fprintf(stdout, "---------------------------------------------------------------------------------------\n");
  fprintf(stdout, "-> ");
  if ((sread = getline(&buffer, &len, stdin)) == -1) {
    free(buffer);
    return (-1);
  }

  buffer[sread - 1] = '\0'; // getline takes the '\n' into account

  switch(buffer[0]) {
  case 'c':
    ret_value = CONTINUE;
    break;
  case 'n':
    ret_value = BLOCK;
    break;
  case 'b':
    ret_value = BREAK;
    if (strlen(buffer) > 2) {
      add_breakpoint(buffer + 2, breakpoints);
    }
    break;
  case 'd':
    ret_value = BREAK;
    if (strlen(buffer) > 2) {
      get_x_next_instru(fd, atoi(buffer + 2));
    }
    break;
  }
  free(buffer);
  return (ret_value);
}

/*
** Get the next X instructions (debugging purpose)
** (gdb-style don't forget ;) )
*/
void get_x_next_instru(FILE *fd, int nb)
{
  unsigned char instruction = 0x00;
  unsigned char operands[2];
  unsigned long save = ftell(fd);
  unsigned int fd_pos;

  for (int k = 0; k < nb; ++k) {
    /* Save pos of fd */
    fd_pos = (unsigned int)ftell(fd);

    /* Read instruction from the gb file */
    fread(&instruction, sizeof(unsigned char), 0x1, fd);

    /* Check if the instruction is known */
    if (instructions[instruction].disass == NULL) {
      fprintf(stdout, "%04x|\tUNKNOWN INSTRUCTION 0x%02x\n", fd_pos, instruction);
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
  get_x_next_instru(fd, 5);
  fprintf(stdout, "-------------------------------\n");
  return (0);
}
