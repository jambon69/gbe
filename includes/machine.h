/*
** Header file for machine.c
*/

#ifndef MACHINE_H
#define MACHINE_H

#include "emulate.h"
#include <stdio.h>

typedef struct s_instructions
{
  char *disass;
  unsigned char operandSize;
  void (*funcptr)(FILE *, unsigned char *);
} t_instructions;

extern struct s_instructions instructions[256];


#endif
