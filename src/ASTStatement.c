#include "ASTStatement.h"
#include <stdlib.h>

// typedef struct ASTBlock {
//   ArrayList* variables; // containing ASTDeclarationVariable
//   ArrayList* statements; // containing ASTStatement
// } ASTBlock;
ASTBlock* ASTBlock_create(ArrayList* var_decl, ArrayList* statements) {
  ASTBlock* result = (ASTBlock*) malloc(sizeof(ASTBlock));
  result->variables = var_decl;
  result->statements = statements;

  return result;
}
