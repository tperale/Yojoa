/*
 * @file ASTExpression.h
 * @brief
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "AST.h"
#include "Symbols.h"
#include "ArrayList.h"

//       +-------------------------------+
//       |                               |
//       |         Expression            |
//       |                               |
//       +-----------------------------+-+
//                                     ^
//                                     |
//  +------------------------+         |
//  | Identifier             |         |
//  | + value: string        +---------+
//  |                        |         |
//  +------------------------+         |
//  +------------------------+         |
//  | Operator               +---------+
//  | + type: int            |         |
//  | | lvalue: Expression   |         |
//  | + rvalue: Expression   |         |
//  +------------------------+         |
//  +------------------------+         |
//  |  Integer               +---------+
//  |  + value: int          |         |
//  +--+---------------------+         |
//       +-------------------+         |
//       | Char              |         |
//       | + value: char     +---------+
//       +-------------------+         |
//  +------------------------+         |
//  | FunctionCall           |         |
//  | + name: Identifier     +---------+
//  | + arguments: Expression[]
//  +------------------------+

/**
 * @brief Base data structure used for expressions.
 */
typedef struct ASTExpression {
  ASTNode node;
} ASTExpression;

typedef struct {
  ASTExpression expression;
  int value;
} ASTInteger;
void ASTInteger_free(void* _self);
ASTInteger* ASTInteger_create(int value, ASTInfo);

typedef struct {
  ASTExpression expression;
  char value;
} ASTChar;
void ASTChar_free(void* _self);
ASTChar* ASTChar_create(char value, ASTInfo);

typedef struct {
  ASTExpression expression;
  char* value;
} ASTString;
void ASTString_free(void* _self);
ASTString* ASTString_create(char* value, ASTInfo);

typedef struct {
  ASTExpression expression;
  ASTExpression* lvalue;
  ASTExpression* rvalue;
  Operator_t operator_token;
} ASTOperator;
void ASTOperator_free(void* _self);
ASTOperator* ASTOperator_create(ASTExpression* lvalue, ASTExpression* rvalue, int operator_token, ASTInfo);

typedef struct ASTIdentifier {
  ASTExpression expression;
  char* value;
} ASTIdentifier;
int ASTIdentifier_equal(ASTIdentifier* x, ASTIdentifier* y);
void ASTIdentifier_free(void* self);
ASTIdentifier* ASTIdentifier_create(char* value, ASTInfo);

typedef struct {
  ASTExpression expression;
  ASTIdentifier* name;
  ArrayList* arguments;
} ASTFunctionCall;
void ASTFunctionCall_free(void* _self);
ASTFunctionCall* ASTFunctionCall_create(ASTIdentifier* name, ArrayList* arguments, ASTInfo);

// TODO ASTArrayAccessor for array type (eg: <varname>[<number>] or for array declaration int[4])
#endif
