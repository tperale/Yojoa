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
char* ASTDeclarationVariable_code_gen(ASTNode* _self) {
  ASTDeclarationVariable* self = (ASTDeclarationVariable*) _self;

  if (self->type.length == 0) {
    char* type;
    asprintf(&type, "i32");

    char* name = self->name->value;
    if (_self->info.type == ASTVARIABLE_DECLARATION) {
      asprintf(&((ASTNode*) self)->code, "(local $%s %s)", name, type);
    } else {
      asprintf(&((ASTNode*) self)->code, "(global $%s (mut %s) (i32.const 0))", name, type);
    }
    free(type);
    return ((ASTNode*) self)->code;
  } else {
    return "";
  }
}

void ASTDeclarationVariable_free(ASTNode* _self) {
  ASTDeclarationVariable* self = (ASTDeclarationVariable*) _self;
  ASTIdentifier_free((ASTNode*) self->name);
  if (self->declaration.node.code) { free(self->declaration.node.code); }
  free(self);
}

ASTDeclarationVariable* ASTDeclarationVariable_create(ASTTypePrimitive_t type, struct ASTIdentifier* name, ASTInfo info) {
  static int global_memory_offset = 0; // This static variable get incremented for each creation call for an array.
  ASTDeclarationVariable* result = malloc(sizeof(ASTDeclarationVariable));

  if (info.type != ASTVARIABLE_PARAM && type.length) {
    result->memory_offset = global_memory_offset;
    global_memory_offset += (type.length * 4);
  }

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
char* ASTDeclarationFunction_code_gen(ASTNode* _self) {
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

void ASTDeclarationFunction_free(ASTNode* _self) {
  ASTDeclarationFunction* self = (ASTDeclarationFunction*) _self;
  ASTDeclarationVariable_free((ASTNode*) self->name);
  self->parameters->free(self->parameters);
  ASTBlock_free((ASTNode*) self->block);
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
