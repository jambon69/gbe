/*
** Useful functions
*/

#include <stdio.h>

void hexDumpBuffer(unsigned char *buffer, int size)
{
  for (int i = 0; i < size; ++i)
    {
      fprintf(stdout, "%02x ", buffer[i]);
    }
  fprintf(stdout, "\n");
}

void cleanBuffer(unsigned char *buffer, int size)
{
  for (int i = 0; i < size; ++i)
    {
      buffer[i] = '\0';
    }  
}
