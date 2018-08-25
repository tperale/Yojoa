#ifndef __SYMBOLSLIST_H__
#define __SYMBOLSLIST_H__

#include <stddef.h>
#include "ASTExpression.h"
#include "ASTDeclaration.h"
#include "AST.h"

#define FUNC 0
#define VAR 1

typedef struct {
  ASTType type;
  ASTDeclarationVariable* identifier;
  ASTNode* reference;
} SymbolItem;

typedef struct {
  SymbolItem* content;
  size_t size;
  size_t _max_size;
} SymbolList;

SymbolItem* SymbolList_get(SymbolList*, const unsigned int);
size_t SymbolList_add(SymbolList*, ASTType, ASTDeclarationVariable*, ASTNode*);
void SymbolList_free(SymbolList*);
int SymbolList_exist(SymbolList*, ASTIdentifier*);
int SymbolList_new(SymbolList*, ASTType, ASTDeclarationVariable*, ASTNode*);
int SymbolList_reserve(SymbolList*, ASTType, ASTDeclarationVariable*);
int SymbolList_set(SymbolList*, ASTDeclarationVariable*, ASTNode*);
SymbolList* SymbolList_create(void);

#endif
