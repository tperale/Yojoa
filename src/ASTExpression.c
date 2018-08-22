/*
 * Expression.c
 * Implementation of functioASTIdentifierns used to build the syntax tree.
 */

#include "ASTExpression.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// typedef struct {
//   ASTExpression expression;
//   int value;
// } ASTInteger;
char* ASTInteger_code_gen(void* _self) {
  ASTInteger* self = (ASTInteger*) _self;

  asprintf(&((ASTNode*) self)->code, "(i32.const %i)", self->value);

  return ((ASTNode*) self)->code;
}

void ASTInteger_free(void* _self) {
  ASTInteger* self = (ASTInteger*) _self;
  free(self);
}

ASTInteger* ASTInteger_create(int value) {
  ASTInteger* result = malloc(sizeof(ASTInteger));
  result->value = value;
  result->expression.node.free = ASTInteger_free;
  result->expression.node.code_gen = ASTInteger_code_gen;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   char value;
// } ASTChar;
void ASTChar_free(void* _self) {
  ASTChar* self = (ASTChar*) _self;
  free(self);
}

ASTChar* ASTChar_create(char value) {
  ASTChar* result = malloc(sizeof(ASTChar));
  result->value = value;
  result->expression.node.free = ASTChar_free;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   char* value;
// } ASTString;
void ASTString_free(void* _self) {
  ASTString* self = (ASTString*) _self;
  free(self->value);
  free(self);
}

ASTString* ASTString_create(char* value) {
  ASTString* result = malloc(sizeof(ASTString));
  result->value = value;
  result->expression.node.free = ASTString_free;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   ASTExpression* lvalue;
//   ASTExpression* rvalue;
// } ASTOperator;
char* ASTOperator_code_gen(void* _self) {
  ASTOperator* self = (ASTOperator*) _self;

  asprintf(&((ASTNode*) self)->code, "(i32.add %s %s)", ((ASTNode*) self->lvalue)->code_gen(self->lvalue), ((ASTNode*) self->rvalue)->code_gen(self->rvalue));

  return ((ASTNode*) self)->code;
}

void ASTOperator_free(void* _self) {
  ASTOperator* self = (ASTOperator*) _self;
  ((ASTNode*) self->lvalue)->free(self->lvalue);
  if (self->rvalue) ((ASTNode*) self->rvalue)->free(self->rvalue);
  free(self->expression.node.code);
  free(self);
}

ASTOperator* ASTOperator_create(ASTExpression* lvalue, ASTExpression* rvalue, int operator_token) {
  ASTOperator* result = malloc(sizeof(ASTOperator));
  result->lvalue = lvalue;
  result->rvalue = rvalue;
  result->operator_token = operator_token;
  result->expression.node.free = ASTOperator_free;
  result->expression.node.code_gen = ASTOperator_code_gen;
  return result;
}

// typedef struct {
//   char value[64];
// } ASTIdentifier;
int ASTIdentifier_equal(ASTIdentifier* x, ASTIdentifier* y) {
  return strcmp(x->value, y->value) == 0;
}

void ASTIdentifier_free(void* _self) {
  ASTIdentifier* self = (ASTIdentifier*) _self;
  free(self);
}

ASTIdentifier* ASTIdentifier_create(char* value) {
  ASTIdentifier* result = malloc(sizeof(ASTIdentifier));
  strcpy(result->value, value);
  result->expression.node.free = ASTIdentifier_free;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   ASTIdentifier* name;
//   ArrayList* arguments;
// } ASTFunctionCall;
void ASTFunctionCall_free(void* _self) {
  ASTFunctionCall* self = (ASTFunctionCall*) _self;
  ASTIdentifier_free(self->name);
  self->arguments->free(self->arguments);
  free(self);
}

ASTFunctionCall* ASTFunctionCall_create(ASTIdentifier* name, ArrayList* arguments) {
  ASTFunctionCall* result = malloc(sizeof(ASTFunctionCall));
  result->name = name;
  result->arguments = arguments;
  result->expression.node.free = ASTFunctionCall_free;
  return result;
}
