SRC=./
CC=gcc
CFLAGS=-g -Wall -Werror -Wextra -Wshadow -std=c11

FILES=$(wildcard ${SRC}*.c) ${SRC}Lexer.c ${SRC}Parser.c

all:	$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o tiny

Lexer.c:	Lexer.l
		flex Lexer.l

Parser.c:	Parser.y Lexer.c
		bison Parser.y

clean:
		rm -f *.o *~ Lexer.c Lexer.h Parser.c Parser.h
