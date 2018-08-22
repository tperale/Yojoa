#include "ASTStatement.h"
#include <stdlib.h>
#include <stdio.h>

// typedef struct ASTBlock {
//   ArrayList* variables; // containing ASTDeclarationVariable
//   ArrayList* statements; // containing ASTStatement
// } ASTBlock;
char* ASTBlock_code_gen(void* _self) {
  ASTBlock* self = (ASTBlock*) _self;

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

  asprintf(&((ASTNode*) self)->code, "%s", statements);

  return ((ASTNode*) self)->code;
}

void ASTBlock_free(void* _self) {
  ASTBlock* self = (ASTBlock*) _self;
  self->variables->free(self->variables);
  self->statements->free(self->statements);

  free(self->statement.node.code);
  free(self);
}

ASTBlock* ASTBlock_create(ArrayList* var_decl, ArrayList* statements) {
  ASTBlock* result = (ASTBlock*) malloc(sizeof(ASTBlock));
  result->variables = var_decl;
  result->statements = statements;

  result->statement.node.free = ASTBlock_free;
  result->statement.node.code_gen = ASTBlock_code_gen;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTIdentifier* lvalue;
//   ASTExpression* rvalue;
// } ASTStatementAssignment;
void ASTStatementAssignment_free(void* _self) {
  ASTStatementAssignment* self = (ASTStatementAssignment*) _self;

  ASTIdentifier_free(self->lvalue);
  ((ASTNode*) self->rvalue)->free(self->rvalue);
  free(self);
}

ASTStatementAssignment* ASTStatementAssignment_create(ASTIdentifier* lvalue, ASTExpression* rvalue) {
  ASTStatementAssignment* result = (ASTStatementAssignment*) malloc(sizeof(ASTStatementAssignment));
  result->lvalue = lvalue;
  result->rvalue = rvalue;

  result->statement.node.free =  ASTStatementAssignment_free;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTExpression* condition;
//   ASTBlock* if_block;
//   ASTBlock* else_block;
// } ASTStatementCondition;
void ASTStatementCondition_free(void* _self) {
  ASTStatementCondition* self = (ASTStatementCondition*) _self;

  ((ASTNode*) self->condition)->free(self->condition);
  ASTBlock_free(self->if_block);
  if (self->else_block) ASTBlock_free(self->else_block);
  free(self);
}

ASTStatementCondition* ASTStatementCondition_create(ASTExpression* cond, ASTStatement* if_block, ASTStatement* else_block) {
  ASTStatementCondition* result = (ASTStatementCondition*) malloc(sizeof(ASTStatementCondition));

  result->condition = cond;
  result->if_block = if_block;
  result->else_block = else_block;

  result->statement.node.free = ASTStatementCondition_free;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTExpression* condition;
//   ASTBlock* content;
// } ASTStatementLoop;
void ASTStatementLoop_free(void* _self) {
  ASTStatementLoop* self = (ASTStatementLoop*) _self;

  ((ASTNode*) self->condition)->free(self->condition);
  ASTBlock_free(self->content);
  free(self);
}

ASTStatementLoop* ASTStatementLoop_create(ASTExpression* cond, ASTStatement* content) {
  ASTStatementLoop* result = (ASTStatementLoop*) malloc(sizeof(ASTStatementLoop));

  result->condition = cond;
  result->content = content;

  result->statement.node.free = ASTStatementLoop_free;

  return result;
}

// typedef struct {
//   ASTStatement statement;
//   ASTExpression* value;
// } ASTStatementReturn;
char* ASTStatementReturn_code_gen(void* _self) {
  ASTStatementReturn* self = (ASTStatementReturn*) _self;

  asprintf(&((ASTNode*) self)->code, "%s", ((ASTNode*) self->value)->code_gen(self->value));

  return ((ASTNode*) self)->code;
}

void ASTStatementReturn_free(void* _self) {
  ASTStatementReturn* self = (ASTStatementReturn*) _self;

  ((ASTNode*) self->value)->free(self->value);
  free(self->statement.node.code);
  free(self);
}

ASTStatementReturn* ASTStatementReturn_create(ASTExpression* value) {
  ASTStatementReturn* result = (ASTStatementReturn*) malloc(sizeof(ASTStatementReturn));

  result->value = value;

  result->statement.node.free = ASTStatementReturn_free;
  result->statement.node.code_gen = ASTStatementReturn_code_gen;

  return result;
}
