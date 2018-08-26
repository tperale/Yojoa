#include "Symbols.h"
#include <stdlib.h>
#include <stdio.h>

SymbolItem* SymbolList_get(SymbolList* self, const unsigned int index) {
  return &self->content[index];
}

size_t SymbolList_add(SymbolList* self, ASTIdentifier* id, ASTNode* ref) {
  if (self->size >= self->_max_size) {
    self->_max_size *= 2;
    self->content = realloc(self->content, sizeof(SymbolItem) * self->_max_size);
  }

  self->content[self->size] = (SymbolItem) {
    .reference = ref,
    .identifier = id,
  };

  return self->size += 1;
}

void SymbolList_free(SymbolList* self) {
  free(self->content);
  free(self);
}

int SymbolList_exist(SymbolList* self, ASTIdentifier* identifier) {
  for (unsigned int i = 0; i < self->size; ++i) {
    if (ASTIdentifier_equal(self->content[i].identifier, identifier)) { return 1; }
  }
  return 0;
}

int SymbolList_new(SymbolList* self, ASTIdentifier* id, ASTNode* ref) {
  if (SymbolList_exist(self, id)) {
    return 0;
  }
  return SymbolList_add(self, id, ref);
}

SymbolList* SymbolList_create(SymbolList* parent) {
  SymbolList* result = (SymbolList*) malloc(sizeof(SymbolList));
  result->_max_size = 32;
  result->size = 0;
  result->content = malloc(sizeof(SymbolItem) * result->_max_size);
  result->parent = parent;
  return result;
}

static SymbolList* SymbolList_block_create(SymbolList* parent, ASTBlock* block) {
  SymbolList* list = SymbolList_create(parent);

  ASTDeclarationVariable** variables = (ASTDeclarationVariable**) block->variables->content;
  for (unsigned int i = 0; i < block->variables->size; ++i) {
    ASTNode* node = (ASTNode*) variables[i];
    if (!SymbolList_new(list, variables[i]->name, node)) {
      fprintf(stderr,"[%d] Error: Variable '%s' is already defined\n", node->info.source_line, variables[i]->name->value);
    }
  }

  ASTNode** statements = (ASTNode**) block->statements->content;
  statements = (ASTNode**) statements;
  for (unsigned int i = 0; i < block->statements->size; ++i) {
    //statements[i]->check(statements[i]);
  }

  return list;
}

static SymbolList* SymbolList_function_create(SymbolList* parent, ASTDeclarationFunction* function) {
  SymbolList* list = SymbolList_create(parent);

  ASTDeclarationVariable** variables = (ASTDeclarationVariable**) function->parameters->content;
  for (unsigned int i = 0; i < function->parameters->size; ++i) {
    ASTNode* node = (ASTNode*) variables[i];
    if (!SymbolList_new(list, variables[i]->name, node)) {
      fprintf(stderr,"[%d] Error: Variable '%s' is already defined\n", node->info.source_line, variables[i]->name->value);
    }
  }

  SymbolList_block_create(list, function->block);
  // TODO add to children
  return list;
}

SymbolList* SymbolList_init(ASTProgram* program) {
  SymbolList* list = SymbolList_create(NULL);
  ASTNode** decls = (ASTNode**) program->decl->content;
  for (unsigned int i = 0; i < program->decl->size; ++i) {
    switch (decls[i]->info.type) {
      case ASTFUNCTION: {
        ;
        ASTDeclarationFunction* func = (ASTDeclarationFunction*) decls[i];
        SymbolList_new(list, func->name->name, (ASTNode*) func);
        SymbolList_function_create(list, func);
        // TODO add to childrens
        break;
      }
      case ASTVARIABLE_DECLARATION: {
        ;
        ASTDeclarationVariable* var = (ASTDeclarationVariable*) decls[i];
        ASTNode* node = (ASTNode*) var;
        if (!SymbolList_new(list, var->name, node)) {
          fprintf(stderr,"[%d] Error: Variable '%s' is already defined\n", node->info.source_line, var->name->value);
        }
        break;
      }
      default:
        break;
    }
  }
  return list;
}
