#ifndef __DECLARATION_H__
#define __DECLARATION_H__

#include "AST.h"
#include "ASTStatement.h"
#include "ASTExpression.h"

//  +-------------+--------------+
//  |                            |
//  |         Declaration        |
//  |                            |
//  +-+--------------------------+
//    ^           +-------------------------------------+
//    |           | FunctionDeclaration                 |
//    |           | + type: Identifier                  |
//    |           | | name: Identifier                  |
//    +-----------+ | parameters: VariableDeclaration[] |
//    |           | + block: Block                      |
//    |           +-------------------------------------+
//    |
//    |           +----------------------------+
//    |           | VariableDeclaration        |
//    |           | + type: Identifier         |
//    +-----------+ + name: Identifier         |
//                +----------------------------+

/**
 * @brief Base data structure used for declaration.
 */
typedef struct {
  ASTNode node;
} ASTDeclaration;

typedef struct {
  ASTDeclaration declaration;
  ASTIdentifier* type;
  char* name;
} ASTDeclarationVariable;

typedef struct {
  ASTDeclaration declaration;
  ASTDeclarationVariable* name;
  ASTDeclarationVariable** parameters;
  ASTBlock* block;
} ASTDeclarationFunction;

ASTDeclarationFunction* ASTDeclarationFunction_create(char* type, char* name, ASTDeclarationVariable* parameters, ASTBlock* block);

#endif
