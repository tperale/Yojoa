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

/**
 * @brief The most basic expression structure.
 */
typedef struct _ASTNode {
  void (*free)(void*);
  char* (*code_gen)(void*);
  char* code;
} ASTNode;

#endif
