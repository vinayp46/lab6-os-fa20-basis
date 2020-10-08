CC = gcc
CFLAGS = -g -O0 -Wall -Werror -Wextra -pedantic -Wno-unused-parameter
THREAD=-pthread
RM=rm -rf

all: ex1 ex2

ex1: circle.c ex1.c
	@$(CC) $(CFLAGS) -o $@ $^ $(THREAD)

ex2: circle.c ex2.c
	@$(CC) $(CFLAGS) -o $@ $^ $(THREAD)

clean:
	@$(RM) ex1 ex2
