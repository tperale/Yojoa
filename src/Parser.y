%{
#include <stdio.h>
#include "AST.h"
#include "ASTDeclaration.h"
#include "ASTStatement.h"
#include "ASTExpression.h"

extern int yylex();
void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

%output  "Parser.c"
%defines "Parser.h"

/**
 * Value type specification
 */
%union {
  // TODO add the *_create(**) func declaration ?
  ASTDeclarationFunction* fun_decl;
  ASTDeclarationVariable* var_decl;
  ASTBlock* block;
  ASTStatement* statement;
  ASTExpression* expression;
  ASTIdentifier* identifier;
  ASTIdentifier** identifier_list;
  ASTDeclarationVariable** var_decl_list;
  ASTStatement** statement_list;
  int token;
}

/**
 *
 */
%token <string> NAME              // A String starting with a letter followed by 0 or more letters, digits or undercores.
%token <value> NUMBER         // Is a string of digits
%token <char> QCHAR            // Is a character between single quotes

%token <token> INT CHAR                   // Type tokens
%token <token> IF ELSE RETURN SEMICOLON COMMA WRITE READ LENGTH WHILE

%token <token> LPAR RPAR
%token <token> LBRACE RBRACE
%token <token> LBRACK RBRACK
%token <token> ASSIGN

/**
 * Type of the nonterminal symbol specification
 */
%type <fun_decl> fun_declaration
%type <var_decl_list> formal_pars var_declarations
%type <var_decl> var_declaration var_identifier
%type <statement> statement
%type <statement_list> statements
%type <block> block
%type <token> binop unop
%type <expression> lexp exp
%type <identifier_list> pars
%type <identifier> var

/**
 *
 */
%left PLUS TIMES DIVIDE           // Calculus binary operators
%nonassoc MINUS                   // Could be either binary or unary
%left EQUAL GREATER LESS NEQUAL   // Logic binary operators
%right NOT                        // Logic unary operator

%start program

%%
program                           // A program is a list of function declarations
		: declaration
    | declaration program
		;

declaration                       // The program body should only be made of function declaration and global variable declaration.
    : fun_declaration // TODO trigger a function to define the function
		| var_declaration // TODO trigger a function to define the variable
		;

fun_declaration                   //
	  : var_identifier LPAR formal_pars RPAR block { $$ = ASTDeclarationFunction_create($1, $2, $4, $6) }
		;

formal_pars                            // formal_pars is the declaration of arguments in parentheses
    : var_identifier COMMA formal_pars // Can be either multiple declaration separated by commas
    | var_identifier                   // a simple declaration
		|                                  // or no declaration
		;

block                             // The content of a function, if, while. Variable declarations are always done on the top of the block.
		: LBRACE var_declarations statements RBRACE // ex: { int foo; }
		;

var_declarations                   // How to do a variable declaration
    : var_declaration var_declarations
    |
		;

var_declaration                   // How to do a variable declaration
    : var_identifier SEMICOLON // { $$ = $1 } // ex: int foo;
		;

var_identifier                    // A simple way to identify combination of variable type and name
    : type NAME // { $$ = ASTDeclarationVariable_create($1, $2) }
    ;

type                              // Their are only two primitive data types (char, int) and the composed data types
		: INT
		| CHAR
		| type LBRACK exp RBRACK // array type (eg: int[4])
		;

statements                        // Statements express how multiple statement need to be combined
	  : statement SEMICOLON statements // Can be either multiple statement semicolon separated
    | statement
		| // or no statement
		;

statement                         // Statement express possible actions you can do on the programming language
	  : IF LPAR exp RPAR statement
		| IF LPAR exp RPAR statement ELSE statement
		| WHILE LPAR exp RPAR statement
		| lexp ASSIGN exp // assignment
		| RETURN exp // return statement
		| NAME LPAR pars RPAR	// function call
		| block
		| WRITE exp
		| READ lexp
		;

lexp                              // left expression are either a variable name or variable name array access
		: var // ex: foo TODO: function call for variable lookup
		| lexp LBRACK exp RBRACK	// ex: foo[2] TODO:
		;

exp
		: lexp
		| exp binop exp // ex: foo == bar TODO: Function call for calculation
		| unop exp // ex: !foo TODO: Function call for calculation
		| LPAR exp RPAR // ex: (foo)
		| NUMBER // ex: 1
		| NAME LPAR pars RPAR	// function call ex: foo(bar, 2)
		| QCHAR // Just a charachter ex: 'c'
		| LENGTH lexp	// size of an array ex: length foo
		;

binop                           // List of the binary operators tokens
		: MINUS
		| PLUS
		| TIMES
		| DIVIDE
		| EQUAL
		| NEQUAL
		| GREATER
		| LESS
		;

unop                            // List of the binary operators tokens
		: MINUS
		| NOT
		;

pars                            // Content of argument comma separated in function call parentheses
		: exp COMMA pars
    | exp
		| // Empty
		;

var                            // variable reference (just a name)
		: NAME
    ;
%%
