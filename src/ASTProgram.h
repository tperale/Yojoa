#ifndef __ASTPROGRAM_H__
#define __ASTPROGRAM_H__

#include "AST.h"
#include "ArrayList.h"

typedef struct {
  ArrayList* decl;
} ASTProgram;

ASTProgram* ASTProgram_create(ArrayList*);

#endif
