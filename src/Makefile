all: tiny
SRC=./
CC=gcc
CFLAGS=-g -Wall -Werror -Wextra -Wshadow -std=gnu11
LDFLAGS=-lm

FILES=$(wildcard ${SRC}*.c) ${SRC}Lexer.c ${SRC}Parser.c
OBJS=$(subst .c,.o,$(FILES))

Parser.c:	Parser.y
		bison -v -d -o $@ $^

Parser.h: Parser.c

Lexer.c:	Lexer.l Parser.h
		flex -o $@ $^

%.o: %.c
		$(CC) -c $(CFLAGS) -o $@ $<

tiny:	$(OBJS)
		$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
		rm -f *.o *~ Lexer.c Lexer.h Parser.c Parser.h

print-%  : ; @echo $* = $($*)
