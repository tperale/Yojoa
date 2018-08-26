#ifndef __SYMBOLSLIST_H__
#define __SYMBOLSLIST_H__

#include <stddef.h>
#include "ASTProgram.h"
#include "ASTExpression.h"
#include "ASTDeclaration.h"
#include "AST.h"

#define FUNC 0
#define VAR 1

typedef struct {
  ASTIdentifier* identifier;
  ASTNode* reference;
} SymbolItem;

typedef struct _SymbolList {
  struct _SymbolList* parent;
  struct _SymbolList** _childrens;
  size_t _childrens_size;
  size_t _childrens_max_size;
  SymbolItem* content;
  size_t size;
  size_t _max_size;
} SymbolList;

SymbolItem* SymbolList_get(SymbolList*, const unsigned int);
size_t SymbolList_add(SymbolList*, ASTIdentifier*, ASTNode*);
void SymbolList_free(SymbolList*);
int SymbolList_exist(SymbolList*, ASTIdentifier*);
int SymbolList_new(SymbolList*, ASTIdentifier*, ASTNode*);
SymbolList* SymbolList_create(SymbolList*);
SymbolList* SymbolList_init(ASTProgram* program);

#endif
