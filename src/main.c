/*
 * main.c file
 */

#include <stdio.h>

extern int yyparse();

int main(void) {
  yyparse();
  return 0;
}
