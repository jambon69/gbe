# MAKEFILE FOR GAMEBOY EMULATOR
# BY LOUIS GIESEN

NAME = gbe

SRCS =  sources/main.c		\
	sources/header.c	\
	sources/utils.c		\
	sources/args.c		\
	sources/emulate.c	\
	sources/machine.c	\
	sources/registers.c	\
	sources/dec.c		\
	sources/jump.c		\
	sources/load.c		\
	sources/xor.c		\
	sources/comp.c		\
	sources/debugger.c	\

OBJS = $(SRCS:.c=.o)

CC = gcc

CPPFLAGS = -Wall -Wextra -Iincludes/

LDFLAGS =

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(CPPFLAGS) $(LDFLAGS)

clean:
	$(RM) $(NAME)

fclean: clean
	$(RM) $(OBJS)

re: fclean all

.PHONY: all clean fclean re
