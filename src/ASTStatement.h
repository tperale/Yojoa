#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "ASTDeclaration.h"
#include "ASTExpression.h"

extern ASTDeclarationVariable;

//                +------------------+--------------------+
//                |                                       |
//                |            Statement                  |
//                |                                       |
//                +------------------+--------------------+
//                                   ^
// +------------------------+        |
// | Block                  |        |
// | + var: VariableDecl[]  |        |
// | + content: Statement[] |        |
// |                        +--------+
// +------------------------+        |
//                                   |
// +------------------------+        |
// | Assignment             |        |
// | + lvalue: Identifier   |        |
// | + rvalue: Expression   +--------+
// +------------------------+        |
//      +-------------------+        |
//      | Loop              |        |
//      | + cond: Expression|        |
//      | + content: Block  +--------+
//      +-------------------+        |
//                                   |
//                                   |
//      +-------------------+        |
//      | Condition         |        |
//      | + cond: Expression+--------+
//      | | if: Block       |        |
//      | + else: Block     |        |
//      +-------------------+        |
//      +-------------------+        |
//      | Return            +--------+
//      | + value: Expression
//      +-------------------+

/**
 * @brief Base data structure used for statements.
 */
typedef struct {
  ASTNode node;
} ASTStatement;

typedef struct {
  ASTStatement statement;
  ASTDeclarationVariable** variables;
  ASTStatement** statements;
} ASTBlock;

typedef struct {
  ASTStatement statement;
  ASTIdentifier* lvalue;
  ASTExpression* rvalue;
} ASTStatementAssignment;

typedef struct {
  ASTStatement statement;
  ASTExpression* condition;
  ASTBlock* if_block;
  ASTBlock* else_block;
} ASTStatementCondition;

typedef struct {
  ASTStatement statement;
  ASTExpression* condition;
  ASTBlock* content;
} ASTStatementLoop;

typedef struct {
  ASTStatement statement;
  ASTExpression* value;
} ASTStatementReturn;

#endif
