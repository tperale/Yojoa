#ifndef __DECLARATION_H__
#define __DECLARATION_H__

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
  ASTIdentifier type;
  char* name;
} ASTDeclarationVariable;

typedef struct {
  ASTDeclaration declaration;
  ASTIdentifier type;
  char* name;
  ASTDeclarationVariable* parameters;
  ASTBlock block;
} ASTDeclarationFunction;

#endif
