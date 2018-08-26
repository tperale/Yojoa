#ifndef __DECLARATION_H__
#define __DECLARATION_H__

#include "AST.h"
#include "ASTStatement.h"
#include "ASTExpression.h"
#include "ArrayList.h"

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

struct ASTBlock;

/**
 * @brief Base data structure used for declaration.
 */
typedef struct {
  ASTNode node;
} ASTDeclaration;

typedef struct ASTDeclarationVariable {
  ASTDeclaration declaration;
  ASTType type;
  ASTIdentifier* name;
} ASTDeclarationVariable;

void ASTDeclarationVariable_free(void* self);
ASTDeclarationVariable* ASTDeclarationVariable_create(int type, ASTIdentifier* name, ASTInfo);

typedef struct {
  ASTDeclaration declaration;
  ASTDeclarationVariable* name;
  ArrayList* parameters;
  struct ASTBlock* block;
} ASTDeclarationFunction;

void ASTDeclarationFunction_free(void* self);
ASTDeclarationFunction* ASTDeclarationFunction_create(ASTDeclarationVariable* name, ArrayList* parameters, struct ASTBlock* block, ASTInfo);

#endif
