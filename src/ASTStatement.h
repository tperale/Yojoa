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
  ASTStatement statement;
  ArrayList* variables; // containing ASTDeclarationVariable
  ArrayList* statements; // containing ASTStatement
} ASTBlock;

void ASTBlock_free(void*);
ASTBlock* ASTBlock_create(ArrayList* var_decl, ArrayList* statements);

typedef struct {
  ASTStatement statement;
  ASTIdentifier* lvalue;
  ASTExpression* rvalue;
} ASTStatementAssignment;

void ASTStatementAssignment_free(void* self);
ASTStatementAssignment* ASTStatementAssignment_create(ASTIdentifier* lvalue, ASTExpression* rvalue);

typedef struct {
  ASTStatement statement;
  ASTExpression* condition;
  ASTStatement* if_block; // TODO Change to block ?
  ASTStatement* else_block;
} ASTStatementCondition;

void ASTStatementCondition_free(void* _self);
ASTStatementCondition* ASTStatementCondition_create(ASTExpression* cond, ASTStatement* if_block, ASTStatement* else_block);

typedef struct {
  ASTStatement statement;
  ASTExpression* condition;
  ASTStatement* content;
} ASTStatementLoop;

void ASTStatementLoop_free(void* _self);
ASTStatementLoop* ASTStatementLoop_create(ASTExpression* cond, ASTStatement* content);

typedef struct {
  ASTStatement statement;
  ASTExpression* value;
} ASTStatementReturn;

void ASTStatementReturn_free(void* _self);
ASTStatementReturn* ASTStatementReturn_create(ASTExpression* value);

#endif
