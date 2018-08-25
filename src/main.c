/*
 * main.c file
 */

#include <stdio.h>
#include "ASTProgram.h"
#include "Symbols.h"

extern ASTProgram* program_struct;
extern int yyparse();
extern SymbolList* symbols;

int main(void) {
  symbols = SymbolList_create();
  if (yyparse()) {
    printf("Exiting\n");
    return 1;
  }

  printf("%s\n", ASTProgram_code_gen(program_struct));
  return 0;
}
