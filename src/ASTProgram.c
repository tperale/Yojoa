#include "ASTProgram.h"
#include <stdlib.h>
#include <stdio.h>

char* ASTProgram_code_gen(ASTNode* _self) {
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

  asprintf(&(_self->code), "(module (import \"console\" \"read\" (func $read (param i32) (param i32)))(import \"console\" \"write\" (func $write (param i32)))(import \"env\" \"memory\" (memory 256))(import \"env\" \"table\" (table 0 anyfunc))%s)", program);

  return _self->code;
}

void ASTProgram_free(ASTNode* _self) {
  ASTProgram* self = (ASTProgram*) _self;
  if (self->node.code) { free(self->node.code); }
  free(self);
}

ASTProgram* ASTProgram_create(ArrayList* decl) {
  ASTProgram* result = (ASTProgram*) malloc(sizeof(ASTProgram));
  result->decl = decl;
  result->node.free = ASTProgram_free;
  result->node.code_gen = ASTProgram_code_gen;
  return result;
}
