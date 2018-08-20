/*
 * @file List.h
 * @brief
 */
#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__

#include <stddef.h>

typedef struct ArrayList {
  void** content;
  size_t size;
  size_t _item_size;
  size_t _max_size;
  void* (*get)(struct ArrayList*, const unsigned int);
  void (*remove)(struct ArrayList*, const unsigned int);
  void (*free)(struct ArrayList*);
  size_t (*add)(struct ArrayList*, void*);
  size_t (*add_list)(struct ArrayList*, struct ArrayList*);
} ArrayList;

ArrayList* ArrayList_create(size_t content_size);

#endif
