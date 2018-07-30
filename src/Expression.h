/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "Parser.h"


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
//                |            Statement                  |         |       Expression              |               |       Declaration          |
//                |                                       |         |                               |               |                            |
//                +------------------+--------------------+         +-----------------------------+-+               +-+--------------------------+
//                                   ^                                                            ^                   ^           +-------------------------------------+
// +------------------------+        |                                                            |                   |           | FunctionDeclaration                 |
// | Block                  |        |                         +------------------------+         |                   |           | + type: Identifier                  |
// | + var: VariableDecl[]  |        |                         | Identifier             |         |                   |           | | name: Identifier                  |
// | + content: Statement[] |        |                         | + value: string        +---------+                   +-----------+ | parameters: VariableDeclaration[] |
// |                        +--------+                         |                        |         |                   |           | + block: Block                      |
// |                        |        |                         |                        |         |                   |           |                                     |
// +------------------------+        |                         +------------------------+         |                   |           |                                     |
//                                   |                         +------------------------+         |                   |           +-------------------------------------+
// +------------------------+        |                         | Operator               +---------+                   |           +----------------------------+
// | Assignment             |        |                         | + type: int            |         |                   |           | VariableDeclaration        |
// | + l^alue: Identifier   |        |                         | | l^alue: Expression   |         |                   |           | + type: Identifier         |
// | + rvalue: Expression   +--------+                         | + rvalue: Expression   |         |                   |           | + name: Identifier         |
// |                        |        |                         |                        |         |                   +-----------+                            |
// +------------------------+        |                         +------------------------+         |                               |                            |
//      +-------------------+        |                         +------------------------+         |                               +----------------------------+
//      | Loop              |        |                         |  Integer               +---------+
//      | + cond: Expression|        |                         |  + ^alue: int          |         |
//      | + content: Block  +--------+                         +--+---------------------+         |
//      |                   |        |                              +-------------------+         |
//      |                   |        |                              | Char              |         |
//      +-------------------+        |                              | + value: char     +---------+
//      +-------------------+        |                              |                   |
//      | Condition         |        |                              +-------------------+
//      | + cond: Expression+--------+
//      | | if: Block       |        |
//      | | else: Block     |        |
//      +-------------------+        |
// +------------------------+        |          +-------------------+
// | FunctionCall           |        |          | Return            |
// | + name: Identifier     +--------+----------+ + value: Expression
// | + arguments: Expression[]                  |                   |
// |                        |                   |                   |
// +------------------------+                   +-------------------+


/**
 * @brief The expression structure
 */
typedef struct _ASTNode
{
    enum yytokentype type;///< type of operation

    int value;

    struct _ASTNode *left; ///< left side of the tree
    struct _ASTNode *right;///< right side of the tree
} ASTNode;


/**
 * @brief It creates an identifier
 * @param value The number value
 * @return The expression or NULL in case of no memory
 */
ASTNode *createNumber(int value);

/**
 * @brief It creates an operation
 * @param type The operation type
 * @param left The left operand
 * @param right The right operand
 * @return The expression or NULL in case of no memory
 */
ASTNode *createOperation(EOperationType type, ASTNode* left, ASTNode* right);

/**
 * @brief Deletes a expression
 * @param b The expression
 */
void deleteExpression(ASTNode* b);

#endif // __EXPRESSION_H__
