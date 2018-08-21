#include "ASTProgram.h"
#include <stdlib.h>

ASTProgram* ASTProgram_create(ArrayList* decl) {
  ASTProgram* result = (ASTProgram*) malloc(sizeof(ASTProgram));
  result->decl = decl;
  return result;
}
