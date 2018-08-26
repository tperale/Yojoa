/*
 * main.c file
 */

#include <stdio.h>
#include "ASTProgram.h"
#include "Symbols.h"

extern ASTProgram* program_struct;
extern int yyparse();

int main(void) {
  if (yyparse()) {
    printf("Exiting\n");
    return 1;
  }
  SymbolList_init(program_struct);
  printf("%s\n", ASTProgram_code_gen(program_struct));
  return 0;
}
