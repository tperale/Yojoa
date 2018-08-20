#include "ASTDeclaration.h"
#include <stddef.h>
#include <stdlib.h>

/**
 * typedef struct {
 *   ASTDeclaration declaration;
 *   ASTType type;
 *   ASTIdentifier* name;
 * } ASTDeclarationVariable;
*/
void ASTDeclarationVariable_free(void* _self) {
  ASTDeclarationVariable* self = (ASTDeclarationVariable*) _self;
  ASTIdentifier_free(self->name);

  free(self);
}

ASTDeclarationVariable* ASTDeclarationVariable_create(ASTType type, ASTIdentifier* name) {
  ASTDeclarationVariable* result = malloc(sizeof(ASTDeclarationVariable));

  result->type = type;
  result->name = name;

  result->declaration.node.free = ASTDeclarationVariable_free;

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
void ASTDeclarationFunction_free(void* _self) {
  ASTDeclarationFunction* self = (ASTDeclarationFunction*) _self;
  ASTDeclarationVariable_free(self->name);
  self->parameters->free(self->parameters);
  // TODO ASTBlock_free(self->block);
  free(self);
}

ASTDeclarationFunction* ASTDeclarationFunction_create(ASTDeclarationVariable* name, ArrayList* parameters, ASTBlock* block) {
  ASTDeclarationFunction* result = (ASTDeclarationFunction*) malloc(sizeof(ASTDeclarationFunction));

  result->name = name;
  result->parameters = parameters;
  result->block = block;

  result->declaration.node.free = ASTDeclarationFunction_free;

  return result;
}
