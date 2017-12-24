/*
** Gameboy emulator by Louis Giesen
*/

#include <stdio.h>

#include <string.h>
#include "args.h"

void parseArgs(struct s_Args *strArgs, int ac, char **av)
{
  strArgs->header = 0;
  strArgs->debug = 0;
  strArgs->log = 0;

  for (int i = 1; i < ac; ++i) {
    if (strcmp(av[i], "-h") == 0)
      strArgs->header = 1;
    else if (strcmp(av[i], "-d") == 0)
      strArgs->debug = 1;
    else if (strcmp(av[i], "-l") == 0)
      strArgs->log = 1;
  }
}
