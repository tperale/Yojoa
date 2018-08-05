/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "Parser.h"

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
typedef struct {
  ASTNode node;
} ASTExpression;

typedef struct {
  ASTExpression expression;
  int value;
} ASTInteger;

typedef struct {
  ASTExpression expression;
  char value;
} ASTChar;

typedef struct {
  ASTExpression expression;
  char* value;
} ASTString;

typedef struct {
  ASTExpression expression;
  ASTExpression* lvalue;
  ASTExpression* rvalue;
} ASTOperator;

typedef struct {
  char[64] value;
} ASTIdentifier;

typedef struct {
  ASTExpression expression;
  ASTIdentifier name;
  ASTExpression* arguments;
} ASTFunctionCall;

#endif
