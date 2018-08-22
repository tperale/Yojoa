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
  yyparse();
  printf("%s", ASTProgram_code_gen(program_struct));
  return 0;
}
