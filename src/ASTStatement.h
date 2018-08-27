#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include "ASTDeclaration.h"
#include "ASTExpression.h"
#include "Symbols.h"
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
struct ASTIdentifier;

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

void ASTBlock_free(ASTNode*);
ASTBlock* ASTBlock_create(ArrayList* var_decl, ArrayList* statements, ASTInfo);

typedef struct {
  ASTStatement statement;
  struct ASTIdentifier* lvalue;
  struct ASTExpression* rvalue;
} ASTStatementAssignment;

void ASTStatementAssignment_free(ASTNode* self);
ASTStatementAssignment* ASTStatementAssignment_create(struct ASTIdentifier* lvalue, struct ASTExpression* rvalue, ASTInfo);

typedef struct {
  ASTStatement statement;
  struct ASTExpression* condition;
  ASTStatement* if_block; // TODO Change to block ?
  ASTStatement* else_block;
} ASTStatementCondition;

void ASTStatementCondition_free(ASTNode* _self);
ASTStatementCondition* ASTStatementCondition_create(struct ASTExpression* cond, ASTStatement* if_block, ASTStatement* else_block, ASTInfo);

typedef struct {
  ASTStatement statement;
  struct ASTExpression* condition;
  ASTStatement* content;
} ASTStatementLoop;

void ASTStatementLoop_free(ASTNode* _self);
ASTStatementLoop* ASTStatementLoop_create(struct ASTExpression* cond, ASTStatement* content, ASTInfo);

typedef struct {
  ASTStatement statement;
  struct ASTExpression* value;
} ASTStatementReturn;

void ASTStatementReturn_free(ASTNode* _self);
ASTStatementReturn* ASTStatementReturn_create(struct ASTExpression* value, ASTInfo);

typedef struct {
  ASTStatement statement;
  struct ASTExpression* value;
} ASTStatementWrite;

void ASTStatementWrite_free(ASTNode* _self);
ASTStatementWrite* ASTStatementWrite_create(struct ASTExpression*, ASTInfo);

typedef struct {
  ASTStatement statement;
  struct ASTIdentifier* ref;
} ASTStatementRead;

void ASTStatementRead_free(ASTNode* _self);
ASTStatementRead* ASTStatementRead_create(struct ASTIdentifier*, ASTInfo);

#endif
