#include "ASTProgram.h"
#include <stdlib.h>
#include <stdio.h>

char* ASTProgram_code_gen(void* _self) {
  ASTProgram* self = (ASTProgram*) _self;

  char* program;
  asprintf(&program, " ");
  for (unsigned int i = 0; i < self->decl->size; ++i) {
    char* buffer;
    ASTNode* node = (ASTNode*) self->decl->get(self->decl, i);
    asprintf(&buffer, "%s %s", program, node->code_gen(node));
    free(program);
    asprintf(&program, "%s", buffer);
    free(buffer);
  }

  asprintf(&((ASTNode*) self)->code, "(module %s)", program);

  return ((ASTNode*) self)->code;
}

ASTProgram* ASTProgram_create(ArrayList* decl) {
  ASTProgram* result = (ASTProgram*) malloc(sizeof(ASTProgram));
  result->decl = decl;
  result->node.code_gen = ASTProgram_code_gen;
  return result;
}
