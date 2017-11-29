/*
** Gameboy Emulator by Louis Giesen
*/

#include <stdio.h>
#include <string.h>

#include "emulate.h"
#include "registers.h"
#include "utils.h"

/*
** Starting the state machine
*/
int emulates(FILE *fd, struct s_gbHeader *headers)
{
  unsigned char instruction = 0x00;
  FILE *logFile = fopen("log.txt", "w+");

  /*
  ** Call to fseek to get at position 0x100 which is the start of the program
  ** SEEK_SET is the start of the file
  */
  fseek(fd, 0x100, SEEK_SET);
  while (1)
    {
      fread(&instruction, sizeof(unsigned char), 0x1, fd);
      /* TO REPLACE WITH A POINTER ON FUNCTION TABLE */
      switch (instruction) {
      case 0x00:
	/* NOP */
	fwrite("NOP\n", sizeof(unsigned char), strlen("NOP\n"), logFile);
	continue ;
      default:
	fprintf(stdout, "Unknow instruction: 0x%02x\n", instruction);
	return (-1);
      }
    }
  return (0);
}
