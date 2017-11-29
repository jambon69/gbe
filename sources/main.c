/*
** Gameboy emulator by Louis Giesen
** http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "emulate.h"
#include "header.h"
#include "args.h"
#include "registers.h"

/*
** USAGE if no arguments
*/
void usage(char *prog)
{
  fprintf(stderr,
	  "USAGE : \n"
	  "\t%s file.gb [options]\n"
	  "options : \n"
	  "\t-h: Get Header informations\n", prog);
}

/*
** main function
*/
int main(int ac, char **av, char **env)
{
  (void)env;
  if (ac == 1) {
    usage(av[0]);
    return (-1);
  }

  struct s_Args args;
  struct s_gbHeader gbHeader;
  parseArgs(&args, ac, av);
  FILE  *fd = fopen(av[1], "r");

  reservedMemoryLocation(fd);
  getHeaderInformations(fd, &gbHeader);
  if (args.header == 1) {
    dumpHeader(&gbHeader);
    fprintf(stdout, "-----------------------------------------------------------\n");
  }
  if (emulates(fd, &gbHeader) == -1) {
    fprintf(stderr, "Something went wrong with the emulation\n");
    return (-1);
  }
  return (0);
}
