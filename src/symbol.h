#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "Parser.h"

typedef struct symbol_table {
  char* name;
  enum yytokentype;
  void* value;
  // int scope;
  // struct symbol_table parent;
} Symbol;

void symbol_insert(char* name, int type);

Symbol symbol_find(char* name);

#endif
