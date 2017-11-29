/*
** Gameboy emulator by Louis Giesen
** ref: http://gbdev.gg8.se/wiki/articles/The_Cartridge_Header
*/

#include <stdio.h>
#include <unistd.h>

#include "utils.h"
#include "header.h"

int calculateChecksum()
{
  /* x=0:FOR i=0134h TO 014Ch:x=x-MEM[i]-1:NEXT */
  /* TODO LATER */
  return (0);
}

void dumpHeader(struct s_gbHeader *gbHeader)
{
  fprintf(stdout, "BEGIN CODE\t\t: ");
  hexDumpBuffer(gbHeader->beginCode, 0x4);
  fprintf(stdout, "NINTENDO\t\t: ");
  hexDumpBuffer(gbHeader->nintendo, 0x30);
  fprintf(stdout, "TITLE\t\t\t: %s\n", gbHeader->title);
  fprintf(stdout, "GAMEBOY COLOR ?\t\t: ");
  if (gbHeader->color[0] == 0x80)
    fprintf(stdout, "YES\n");
  else
    fprintf(stdout, "NO\n");
  fprintf(stdout, "LICENSEE\t\t: ");
  hexDumpBuffer(gbHeader->licensee, 0x2);
  fprintf(stdout, "GAMEBOY OR SUPERGB ?\t: ");
  if (gbHeader->color[0] == 0x00)
    fprintf(stdout, "GB\n");
  else
    fprintf(stdout, "SGB\n");
  fprintf(stdout, "CARTRIDGE\t\t: ");
  hexDumpBuffer(gbHeader->cartridge, 0x1);
  fprintf(stdout, "ROM SIZE\t\t: ");
  hexDumpBuffer(gbHeader->rom, 0x1);
  fprintf(stdout, "RAM SIZE\t\t: ");
  hexDumpBuffer(gbHeader->ram, 0x1);
  fprintf(stdout, "DESTINATION\t\t: ");
  if (gbHeader->dest[0] == 0x00)
    fprintf(stdout, "JAPANESE\n");
  else
    fprintf(stdout, "NON-JAPANESE\n");
  fprintf(stdout, "CHECKSUM\t\t: ");
  hexDumpBuffer(gbHeader->checksum, 0x2);
}

/*
** Reads the RML (bytes 0x00 to 0x100) from the gameboy rom
*/
int reservedMemoryLocation(int fd)
{
  unsigned char buffer[0x100];
  int sizeRead = 0;

  if ((sizeRead = read(fd, buffer, 0x100)) == -1)
    return (-1);
  /* DO SOMETHING WITH THESE INFOS */
  /* hexDumpBuffer(buffer, 0x100); */
  /* printf("\n---------------\n"); */
  return (0);
}

int getHeaderInformations(int fd, struct s_gbHeader *gbHeader)
{
  unsigned char buffer[256];
  int sizeRead = 0;

  /* Begin code execution point */
  if ((sizeRead = read(fd, gbHeader->beginCode, 0x4)) == -1)
    return (-1);

  /* Nintendo graphic */
  if ((sizeRead = read(fd, gbHeader->nintendo, 0x30)) == -1)
    return (-1);

  /* Title of the game */
  if ((sizeRead = read(fd, gbHeader->title, 0xf)) == -1)
    return (-1);

  /* Is the game for gameboy color or not */
  if ((sizeRead = read(fd, gbHeader->color, 0x1)) == -1)
    return (-1);

  /* Licensee code */
  if ((sizeRead = read(fd, gbHeader->licensee, 0x2)) == -1)
    return (-1);

  /* Gameboy or super gameboy ? */
  if ((sizeRead = read(fd, gbHeader->superGB, 0x1)) == -1)
    return (-1);

  /* Cartridge type */
  if ((sizeRead = read(fd, gbHeader->cartridge, 0x1)) == -1)
    return (-1);

  /* Rom size */
  if ((sizeRead = read(fd, gbHeader->rom, 0x1)) == -1)
    return (-1);

  /* Ram size */
  if ((sizeRead = read(fd, gbHeader->ram, 0x1)) == -1)
    return (-1);

  /* Destination */
  if ((sizeRead = read(fd, gbHeader->dest, 0x1)) == -1)
    return (-1);

  /* USELESS FOR US */
  /* SEE LATER */
  if ((sizeRead = read(fd, buffer, 0x3)) == -1)
    return (-1);

  /* Checksum */
  if ((sizeRead = read(fd, gbHeader->checksum, 0x2)) == -1)
    return (-1);
  return (0);  
}
