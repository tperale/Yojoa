#include "ASTDeclaration.h"

/**
 * typedef struct {
 *   ASTDeclaration declaration;
 *   ASTTypedIdentifier id;
 *   ASTDeclarationVariable* parameters;
 *   ASTBlock block;
 * } ASTDeclarationFunction;
 */
void ASTDeclarationFunction_free(ASTDeclarationFunction* self) {
  // ASTTypedIdentifier_free(self->id);

  // TODO have to see if it really has to be of ASTDeclarationVariable type. But ok it should be fine because the DeclarationVariable is basically type + name.
  // ASTDeclarationVariable_free(self->parameters)
  // ASTBlock_free(self->block);
  free(self);
  *self = NULL;
}

ASTDeclarationFunction* ASTDeclarationFunction_create(ASTDeclarationVariable* name, ASTDeclarationVariable** parameters, ASTBlock* block) {
  ASTDeclarationFunction* result = (ASTDeclarationFunction*) malloc(sizeof(ASTDeclarationFunction));

  result->name = name;
  result->parameters = parameters;
  result->block = block;

  return result;
}
