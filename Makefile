CC = gcc
CFLAGS = -g -Wall -Werror -Wextra -pedantic
THREAD=-pthread
RM=rm -rf

all: ex1 ex2

ex1: circle.c ex1.c
	@$(CC) $(CFLAGS) -o $@ $^ $(THREAD)

ex2: circle.c ex2.c
	@$(CC) $(CFLAGS) -o $@ $^ $(THREAD)

clean:
	@$(RM) ex1 ex2
