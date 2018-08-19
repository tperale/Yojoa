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
void ASTDeclarationVariable_free(ASTDeclarationVariable* self) {
  ASTIdentifier_free(self->name);

  free(self);
}

ASTDeclarationVariable* ASTDeclarationVariable_create(ASTType type, ASTIdentifier* name) {
  ASTDeclarationVariable* result = malloc(sizeof(ASTDeclarationVariable));

  result->type = type;
  result->name = name;

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
void ASTDeclarationFunction_free(ASTDeclarationFunction* self) {
  ASTDeclarationVariable_free(self->name);

  for (int i = 0; self->parameters != NULL; ++i) {
    ASTDeclarationVariable_free(self->parameters[i]);
  }
  // TODO ASTBlock_free(self->block);
  free(self);
}

ASTDeclarationFunction* ASTDeclarationFunction_create(ASTDeclarationVariable* name, ASTDeclarationVariable** parameters, ASTBlock* block) {
  ASTDeclarationFunction* result = (ASTDeclarationFunction*) malloc(sizeof(ASTDeclarationFunction));

  result->name = name;
  result->parameters = parameters;
  result->block = block;

  return result;
}
