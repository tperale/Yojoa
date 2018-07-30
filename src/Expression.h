/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "Parser.h"

/**
 * @brief The expression structure
 */
typedef struct _ASTNode
{
    enum yytokentype type;///< type of operation

    int value;

    struct _ASTNode *left; ///< left side of the tree
    struct _ASTNode *right;///< right side of the tree
} ASTNode;


/**
 * @brief It creates an identifier
 * @param value The number value
 * @return The expression or NULL in case of no memory
 */
ASTNode *createNumber(int value);

/**
 * @brief It creates an operation
 * @param type The operation type
 * @param left The left operand
 * @param right The right operand
 * @return The expression or NULL in case of no memory
 */
ASTNode *createOperation(EOperationType type, ASTNode* left, ASTNode* right);

/**
 * @brief Deletes a expression
 * @param b The expression
 */
void deleteExpression(ASTNode* b);

#endif // __EXPRESSION_H__
