%{
#include <stdio.h>
#include "AST.h"
#include "ASTDeclaration.h"
#include "ASTStatement.h"
#include "ASTExpression.h"
#include "ASTProgram.h"
#include "ArrayList.h"

extern int yylex();
void yyerror(const char *s) { printf("ERROR: %sn", s); }

ASTProgram* program_struct;
%}

%output  "Parser.c"
%defines "Parser.h"

/**
 * Value type specification
 */
%union {
  ASTProgram* prog;
  ASTDeclarationFunction* fun_decl;
  ASTDeclarationVariable* var_decl;
  ASTBlock* block;
  ASTStatement* statement;
  ASTExpression* expression;
  ASTIdentifier* identifier;
  ArrayList* array;
  ASTType type_decl;
  int token;
  char* string;
  char qchar;
  int value;
}

/**
 *
 */
%token <string> NAME              // A String starting with a letter followed by 0 or more letters, digits or undercores.
%token <value> NUMBER         // Is a string of digits
%token <qchar> QCHAR            // Is a character between single quotes

%token <token> INT CHAR                   // Type tokens
%token <token> IF ELSE RETURN SEMICOLON COMMA WRITE READ LENGTH WHILE

%token <token> LPAR RPAR
%token <token> LBRACE RBRACE
%token <token> LBRACK RBRACK
%token <token> ASSIGN

/**
 * Type of the nonterminal symbol specification
 */
%type <prog> program
%type <fun_decl> fun_declaration
%type <var_decl> var_declaration var_identifier
%type <statement> statement
%type <array> fun_declarations formal_pars var_declarations statements pars
%type <block> block
%type <token> binop unop
%type <expression> lexp exp
%type <identifier> var
%type <type_decl> type

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
		: var_declarations fun_declarations { program_struct = ASTProgram_create($1, $2); }
		;

fun_declarations
    : fun_declaration fun_declarations { $$ = ArrayList_create(sizeof(ASTDeclarationFunction*)); $$->add($$, (void*) $1); $$->add_list($$, $2); }
    |                                  { $$ = ArrayList_create(sizeof(ASTDeclarationFunction*)); }
    ;

fun_declaration                   //
	  : var_identifier LPAR formal_pars RPAR block { $$ = ASTDeclarationFunction_create($1, $3, $5); }
		;

formal_pars                            // formal_pars is the declaration of arguments in parentheses
    : var_identifier COMMA formal_pars { $$ = ArrayList_create(sizeof(ASTDeclarationVariable*)); $$->add($$, (void*) $1); $$->add_list($$, $3); } // Can be either multiple declaration separated by commas
    | var_identifier                   { $$ = ArrayList_create(sizeof(ASTDeclarationVariable*)); $$->add($$, (void*) $1); } // a simple declaration
		|                                  { $$ = ArrayList_create(sizeof(ASTDeclarationVariable*)); } // or no declaration
		;

block                                           // The content of a function, if, while. Variable declarations are always done on the top of the block (eg. { int foo; })
		: LBRACE var_declarations statements RBRACE { $$ = ASTBlock_create($2, $3); }
		;

var_declarations                       // How to do a variable declaration
    : var_declaration var_declarations { $$ = ArrayList_create(sizeof(ASTDeclarationVariable*)); $$->add($$, (void*) $1); $$->add_list($$, $2); }
    |                                  { $$ = ArrayList_create(sizeof(ASTDeclarationVariable*)); }
		;

var_declaration                   // How to do a variable declaration
    : var_identifier SEMICOLON { $$ = $1; } // ex: int foo;
		;

var_identifier                    // A simple way to identify combination of variable type and name
    : type var { $$ = ASTDeclarationVariable_create($1, $2); }
    ;

type                              // Their are only two primitive data types (char, int) and the composed data types
		: INT  { $$ = (ASTType) INT; }
		| CHAR { $$ = (ASTType) CHAR; }
		/* | type LBRACK exp RBRACK // array type (eg: int[4]) */
		;

statements                        // Statements express how multiple statement need to be combined
	  : statement SEMICOLON statements { $$ = ArrayList_create(sizeof(ASTStatement*)); $$->add($$, (void*) $1); $$->add_list($$, $3); } // Can be either multiple statement semicolon separated
    | statement                      { $$ = ArrayList_create(sizeof(ASTStatement*)); $$->add($$, (void*) $1); }
		|                                { $$ = ArrayList_create(sizeof(ASTStatement*)); } // or no statement
		;

statement                         // Statement express possible actions you can do on the programming language
	  : IF LPAR exp RPAR statement
		| IF LPAR exp RPAR statement ELSE statement
		| WHILE LPAR exp RPAR statement
		| lexp ASSIGN exp // assignment
		| RETURN exp // return statement
		| NAME LPAR pars RPAR	// function call // TODO free "NAME"
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
		| NAME LPAR pars RPAR	// function call ex: foo(bar, 2) // TODO free "NAME"
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
		: NAME { $$ = ASTIdentifier_create($1); free($1); }
    ;
%%
