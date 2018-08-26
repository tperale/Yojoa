#include "ASTStatement.h"
#include <stdlib.h>
#include <stdio.h>

// typedef struct ASTBlock {
//   ArrayList* variables; // containing ASTDeclarationVariable
//   ArrayList* statements; // containing ASTStatement
// } ASTBlock;
void ASTBlock_check(SymbolList* list, ASTNode* _self) {
  ASTBlock* self = (ASTBlock*) _self;
  SymbolList_block_create(list, self); // TODO Should return this or put yourself into children of "list"
}

char* ASTBlock_code_gen(void* _self) {
  ASTBlock* self = (ASTBlock*) _self;

  char* variables;
  asprintf(&variables, " ");
  for (unsigned int i = 0; i < self->variables->size; ++i) {
    char* buffer;
    ASTNode* node = self->variables->get(self->variables, i);
    asprintf(&buffer, "%s %s", variables, node->code_gen(node));
    free(variables);
    asprintf(&variables, "%s", buffer);
    free(buffer);
  }

  char* statements;
  asprintf(&statements, " ");
  for (unsigned int i = 0; i < self->statements->size; ++i) {
    char* buffer;
    ASTNode* node = self->statements->get(self->statements, i);
    asprintf(&buffer, "%s %s", statements, node->code_gen(node));
    free(statements);
    asprintf(&statements, "%s", buffer);
    free(buffer);
  }

  asprintf(&((ASTNode*) self)->code, "%s%s", variables, statements);
  free(variables);
  free(statements);

  return ((ASTNode*) self)->code;
}

void ASTBlock_free(void* _self) {
  ASTBlock* self = (ASTBlock*) _self;
  self->variables->free(self->variables);
  self->statements->free(self->statements);

  free(self->statement.node.code);
  free(self);
}

