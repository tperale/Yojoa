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
%type <expression> exp
%type <identifier> lexp var
%type <type_decl> type binop unop

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
	  : IF LPAR exp RPAR statement                 { $$ = (ASTStatement*) ASTStatementCondition_create($3, $5, NULL); }
		| IF LPAR exp RPAR statement ELSE statement  { $$ = (ASTStatement*) ASTStatementCondition_create($3, $5, $7); }
		| WHILE LPAR exp RPAR statement              { $$ = (ASTStatement*) ASTStatementLoop_create($3, $5); }
		| lexp ASSIGN exp                            { $$ = (ASTStatement*) ASTStatementAssignment_create($1, $3); } // assignment
		| RETURN exp                                 { $$ = (ASTStatement*) ASTStatementReturn_create($2); } // return statement
		| block                                      { $$ = (ASTStatement*) $1; }
		// TODO | WRITE exp
		// TODO | READ lexp
		;

exp
		: lexp                { $$ = (ASTExpression*) $1; }
		| var LPAR pars RPAR	{ $$ = (ASTExpression*) ASTFunctionCall_create($1, $3); }   // function call
		| exp binop exp       { $$ = (ASTExpression*) ASTOperator_create($1, $3, $2); }   // (eg: foo == bar)
		| unop exp            { $$ = (ASTExpression*) ASTOperator_create($2, NULL, $1); } // (eg: !foo)
		| LPAR exp RPAR       { $$ = $2; }                                                // (eg: (foo))
		| NUMBER              { $$ = (ASTExpression*) ASTInteger_create($1); }            // (eg: 2)
		| QCHAR               { $$ = (ASTExpression*) ASTChar_create($1); }               // Just a charachter (eg: 'c')
		// TODO | LENGTH lexp	        { }                                                 // size of an array (eg: length foo)
		;

lexp                              // left expression are either a variable name or variable name array access
		: var     { $$ = $1; } // (eg: foo) // TODO symbol verification
		// TODO | lexp LBRACK exp RBRACK	// ex: foo[2]
		;

binop                           // List of the binary operators tokens
		: MINUS   { $$ = (ASTType) MINUS; }
		| PLUS    { $$ = (ASTType) PLUS; }
		| TIMES   { $$ = (ASTType) TIMES; }
		| DIVIDE  { $$ = (ASTType) DIVIDE; }
		| EQUAL   { $$ = (ASTType) EQUAL; }
		| NEQUAL  { $$ = (ASTType) NEQUAL; }
		| GREATER { $$ = (ASTType) GREATER; }
		| LESS    { $$ = (ASTType) LESS; }
		;

unop                            // List of the binary operators tokens
		: MINUS   { $$ = (ASTType) MINUS; }
		| NOT     { $$ = (ASTType) NOT; }
		;

pars                            // Content of argument comma separated in function call parentheses
		: exp COMMA pars { $$ = ArrayList_create(sizeof(ASTExpression*)); $$->add($$, (void*) $1); $$->add_list($$, $3); }
    | exp            { $$ = ArrayList_create(sizeof(ASTExpression*)); $$->add($$, (void*) $1); }
		|                { $$ = ArrayList_create(sizeof(ASTExpression*)); }                                                 // Empty
		;

var                            // variable reference (just a name)
		: NAME { $$ = ASTIdentifier_create($1); free($1); }
    ;
%%
