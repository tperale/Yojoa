#include "ArrayList.h"
#include "AST.h"
#include <stdlib.h>

#define ARRAY_LIST_BASE_SIZE 32

static void* ArrayList_get(ArrayList* self, const unsigned int index) {
  return (void*) self->content[index];
}

static size_t ArrayList_add(ArrayList* self, void* item) {
  if (self->size >= self->_max_size) {
    self->_max_size *= 2;
    self->content = realloc(self->content, self->_max_size * self->_item_size);
  }

  *(self->content + (self->size * self->_item_size)) = item;
  return self->size += 1;
}

static size_t ArrayList_addlist(ArrayList* self, ArrayList* other) {
  // TODO verification same content size
  for (unsigned int i = 0; i < other->size; ++i) {
    ArrayList_add(self, ArrayList_get(other, i));
  }

  free(other); // non recursize free of the "other" array as we keep the pointer.

  return self->size;
}


// static void ArrayList_remove(ArrayList* self, int index) {
//
// }

static void ArrayList_free(ArrayList* self) {
  for (unsigned int i = 0; i < self->size; ++i) {
    void* tmp = self->content + (i * (self->_item_size));
    ((ASTNode*) tmp)->free(tmp);
  }
  free(self->content);
  free(self);
}

ArrayList* ArrayList_create(size_t content_size) {
  ArrayList* result = (ArrayList*) malloc(sizeof(ArrayList));
  result->_item_size = content_size;
  result->_max_size = ARRAY_LIST_BASE_SIZE;
  result->size = 0;
  result->content = malloc(result->_item_size * ARRAY_LIST_BASE_SIZE);

  result->add = ArrayList_add;
  result->add_list = ArrayList_addlist;
  result->get = ArrayList_get;
  // result->remove = ArrayList_remove;
  result->free = ArrayList_free;

  return result;
}
