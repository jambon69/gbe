/*
** All registers for the gameboy
** Access them via registers.a or regiters.af
** Structure given by https://cturt.github.io/cinoop.html
*/

#ifndef REGISTERS_H
# define REGISTERS_H

struct registers {
  struct {
    union {
      struct {
	unsigned char f;
	unsigned char a;
      };
      unsigned short af;
    };
  };

  struct {
    union {
      struct {
	unsigned char c;
	unsigned char b;
      };
      unsigned short bc;
    };
  };

  struct {
    union {
      struct {
	unsigned char e;
	unsigned char d;
      };
      unsigned short de;
    };
  };

  struct {
    union {
      struct {
	unsigned char l;
	unsigned char h;
      };
      unsigned short hl;
    };
  };

  unsigned short sp;
  unsigned short pc;

  unsigned short z_flag;
  unsigned short c_flag;
};

extern struct registers registers;

#endif
