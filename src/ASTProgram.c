#include "ASTProgram.h"
#include <stdlib.h>

ASTProgram* ASTProgram_create(ArrayList* var_decl, ArrayList* fun_decl) {
  ASTProgram* result = (ASTProgram*) malloc(sizeof(ASTProgram));
  result->var_decl = var_decl;
  result->fun_decl = fun_decl;
  return result;
}
