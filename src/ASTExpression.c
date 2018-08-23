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
  if (self->expression.node.code) { free(self->expression.node.code); }
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
char* ASTChar_code_gen(void* _self) {
  ASTChar* self = (ASTChar*) _self;

  return ((ASTNode*) self)->code;
}

void ASTChar_free(void* _self) {
  ASTChar* self = (ASTChar*) _self;
  if (self->expression.node.code) { free(self->expression.node.code); }
  free(self);
}

ASTChar* ASTChar_create(char value) {
  ASTChar* result = malloc(sizeof(ASTChar));
  result->value = value;
  result->expression.node.free = ASTChar_free;
  result->expression.node.code_gen = ASTChar_code_gen;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   char* value;
// } ASTString;
char* ASTString_code_gen(void* _self) {
  ASTString* self = (ASTString*) _self;

  asprintf(&((ASTNode*) self)->code, "%s", self->value);

  return ((ASTNode*) self)->code;
}

void ASTString_free(void* _self) {
  ASTString* self = (ASTString*) _self;
  free(self->value);
  if (self->expression.node.code) { free(self->expression.node.code); }
  free(self);
}

ASTString* ASTString_create(char* value) {
  ASTString* result = malloc(sizeof(ASTString));
  result->value = value;
  result->expression.node.free = ASTString_free;
  result->expression.node.code_gen = ASTString_code_gen;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   ASTExpression* lvalue;
//   ASTExpression* rvalue;
//   Operator_t operator_token;
// } ASTOperator;
char* ASTOperator_code_gen(void* _self) {
  ASTOperator* self = (ASTOperator*) _self;

  char* operator;
  switch (self->operator_token) {
    case bPLUS:
      asprintf(&operator, "i32.add");
      break;
    case bMINUS:
      asprintf(&operator, "i32.sub");
      break;
    case bTIMES:
      asprintf(&operator, "i32.mul");
      break;
    case bDIVIDE:
      asprintf(&operator, "i32.div_s");
      break;
    case bEQUAL:
      asprintf(&operator, "i32.eq");
      break;
    case bNEQUAL:
      asprintf(&operator, "i32.neq");
      break;
    case bGREATER:
      asprintf(&operator, "i32.gt_s");
      break;
    case bLESS:
      asprintf(&operator, "i32.lt_s");
      break;
    case uMINUS:
      asprintf(&((ASTNode*) self)->code, "(i32.neg %s)", ((ASTNode*) self->lvalue)->code_gen(self->lvalue));
      return ((ASTNode*) self)->code;
    case uNOT:
      asprintf(&((ASTNode*) self)->code, "(i32.eq %s (i32.const 0))", ((ASTNode*) self->lvalue)->code_gen(self->lvalue));
      return ((ASTNode*) self)->code;
  }

  asprintf(&((ASTNode*) self)->code, "(%s %s %s)", operator, ((ASTNode*) self->lvalue)->code_gen(self->lvalue), ((ASTNode*) self->rvalue)->code_gen(self->rvalue));

  return ((ASTNode*) self)->code;
}

void ASTOperator_free(void* _self) {
  ASTOperator* self = (ASTOperator*) _self;
  ((ASTNode*) self->lvalue)->free(self->lvalue);
  if (self->rvalue) ((ASTNode*) self->rvalue)->free(self->rvalue);
  if (self->expression.node.code) { free(self->expression.node.code); }
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

char* ASTIdentifier_code_gen(void* _self) {
  ASTIdentifier* self = (ASTIdentifier*) _self;

  // TODO link with the symtable
  asprintf(&((ASTNode*) self)->code, "(get_local $%s)", self->value);

  return ((ASTNode*) self)->code;
}

void ASTIdentifier_free(void* _self) {
  ASTIdentifier* self = (ASTIdentifier*) _self;
  if (self->expression.node.code) { free(self->expression.node.code); }
  free(self);
}

ASTIdentifier* ASTIdentifier_create(char* value) {
  ASTIdentifier* result = malloc(sizeof(ASTIdentifier));
  strcpy(result->value, value);
  result->expression.node.free = ASTIdentifier_free;
  result->expression.node.code_gen = ASTIdentifier_code_gen;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   ASTIdentifier* name;
//   ArrayList* arguments;
// } ASTFunctionCall;
char* ASTFunctionCall_code_gen(void* _self) {
  ASTFunctionCall* self = (ASTFunctionCall*) _self;

  char* arguments;
  asprintf(&arguments, " ");
  for (unsigned int i = 0; i < self->arguments->size; ++i) {
    char* buffer;
    ASTNode* node = self->arguments->get(self->arguments, i);
    asprintf(&buffer, "%s %s", arguments, node->code_gen(node));
    free(arguments);
    asprintf(&arguments, "%s", buffer);
    free(buffer);
  }

  asprintf(&((ASTNode*) self)->code, "(call $%s %s)", self->name->value, arguments);
  free(arguments);

  return ((ASTNode*) self)->code;
}

void ASTFunctionCall_free(void* _self) {
  ASTFunctionCall* self = (ASTFunctionCall*) _self;
  ASTIdentifier_free(self->name);
  self->arguments->free(self->arguments);
  if (self->expression.node.code) { free(self->expression.node.code); }
  free(self);
}

ASTFunctionCall* ASTFunctionCall_create(ASTIdentifier* name, ArrayList* arguments) {
  ASTFunctionCall* result = malloc(sizeof(ASTFunctionCall));
  result->name = name;
  result->arguments = arguments;
  result->expression.node.free = ASTFunctionCall_free;
  result->expression.node.code_gen = ASTFunctionCall_code_gen;
  return result;
}
