/*
 * Expression.c
 * Implementation of functions used to build the syntax tree.
 */

#include "Expression.h"

#include <stdlib.h>

/**
 * @brief Allocates space for expression
 * @return The expression or NULL if not enough memory
 */
static ASTNode *allocateExpression() {
  ASTNode *b = (ASTNode*) malloc(sizeof(SExpression));

  if (b == NULL) {
    return NULL;
  }

  b->type = eVALUE;
  b->value = 0;

  b->left = NULL;
  b->right = NULL;

  return b;
}

ASTNode *createNumber(int value) {
  ASTNode *b = allocateExpression();

  if (b == NULL) {
    return NULL;
  }

  b->type = eVALUE;
  b->value = value;

  return b;
}

ASTNode *createOperation(EOperationType type, SExpression *left, SExpression *right) {
  SExpression *b = allocateExpression();

  if (b == NULL) {
    return NULL;
  }

  b->type = type;
  b->left = left;
  b->right = right;

  return b;
}

void deleteExpression(ASTNode* b) {
  if (b == NULL) {
    return;
  }

  deleteExpression(b->left);
  deleteExpression(b->right);

  free(b);
}
