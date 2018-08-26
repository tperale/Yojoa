#include "ASTDeclaration.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * typedef struct {
 *   ASTDeclaration declaration;
 *   ASTType type;
 *   ASTIdentifier* name;
 * } ASTDeclarationVariable;
*/
char* ASTDeclarationVariable_code_gen(void* _self) {
  ASTDeclarationVariable* self = (ASTDeclarationVariable*) _self;

  char* type;
  // if (self->type == INT) {
    asprintf(&type, "i32");
  // }

  char* scope;
  if (self->scope == LOCAL) {
    asprintf(&scope, "local");
  }

  char* name = self->name->value;
  asprintf(&((ASTNode*) self)->code, "(%s $%s %s)", scope, name, type);
  free(type);
  free(scope);

  return ((ASTNode*) self)->code;
}

void ASTDeclarationVariable_free(void* _self) {
  ASTDeclarationVariable* self = (ASTDeclarationVariable*) _self;
  ASTIdentifier_free(self->name);
  if (self->declaration.node.code) { free(self->declaration.node.code); }
  free(self);
}

ASTDeclarationVariable* ASTDeclarationVariable_create(ASTType type, ASTIdentifier* name, ASTInfo info) {
  ASTDeclarationVariable* result = malloc(sizeof(ASTDeclarationVariable));

  result->type = type;
  result->name = name;

  result->declaration.node.free = ASTDeclarationVariable_free;
  result->declaration.node.code_gen = ASTDeclarationVariable_code_gen;
  result->declaration.node.info = info;

  return result;
}

/**
 * typedef struct {
 *   ASTDeclaration declaration;
 *   ASTDeclarationVariable* name;
 *   ASTDeclarationVariable** parameters;
 *   ASTBlock block;
 * } ASTDeclarationFunction;
 */
char* ASTDeclarationFunction_code_gen(void* _self) {
  ASTDeclarationFunction* self = (ASTDeclarationFunction*) _self;

  char* arguments;
  asprintf(&arguments, " ");
  for (unsigned int i = 0; i < self->parameters->size; ++i) {
    char* buffer;
    asprintf(&buffer, "%s (param $%s i32)", arguments, ((ASTDeclarationVariable*) self->parameters->get(self->parameters, i))->name->value);
    free(arguments);
    asprintf(&arguments, "%s", buffer);
    free(buffer);
  }

  char* name = self->name->name->value;
  asprintf(&((ASTNode*) self)->code, "(func $%s %s (result i32) %s)(export \"%s\" (func $%s))", name, arguments, ((ASTNode*) self->block)->code_gen((void*) self->block), name, name);

  return ((ASTNode*) self)->code;
}

void ASTDeclarationFunction_free(void* _self) {
  ASTDeclarationFunction* self = (ASTDeclarationFunction*) _self;
  ASTDeclarationVariable_free(self->name);
  self->parameters->free(self->parameters);
  ASTBlock_free(self->block);
  if (self->declaration.node.code) { free(self->declaration.node.code); }
  free(self);
}

ASTDeclarationFunction* ASTDeclarationFunction_create(ASTDeclarationVariable* name, ArrayList* parameters, ASTBlock* block, ASTInfo info) {
  ASTDeclarationFunction* result = (ASTDeclarationFunction*) malloc(sizeof(ASTDeclarationFunction));

  result->name = name;
  result->parameters = parameters;
  result->block = block;

  result->declaration.node.free = ASTDeclarationFunction_free;
  result->declaration.node.code_gen = ASTDeclarationFunction_code_gen;
  result->declaration.node.info = info;

  return result;
}
