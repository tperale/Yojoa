#include "Symbols.h"
#include <stdlib.h>

SymbolItem* SymbolList_get(SymbolList* self, const unsigned int index) {
  return &self->content[index];
}

size_t SymbolList_add(SymbolList* self, ASTType type, ASTDeclarationVariable* id, ASTNode* ref) {
  if (self->size >= self->_max_size) {
    self->_max_size *= 2;
    self->content = realloc(self->content, sizeof(SymbolItem) * self->_max_size);
  }

  self->content[self->size] = (SymbolItem) {
    .reference = ref,
    .identifier = id,
    .type = type,
  };

  return self->size += 1;
}

void SymbolList_free(SymbolList* self) {
  free(self->content);
  free(self);
}

int SymbolList_exist(SymbolList* self, ASTIdentifier* identifier) {
  for (unsigned int i = 0; i < self->size; ++i) {
    if (ASTIdentifier_equal(self->content[i].identifier->name, identifier)) { return 1; }
  }
  return 0;
}

int SymbolList_new(SymbolList* self, ASTType type, ASTDeclarationVariable* id, ASTNode* ref) {
  if (SymbolList_exist(self, id->name)) {
    return 0;
  }
  return SymbolList_add(self, type, id, ref);
}

SymbolList* SymbolList_create(void) {
  SymbolList* result = (SymbolList*) malloc(sizeof(SymbolList));
  result->_max_size = 32;
  result->size = 0;
  result->content = malloc(sizeof(SymbolItem) * result->_max_size);
  return result;
}