ASTBlock* ASTBlock_create(ArrayList* var_decl, ArrayList* statements, ASTInfo info) {
  ASTBlock* result = (ASTBlock*) malloc(sizeof(ASTBlock));
  result->variables = var_decl;
  result->statements = statements;

  result->statement.node.free = ASTBlock_free;
  result->statement.node.code_gen = ASTBlock_code_gen;
  result->statement.node.check = ASTBlock_check;
  result->statement.node.info = info;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTIdentifier* lvalue;
//   ASTExpression* rvalue;
// } ASTStatementAssignment;
void ASTStatementAssignment_check(SymbolList* list, ASTNode* _self) {
  ASTStatementAssignment* self = (ASTStatementAssignment*) _self;
  ((ASTNode*) self->lvalue)->check(list, (ASTNode*) self->lvalue);
  ((ASTNode*) self->rvalue)->check(list, (ASTNode*) self->rvalue);
}

char* ASTStatementAssignment_code_gen(void* _self) {
  ASTStatementAssignment* self = (ASTStatementAssignment*) _self;

  // TODO link with the symtable
  asprintf(&((ASTNode*) self)->code, "(set_local $%s %s)", self->lvalue->value, ((ASTNode*) self->rvalue)->code_gen(self->rvalue));

  return ((ASTNode*) self)->code;
}

void ASTStatementAssignment_free(void* _self) {
  ASTStatementAssignment* self = (ASTStatementAssignment*) _self;

  ASTIdentifier_free(self->lvalue);
  ((ASTNode*) self->rvalue)->free(self->rvalue);
  free(self);
}

ASTStatementAssignment* ASTStatementAssignment_create(struct ASTIdentifier* lvalue, struct ASTExpression* rvalue, ASTInfo info) {
  ASTStatementAssignment* result = (ASTStatementAssignment*) malloc(sizeof(ASTStatementAssignment));
  result->lvalue = lvalue;
  result->rvalue = rvalue;

  result->statement.node.free = ASTStatementAssignment_free;
  result->statement.node.code_gen = ASTStatementAssignment_code_gen;
  result->statement.node.check = ASTStatementAssignment_check;
  result->statement.node.info = info;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTExpression* condition;
//   ASTBlock* if_block;
//   ASTBlock* else_block;
// } ASTStatementCondition;
void ASTStatementCondition_check(SymbolList* list, ASTNode* _self) {
  ASTStatementCondition* self = (ASTStatementCondition*) _self;
  ((ASTNode*) self->condition)->check(list, (ASTNode*) self->condition);
  if (self->else_block) {
    ((ASTNode*) self->else_block)->check(list, (ASTNode*) self->else_block);
  }
}

char* ASTStatementCondition_code_gen(void* _self) {
  ASTStatementCondition* self = (ASTStatementCondition*) _self;

  char* condition_code = ((ASTNode*) self->condition)->code_gen(self->condition);
  char* if_code = ((ASTNode*) self->if_block)->code_gen(self->if_block);
  char* else_code = self->else_block ? ((ASTNode*) self->else_block)->code_gen(self->else_block) : "";
  asprintf(&((ASTNode*) self)->code, "%s(if (then %s)(else %s))", condition_code, if_code, else_code);

  return ((ASTNode*) self)->code;
}

void ASTStatementCondition_free(void* _self) {
  ASTStatementCondition* self = (ASTStatementCondition*) _self;

  ((ASTNode*) self->condition)->free(self->condition);
  ASTBlock_free(self->if_block);
  if (self->else_block) ASTBlock_free(self->else_block);
  free(self);
}

ASTStatementCondition* ASTStatementCondition_create(struct ASTExpression* cond, ASTStatement* if_block, ASTStatement* else_block, ASTInfo info) {
  ASTStatementCondition* result = (ASTStatementCondition*) malloc(sizeof(ASTStatementCondition));

  result->condition = cond;
  result->if_block = if_block;
  result->else_block = else_block;

  result->statement.node.free = ASTStatementCondition_free;
  result->statement.node.code_gen = ASTStatementCondition_code_gen;
  result->statement.node.check = ASTStatementCondition_check;
  result->statement.node.info = info;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTExpression* condition;
//   ASTBlock* content;
// } ASTStatementLoop;
void ASTStatementLoop_check(SymbolList* list, ASTNode* _self) {
  ASTStatementLoop* self = (ASTStatementLoop*) _self;
  ((ASTNode*) self->condition)->check(list, (ASTNode*) self->condition);
  ((ASTNode*) self->content)->check(list, (ASTNode*) self->content);
}

char* ASTStatementLoop_code_gen(void* _self) {
  ASTStatementLoop* self = (ASTStatementLoop*) _self;
  char* condition_code = ((ASTNode*) self->condition)->code_gen(self->condition);
  char* content = ((ASTNode*) self->content)->code_gen(self->content);
  asprintf(&((ASTNode*) self)->code, "(block (loop (br_if 1 (i32.eq %s (i32.const 0))) %s (br 0)))", condition_code, content);
  return ((ASTNode*) self)->code;
}

void ASTStatementLoop_free(void* _self) {
  ASTStatementLoop* self = (ASTStatementLoop*) _self;
  ((ASTNode*) self->condition)->free(self->condition);
  ASTBlock_free(self->content);
  if (self->statement.node.code) { free(self->statement.node.code); }
  free(self);
}

ASTStatementLoop* ASTStatementLoop_create(struct ASTExpression* cond, ASTStatement* content, ASTInfo info) {
  ASTStatementLoop* result = (ASTStatementLoop*) malloc(sizeof(ASTStatementLoop));

  result->condition = cond;
  result->content = content;

  result->statement.node.free = ASTStatementLoop_free;
  result->statement.node.code_gen = ASTStatementLoop_code_gen;
  result->statement.node.check = ASTStatementLoop_check;
  result->statement.node.info = info;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTExpression* value;
// } ASTStatementReturn;
void ASTStatementReturn_check(SymbolList* list, ASTNode* _self) {
  ASTStatementReturn* self = (ASTStatementReturn*) _self;

  ((ASTNode*) self->value)->check(list, (ASTNode*) self->value);
}

char* ASTStatementReturn_code_gen(void* _self) {
  ASTStatementReturn* self = (ASTStatementReturn*) _self;

  asprintf(&((ASTNode*) self)->code, "(return %s)", ((ASTNode*) self->value)->code_gen(self->value));

  return ((ASTNode*) self)->code;
}

void ASTStatementReturn_free(void* _self) {
  ASTStatementReturn* self = (ASTStatementReturn*) _self;
  ((ASTNode*) self->value)->free(self->value);
  if (self->statement.node.code) { free(self->statement.node.code); }
  free(self);
}

ASTStatementReturn* ASTStatementReturn_create(struct ASTExpression* value, ASTInfo info) {
  ASTStatementReturn* result = (ASTStatementReturn*) malloc(sizeof(ASTStatementReturn));

  result->value = value;

  result->statement.node.free = ASTStatementReturn_free;
  result->statement.node.code_gen = ASTStatementReturn_code_gen;
  result->statement.node.check = ASTStatementReturn_check;
  result->statement.node.info = info;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTExpression* value;
// } ASTStatementWrite;
void ASTStatementWrite_check(SymbolList* list, ASTNode* _self) {
  ASTStatementWrite* self = (ASTStatementWrite*) _self;
  ((ASTNode*) self->value)->check(list, (ASTNode*) self->value);
}

char* ASTStatementWrite_code_gen(void* _self) {
  ASTStatementWrite* self = (ASTStatementWrite*) _self;

  return ((ASTNode*) self)->code;
}

void ASTStatementWrite_free(void* _self) {
  ASTStatementWrite* self = (ASTStatementWrite*) _self;
  ((ASTNode*) self->value)->free(self->value);
  if (self->statement.node.code) { free(self->statement.node.code); }
  free(self);
}

ASTStatementWrite* ASTStatementWrite_create(struct ASTExpression* value, ASTInfo info) {
  ASTStatementWrite* result = (ASTStatementWrite*) malloc(sizeof(ASTStatementWrite));
  result->value = value;

  result->statement.node.free = ASTStatementWrite_free;
  result->statement.node.code_gen = ASTStatementWrite_code_gen;
  result->statement.node.check = ASTStatementWrite_check;
  result->statement.node.info = info;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTIdentifier* ref;
// } ASTStatementRead;
void ASTStatementRead_check(SymbolList* list, ASTNode* _self) {
  ASTStatementRead* self = (ASTStatementRead*) _self;
  ((ASTNode*) self->ref)->check(list, (ASTNode*) self->ref);
}

char* ASTStatementRead_code_gen(void* _self) {
  ASTStatementRead* self = (ASTStatementRead*) _self;
  // TODO
  return ((ASTNode*) self)->code;
}

void ASTStatementRead_free(void* _self) {
  ASTStatementRead* self = (ASTStatementRead*) _self;
  ((ASTNode*) self->ref)->free(self->ref);
  if (self->statement.node.code) { free(self->statement.node.code); }
  free(self);
}

ASTStatementRead* ASTStatementRead_create(struct ASTIdentifier* ref, ASTInfo info) {
  ASTStatementRead* result = (ASTStatementRead*) malloc(sizeof(ASTStatementRead));
  result->ref = ref;

  result->statement.node.free = ASTStatementRead_free;
  result->statement.node.code_gen = ASTStatementRead_code_gen;
  result->statement.node.check = ASTStatementRead_check;
  result->statement.node.info = info;

  return result;
}
