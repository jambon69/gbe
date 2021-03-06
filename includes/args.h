/*
** Header file for args.c
*/

#ifndef ARGS_H
# define ARGS_H

/*
** Structures
*/
typedef struct s_Args
{
  unsigned short header;
  unsigned short debug;
  unsigned short log;
} t_Args;

/*
** functions
*/
void parseArgs(struct s_Args *, int ac, char **av);

#endif
