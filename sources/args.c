/*
** Gameboy emulator by Louis Giesen
*/

#include <string.h>
#include "args.h"

void parseArgs(struct s_Args *strArgs, int ac, char **av)
{
  strArgs->header = 0;
  
  for (int i = 1; i < ac; ++i)
    {
      if (strcmp(av[i], "-h") == 0)
	strArgs->header = 1;
    }
}
