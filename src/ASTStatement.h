#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "ASTDeclaration.h"
#include "ASTExpression.h"
#include "ArrayList.h"

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

struct ASTDeclarationVariable;

/**
 * @brief Base data structure used for statements.
 */
typedef struct {
  ASTNode node;
} ASTStatement;

typedef struct ASTBlock {
  ArrayList* variables; // containing ASTDeclarationVariable
  ArrayList* statements; // containing ASTStatement
} ASTBlock;

ASTBlock* ASTBlock_create(ArrayList* var_decl, ArrayList* statements);

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
