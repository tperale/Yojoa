#ifndef __SYMBOLSLIST_H__
#define __SYMBOLSLIST_H__

#include <stddef.h>
#include "ASTProgram.h"
#include "ASTStatement.h"
#include "ASTDeclaration.h"
#include "ASTExpression.h"
#include "AST.h"

struct ASTBlock;
struct ASTDeclarationFunction;
struct ASTIdentifier;

typedef struct {
  struct ASTIdentifier* identifier;
  struct _ASTNode* reference;
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
size_t SymbolList_add(SymbolList*, struct ASTIdentifier*, ASTNode*);
void SymbolList_free(SymbolList*);
int SymbolList_exist(SymbolList*, struct ASTIdentifier*);
int SymbolList_new(SymbolList*, struct ASTIdentifier*, ASTNode*);
SymbolList* SymbolList_create(SymbolList*);
SymbolList* SymbolList_block_create(SymbolList*, struct ASTBlock*);
SymbolList* SymbolList_function_create(SymbolList*, struct ASTDeclarationFunction*);
SymbolList* SymbolList_init(ASTProgram*);

#endif
