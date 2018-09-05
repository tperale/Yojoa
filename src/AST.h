#ifndef __AST_H__
#define __AST_H__

#include <stddef.h>

typedef int ASTType;
struct _SymbolList;
struct _ASTNode;

typedef enum {
  bMINUS,
  bPLUS,
  bTIMES,
  bDIVIDE,
  bEQUAL,
  bNEQUAL,
  bGREATER,
  bLESS,
  uMINUS,
  uNOT,
} Operator_t;

typedef enum {
  ASTNONE,
  ASTPROGRAM,
  ASTFUNCTION,
  ASTVARIABLE_DECLARATION,
  ASTVARIABLE_PARAM,
  ASTVARIABLE_GLOBAL,
  ASTPARAM,
  ASTBLOCK,
  ASTASSIGNMENT,
  ASTLOOP,
  ASTCONDITION,
  ASTRETURN,
  ASTIDENTIFIER,
  ASTOPERATOR,
  ASTINTEGER,
  ASTCHAR,
  ASTFUNCTIONCALL,
  ASTWRITE,
  ASTREAD,
  ASTVARIABLE,
  ASTLENGTH,
} ASTType_t;

typedef struct {
  ASTType_t type;
  size_t length;
} ASTTypePrimitive_t;

typedef struct {
  int source_line;
  ASTType_t type;
} ASTInfo;

/**
 * @brief The most basic expression structure.
 */
typedef struct _ASTNode {
  ASTInfo info;
  void (*free)(struct _ASTNode*);
  char* (*code_gen)(struct _ASTNode*);
  void (*check)(struct _SymbolList*, struct _ASTNode*);
  char* code;
  struct _SymbolList* scope;
} ASTNode;

#endif
