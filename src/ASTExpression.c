/*
 * Expression.c
 * Implementation of functioASTIdentifierns used to build the syntax tree.
 */

#include "ASTExpression.h"
#include <string.h>
#include <stdlib.h>

/**
 * typedef struct {
 *   char value[64];
 * } ASTIdentifier;
 */
void ASTIdentifier_free(ASTIdentifier* self) {
  free(self);
}

ASTIdentifier* ASTIdentifier_create(char* value) {
  ASTIdentifier* result = malloc(sizeof(ASTIdentifier));
  strcpy(result->value, value);
  return result;
}
