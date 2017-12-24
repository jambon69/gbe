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
int reservedMemoryLocation(FILE *fd)
{
  unsigned char buffer[0x100];
  int sizeRead = 0;

  if ((sizeRead = fread(buffer, sizeof(unsigned char), 0x100, fd)) == -1)
    return (-1);
  /* DO SOMETHING WITH THESE INFOS */
  hexDumpBuffer(buffer, 0x100);
  printf("\n---------------\n");
  return (0);
}

int getHeaderInformations(FILE *fd, struct s_gbHeader *gbHeader)
{
  unsigned char buffer[256];
  int sizeRead = 0;

  /* Begin code execution point */
  if ((sizeRead = fread(gbHeader->beginCode, sizeof(unsigned char), 0x4, fd)) == -1)
    return (-1);

  /* Nintendo graphic */
  if ((sizeRead = fread(gbHeader->nintendo, sizeof(unsigned char), 0x30, fd)) == -1)
    return (-1);

  /* Title of the game */
  if ((sizeRead = fread(gbHeader->title, sizeof(unsigned char), 0xf, fd)) == -1)
    return (-1);

  /* Is the game for gameboy color or not */
  if ((sizeRead = fread(gbHeader->color, sizeof(unsigned char), 0x1, fd)) == -1)
    return (-1);

  /* Licensee code */
  if ((sizeRead = fread(gbHeader->licensee, sizeof(unsigned char), 0x2, fd)) == -1)
    return (-1);

  /* Gameboy or super gameboy ? */
  if ((sizeRead = fread(gbHeader->superGB, sizeof(unsigned char), 0x1, fd)) == -1)
    return (-1);

  /* Cartridge type */
  if ((sizeRead = fread(gbHeader->cartridge, sizeof(unsigned char), 0x1, fd)) == -1)
    return (-1);

  /* Rom size */
  if ((sizeRead = fread(gbHeader->rom, sizeof(unsigned char), 0x1, fd)) == -1)
    return (-1);

  /* Ram size */
  if ((sizeRead = fread(gbHeader->ram, sizeof(unsigned char), 0x1, fd)) == -1)
    return (-1);

  /* Destination */
  if ((sizeRead = fread(gbHeader->dest, sizeof(unsigned char), 0x1, fd)) == -1)
    return (-1);

  /* USELESS FOR US */
  /* SEE LATER */
  if ((sizeRead = fread(buffer, sizeof(unsigned char), 0x3, fd)) == -1)
    return (-1);

  /* Checksum */
  if ((sizeRead = fread(gbHeader->checksum, sizeof(unsigned char), 0x2, fd)) == -1)
    return (-1);
  return (0);  
}
