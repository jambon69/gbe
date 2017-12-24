/*
** Gameboy Emulator by Louis Giesen
** Debugger (GDB-like)
*/

#include <stdio.h>
#include <stdlib.h>

#include "machine.h"
#include "registers.h"

static void get_five_next_instru(FILE *fd)
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
