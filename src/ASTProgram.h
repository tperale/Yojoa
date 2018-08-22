#ifndef __ASTPROGRAM_H__
#define __ASTPROGRAM_H__

#include "AST.h"
#include "ArrayList.h"

typedef struct {
  ASTNode node;
  ArrayList* decl;
} ASTProgram;

char* ASTProgram_code_gen(void* _self);
ASTProgram* ASTProgram_create(ArrayList*);

#endif
