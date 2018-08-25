all: tiny
SRC=./
CC=gcc
CFLAGS=-g -ggdb -Wall -Werror -Wextra -Wshadow -std=gnu11 -D _GNU_SOURCE
LDFLAGS=-lm

ifeq (0, 1)
	FLEX_DEBUG=-d
	BISON_DEBUG=-t
else
	FLEX_DEBUG=
	BISON_DEBUG=
endif

FILES=$(wildcard ${SRC}*.c) ${SRC}Lexer.c ${SRC}Parser.c
OBJS=$(subst .c,.o,$(FILES))

Parser.c:	Parser.y
		bison ${BISON_DEBUG} -d -o $@ $^

Parser.h: Parser.c

Lexer.c:	Lexer.l Parser.h
		flex $(FLEX_DEBUG) -o $@ $^

%.o: %.c
		$(CC) -c $(CFLAGS) -o $@ $<

tiny:	$(OBJS)
		$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
		rm -f *.o *~ Lexer.c Lexer.h Parser.c Parser.h

print-%  : ; @echo $* = $($*)
