#ifndef __AST_H__
#define __AST_H__

//                                                                                                +-------------+
//                                                                                                |             |
//                                                                                                |    Node     |
//                                                                                                |             |
//                                                                                                +-------+-----+
//                                                                                                        ^
//                                                                                                        |
//                                   +---------------------------------------------+----------------------+-----------------------+
//                                   |                                             |                                              |
//                                   |                                             |                                              |
//                                   |                                             |                                              |
//                                   |                                             |                                              |
//                +------------------+--------------------+         +--------------+----------------+               +-------------+--------------+
//                |                                       |         |                               |               |                            |
//                |            Statement                  |         |         Expression            |               |         Declaration        |
//                |                                       |         |                               |               |                            |
//                +------------------+--------------------+         +-----------------------------+-+               +-+--------------------------+
//                                   ^                                                            ^                   ^           +-------------------------------------+
// +------------------------+        |                                                            |                   |           | FunctionDeclaration                 |
// | Block                  |        |                         +------------------------+         |                   |           | + type: Identifier                  |
// | + var: VariableDecl[]  |        |                         | Identifier             |         |                   |           | | name: Identifier                  |
// | + content: Statement[] |        |                         | + value: string        +---------+                   +-----------+ | parameters: VariableDeclaration[] |
// |                        +--------+                         |                        |         |                   |           | + block: Block                      |
// +------------------------+        |                         +------------------------+         |                   |           +-------------------------------------+
//                                   |                         +------------------------+         |                   |
// +------------------------+        |                         | Operator               +---------+                   |           +----------------------------+
// | Assignment             |        |                         | + type: int            |         |                   |           | VariableDeclaration        |
// | + lvalue: Identifier   |        |                         | | lvalue: Expression   |         |                   |           | + type: Identifier         |
// | + rvalue: Expression   +--------+                         | + rvalue: Expression   |         |                   +-----------+ + name: Identifier         |
// +------------------------+        |                         +------------------------+         |                               +----------------------------+
//      +-------------------+        |                         +------------------------+         |
//      | Loop              |        |                         |  Integer               +---------+
//      | + cond: Expression|        |                         |  + value: int          |         |
//      | + content: Block  +--------+                         +--+---------------------+         |
//      +-------------------+        |                              +-------------------+         |
//                                   |                              | Char              |         |
//                                   |                              | + value: char     +---------+
//      +-------------------+        |                              +-------------------+         |
//      | Condition         |        |                         +------------------------+         |
//      | + cond: Expression+--------+                         | FunctionCall           |         |
//      | | if: Block       |        |                         | + name: Identifier     +---------+
//      | + else: Block     |        |                         | + arguments: Expression[]
//      +-------------------+        |                         +------------------------+
//      +-------------------+        |
//      | Return            +--------+
//      | + value: Expression
//      +-------------------+

typedef int ASTType;
struct _SymbolList;

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
} ASTType_t;

typedef struct {
  int source_line;
  ASTType_t type;
} ASTInfo;

/**
 * @brief The most basic expression structure.
 */
typedef struct _ASTNode {
  ASTInfo info;
  void (*free)(void*);
  char* (*code_gen)(void*);
  void (*check)(struct _SymbolList*, struct _ASTNode*);
  char* code;
  struct _SymbolList* scope;
} ASTNode;

#endif
