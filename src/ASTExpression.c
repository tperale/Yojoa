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
void ASTInteger_check(SymbolList* list, ASTNode* _self) {
  (void)(list);
  (void)(_self);
  return;
}

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

ASTInteger* ASTInteger_create(int value, ASTInfo info) {
  ASTInteger* result = malloc(sizeof(ASTInteger));
  result->value = value;
  result->expression.node.free = ASTInteger_free;
  result->expression.node.code_gen = ASTInteger_code_gen;
  result->expression.node.check = ASTInteger_check;
  result->expression.node.info = info;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   char value;
// } ASTChar;
void ASTChar_check(SymbolList* list, ASTNode* _self) {
  (void)(list);
  (void)(_self);
  return;
}

char* ASTChar_code_gen(void* _self) {
  ASTChar* self = (ASTChar*) _self;

  return ((ASTNode*) self)->code;
}

void ASTChar_free(void* _self) {
  ASTChar* self = (ASTChar*) _self;
  if (self->expression.node.code) { free(self->expression.node.code); }
  free(self);
}

ASTChar* ASTChar_create(char value, ASTInfo info) {
  ASTChar* result = malloc(sizeof(ASTChar));
  result->value = value;
  result->expression.node.free = ASTChar_free;
  result->expression.node.code_gen = ASTChar_code_gen;
  result->expression.node.check = ASTChar_check;
  result->expression.node.info = info;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   char* value;
// } ASTString;
void ASTString_check(SymbolList* list, ASTNode* _self) {
  (void)(list);
  (void)(_self);
  return;
}

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

ASTString* ASTString_create(char* value, ASTInfo info) {
  ASTString* result = malloc(sizeof(ASTString));
  result->value = value;
  result->expression.node.free = ASTString_free;
  result->expression.node.code_gen = ASTString_code_gen;
  result->expression.node.check = ASTString_check;
  result->expression.node.info = info;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   ASTExpression* lvalue;
//   ASTExpression* rvalue;
//   Operator_t operator_token;
// } ASTOperator;
static void _ASTOperator_check_return_type(SymbolList* list, ASTNode* x) {
  switch (x->info.type) {
    case ASTOPERATOR:
    case ASTINTEGER:
      break;
    case ASTFUNCTIONCALL: {
      ASTDeclarationFunction* tmp = (ASTDeclarationFunction*) SymbolList_exist(list, ((ASTFunctionCall*) x)->name);
      if (tmp->name->type != ASTINTEGER) {
        print_error(x->info.source_line, "Invalid return type for function '%s', expected an integer", tmp->name->name->value);
      }
      break;
    }
    case ASTVARIABLE: {
      ASTDeclarationVariable* tmp = (ASTDeclarationVariable*) SymbolList_exist(list, (ASTIdentifier*) x);
      if (tmp->type != ASTINTEGER) {
        print_error(x->info.source_line, "Invalid variable type for '%s', expected an integer ", tmp->name->value);
      }
      break;
    }
    default:
      print_error(x->info.source_line, "Unknown expression in the operator");
  }
}

void ASTOperator_check(SymbolList* list, ASTNode* _self) {
  _self->scope = list;
  ASTOperator* self = (ASTOperator*) _self;

  ((ASTNode*) self->lvalue)->check(list, (ASTNode*) self->lvalue);
  ((ASTNode*) self->rvalue)->check(list, (ASTNode*) self->rvalue);
  _ASTOperator_check_return_type(list, (ASTNode*) self->lvalue);
  _ASTOperator_check_return_type(list, (ASTNode*) self->rvalue);
  // TODO Handle the special case for "==" and "!-" which could be done with char.
}

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

ASTOperator* ASTOperator_create(ASTExpression* lvalue, ASTExpression* rvalue, int operator_token, ASTInfo info) {
  ASTOperator* result = malloc(sizeof(ASTOperator));
  result->lvalue = lvalue;
  result->rvalue = rvalue;
  result->operator_token = operator_token;
  result->expression.node.free = ASTOperator_free;
  result->expression.node.code_gen = ASTOperator_code_gen;
  result->expression.node.check = ASTOperator_check;
  result->expression.node.info = info;
  return result;
}

// typedef struct {
//   char value[64];
// } ASTIdentifier;
void ASTIdentifier_check(SymbolList* list, ASTNode* _self) {
  _self->scope = list;
  ASTIdentifier* self = (ASTIdentifier*) _self;

  SymbolList_exist(list, self);
  if (!SymbolList_exist(list, self)) {
    print_error(_self->info.source_line, "Variable name '%s' is not defined", self->value);
  }
}

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
  free(self->value);
  if (self->expression.node.code) { free(self->expression.node.code); }
  free(self);
}

ASTIdentifier* ASTIdentifier_create(char* value, ASTInfo info) {
  ASTIdentifier* result = malloc(sizeof(ASTIdentifier));
  result->value = value;
  result->expression.node.free = ASTIdentifier_free;
  result->expression.node.code_gen = ASTIdentifier_code_gen;
  result->expression.node.check = ASTIdentifier_check;
  result->expression.node.info = info;
  return result;
}

// typedef struct {
//   ASTExpression expression;
//   ASTIdentifier* name;
//   ArrayList* arguments;
// } ASTFunctionCall;
void ASTFunctionCall_check(SymbolList* list, ASTNode* _self) {
  _self->scope = list;
  ASTFunctionCall* self = (ASTFunctionCall*) _self;

  ASTNode* _func;
  if (!(_func = SymbolList_exist(list, self->name))) {
    print_error(_self->info.source_line, "Function name '%s' is not defined", self->name->value);
  }

  ASTDeclarationFunction* func = (ASTDeclarationFunction*) _func;
  if (func->parameters->size != self->arguments->size) {
    print_error(_self->info.source_line, "There is no function '%s' with %d arguments defined", self->name->value, self->arguments->size);
  }

  for (unsigned int i = 0; i < func->parameters->size; ++i) {
    ASTDeclarationVariable* current_param = (ASTDeclarationVariable*) func->parameters->content[i];
    ASTType_t param_type = current_param->type == ASTINTEGER ? ASTINTEGER : ASTCHAR;
    switch (self->arguments->content[i]->info.type) {
      case ASTCHAR:
        if (param_type != ASTCHAR) {
          print_error(_self->info.source_line, "Expected a 'char' in argument number %d", i);
        }
        continue;
      case ASTOPERATOR:
      case ASTINTEGER:
        if (param_type != ASTINTEGER) {
          print_error(_self->info.source_line, "Expected a 'integer' in argument number %d", i);
        }
        continue;
      case ASTFUNCTIONCALL: {
        ASTDeclarationFunction* tmp = (ASTDeclarationFunction*) SymbolList_exist(list, ((ASTFunctionCall*) self->arguments->content[i])->name);
        if (tmp->name->type != param_type) {
          print_error(_self->info.source_line, "Invalid return type for function '%s' in argument number %d", tmp->name->name->value, i);
        }
        continue;
      }
      case ASTVARIABLE: {
        ASTDeclarationVariable* tmp = (ASTDeclarationVariable*) SymbolList_exist(list, (ASTIdentifier*) self->arguments->content[i]);
        if (tmp->type != param_type) {
          print_error(_self->info.source_line, "Invalid type for the variable '%s' in argument number %d", tmp->name->value, i);
        }
        continue;
      }
      default:
        print_error(_self->info.source_line, "Unknown expression in argument number %d", i);
    }
  }
}

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

ASTFunctionCall* ASTFunctionCall_create(ASTIdentifier* name, ArrayList* arguments, ASTInfo info) {
  ASTFunctionCall* result = malloc(sizeof(ASTFunctionCall));
  result->name = name;
  result->arguments = arguments;
  result->expression.node.free = ASTFunctionCall_free;
  result->expression.node.code_gen = ASTFunctionCall_code_gen;
  result->expression.node.check = ASTFunctionCall_check;
  result->expression.node.info = info;
  return result;
}
