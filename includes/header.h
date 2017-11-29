/*
** Gameboy emulator by Louis Giesen
**
** Header file for the header.c
*/

#ifndef HEADER_H
# define HEADER_H

/*
** Structures
*/
typedef struct s_gbHeader
{
  unsigned char beginCode[0x4];
  unsigned char nintendo[0x30];
  unsigned char title[0xf];
  unsigned char color[0x1];
  unsigned char licensee[0x2];
  unsigned char superGB[0x1];
  unsigned char cartridge[0x1];
  unsigned char rom[0x1];
  unsigned char ram[0x1];
  unsigned char dest[0x1];
  unsigned char checksum[0x2];
} t_gbHeader;

/*
** functions
*/
int reservedMemoryLocation(FILE *fd);
int getHeaderInformations(FILE *fd, struct s_gbHeader *);
void dumpHeader(struct s_gbHeader *);

#endif
