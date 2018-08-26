/*
 * @file List.h
 * @brief
 */
#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__

#include <stddef.h>
#include "AST.h"

typedef struct ArrayList {
  ASTNode** content;
  size_t size;
  size_t _item_size;
  size_t _max_size;
  ASTNode* (*get)(struct ArrayList*, const unsigned int);
  void (*remove)(struct ArrayList*, const unsigned int);
  void (*free)(struct ArrayList*);
  size_t (*add)(struct ArrayList*, ASTNode*);
  size_t (*add_list)(struct ArrayList*, struct ArrayList*);
} ArrayList;

ArrayList* ArrayList_create(size_t content_size);

#endif
