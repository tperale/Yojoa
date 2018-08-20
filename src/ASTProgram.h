#ifndef __ASTPROGRAM_H__
#define __ASTPROGRAM_H__

#include "AST.h"
#include "ArrayList.h"

typedef struct {
  ArrayList* var_decl;
  ArrayList* fun_decl;
} ASTProgram;

ASTProgram* ASTProgram_create(ArrayList*, ArrayList*);

#endif
