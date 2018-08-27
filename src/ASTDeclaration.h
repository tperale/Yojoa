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
struct ASTIdentifier;

/**
 * @brief Base data structure used for declaration.
 */
typedef struct {
  ASTNode node;
} ASTDeclaration;

typedef struct ASTDeclarationVariable {
  ASTDeclaration declaration;
  ASTTypePrimitive_t type;
  struct ASTIdentifier* name;
} ASTDeclarationVariable;

void ASTDeclarationVariable_free(ASTNode* self);
ASTDeclarationVariable* ASTDeclarationVariable_create(ASTTypePrimitive_t type, struct ASTIdentifier* name, ASTInfo);

typedef struct ASTDeclarationFunction {
  ASTDeclaration declaration;
  ASTDeclarationVariable* name;
  ArrayList* parameters;
  struct ASTBlock* block;
} ASTDeclarationFunction;

void ASTDeclarationFunction_free(ASTNode* self);
ASTDeclarationFunction* ASTDeclarationFunction_create(ASTDeclarationVariable* name, ArrayList* parameters, struct ASTBlock* block, ASTInfo);

#endif
