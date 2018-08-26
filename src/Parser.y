%{
#include <stdio.h>
#include "AST.h"
#include "ASTDeclaration.h"
#include "ASTStatement.h"
#include "ASTExpression.h"
#include "ASTProgram.h"
#include "ArrayList.h"
#include "Symbols.h"

extern int yylex();
extern int yylineno;
void yyerror(const char *str) {
  fprintf(stderr,"[%d] Error: %s\n", yylineno, str);
}

ASTProgram* program_struct;
SymbolList* symbols;
%}

%error-verbose
%locations
%output  "Parser.c"
%defines "Parser.h"

/**
 * Value type specification
 */
%union {
  ASTProgram* prog;
  ASTDeclaration* decl;
  ASTDeclarationFunction* fun_decl;
  ASTDeclarationVariable* var_decl;
  ASTBlock* block;
  ASTStatement* statement;
  ASTExpression* expression;
  ASTIdentifier* identifier;
  ArrayList* array;
  ASTType type_decl;
  Operator_t operator;
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
%type <decl> declaration
%type <fun_decl> fun_declaration
%type <var_decl> var_declaration var_identifier
%type <statement> statement
%type <array> formal_pars var_declarations statements pars declarations
%type <block> block
%type <expression> exp
%type <identifier> lexp var
%type <type_decl> type
%type <operator> binop unop

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
		: declarations                { program_struct = ASTProgram_create($1); }
		;

declarations
    : declaration                 { $$ = ArrayList_create(sizeof(ASTDeclaration*)); $$->add($$, (void*) $1); }
    | declarations declaration    { $1->add($1, $2); }
    ;

declaration
    : fun_declaration             { $$ = (ASTDeclaration*) $1; }
    | var_declaration             { $$ = (ASTDeclaration*) $1; }
    ;

fun_declaration                   //
	  : var_identifier LPAR formal_pars RPAR block { $$ = ASTDeclarationFunction_create($1, $3, $5, (ASTInfo) {yylineno, ASTFUNCTION}); }
		;

formal_pars                            // formal_pars is the declaration of arguments in parentheses
		:                                  { $$ = ArrayList_create(sizeof(ASTDeclarationVariable*)); } // or no declaration
    | var_identifier                   { $$ = ArrayList_create(sizeof(ASTDeclarationVariable*)); $$->add($$, (void*) $1); } // a simple declaration
    | formal_pars COMMA var_identifier { $1->add($$, (void*) $3); } // Can be either multiple declaration separated by commas
		;

block                                           // The content of a function, if, while. Variable declarations are always done on the top of the block (eg. { int foo; })
		: LBRACE var_declarations statements RBRACE { $$ = ASTBlock_create($2, $3, (ASTInfo) {yylineno, ASTBLOCK}); }
		;

var_declarations                       // How to do a variable declaration
    :                                  { $$ = ArrayList_create(sizeof(ASTDeclarationVariable*)); }
    | var_declarations var_declaration { $1->add($$, (void*) $2); }
		;

var_declaration                   // How to do a variable declaration
    : var_identifier SEMICOLON { $$ = $1; } // ex: int foo;
		;

var_identifier                    // A simple way to identify combination of variable type and name
    : type var { $$ = ASTDeclarationVariable_create($1, $2, (ASTInfo) {yylineno, ASTVARIABLE_DECLARATION}); }
    ;

type                              // Their are only two primitive data types (char, int) and the composed data types
		: INT  { $$ = (ASTType) INT; }
		| CHAR { $$ = (ASTType) CHAR; }
		/* | type LBRACK exp RBRACK // array type (eg: int[4]) */
		;

statements                        // Statements express how multiple statement need to be combined
		:                                { $$ = ArrayList_create(sizeof(ASTStatement*)); } // or no statement
    | statement                      { $$ = ArrayList_create(sizeof(ASTStatement*)); $$->add($$, (void*) $1); }
	  | statements statement           { $1->add($$, (void*) $2); } // Can be either multiple statement semicolon separated
		;

statement                         // Statement express possible actions you can do on the programming language
	  : IF LPAR exp RPAR statement                 { $$ = (ASTStatement*) ASTStatementCondition_create($3, $5, NULL, (ASTInfo) {yylineno, ASTCONDITION}); }
		| IF LPAR exp RPAR statement ELSE statement  { $$ = (ASTStatement*) ASTStatementCondition_create($3, $5, $7, (ASTInfo) {yylineno, ASTCONDITION}); }
		| WHILE LPAR exp RPAR statement              { $$ = (ASTStatement*) ASTStatementLoop_create($3, $5, (ASTInfo) {yylineno, ASTLOOP}); }
		| lexp ASSIGN exp SEMICOLON                  { $$ = (ASTStatement*) ASTStatementAssignment_create($1, $3, (ASTInfo) {yylineno, ASTASSIGNMENT}); } // assignment
		| RETURN exp SEMICOLON                       { $$ = (ASTStatement*) ASTStatementReturn_create($2, (ASTInfo) {yylineno, ASTRETURN}); } // return statement
		| block                                      { $$ = (ASTStatement*) $1; }
		| WRITE exp                                  { $$ = (ASTStatement*) ASTStatementWrite_create($2, (ASTInfo) {yylineno, ASTWRITE}); }
		| READ lexp                                  { $$ = (ASTStatement*) ASTStatementRead_create($2, (ASTInfo) {yylineno, ASTREAD}); }
		;

exp
		: lexp                { $$ = (ASTExpression*) $1; }
		| var LPAR pars RPAR	{ $$ = (ASTExpression*) ASTFunctionCall_create($1, $3, (ASTInfo) {yylineno, ASTFUNCTIONCALL}); }   // function call
		| exp binop exp       { $$ = (ASTExpression*) ASTOperator_create($1, $3, $2, (ASTInfo) {yylineno, ASTOPERATOR}); }   // (eg: foo == bar)
		| unop exp            { $$ = (ASTExpression*) ASTOperator_create($2, NULL, $1, (ASTInfo) {yylineno, ASTOPERATOR}); } // (eg: !foo)
		| LPAR exp RPAR       { $$ = $2; }                                                // (eg: (foo))
		| NUMBER              { $$ = (ASTExpression*) ASTInteger_create($1, (ASTInfo) {yylineno, ASTINTEGER}); }            // (eg: 2)
		| QCHAR               { $$ = (ASTExpression*) ASTChar_create($1, (ASTInfo) {yylineno, ASTCHAR}); }               // Just a charachter (eg: 'c')
		// TODO | LENGTH lexp	        { }                                                 // size of an array (eg: length foo)
		;

lexp                              // left expression are either a variable name or variable name array access
		: var     { $$ = $1; } // (eg: foo)
		// TODO | lexp LBRACK exp RBRACK	// ex: foo[2]
		;

binop                           // List of the binary operators tokens
		: MINUS   { $$ = (Operator_t) bMINUS; }
		| PLUS    { $$ = (Operator_t) bPLUS; }
		| TIMES   { $$ = (Operator_t) bTIMES; }
		| DIVIDE  { $$ = (Operator_t) bDIVIDE; }
		| EQUAL   { $$ = (Operator_t) bEQUAL; }
		| NEQUAL  { $$ = (Operator_t) bNEQUAL; }
		| GREATER { $$ = (Operator_t) bGREATER; }
		| LESS    { $$ = (Operator_t) bLESS; }
		;

unop                            // List of the binary operators tokens
		: MINUS   { $$ = (Operator_t) uMINUS; }
		| NOT     { $$ = (Operator_t) uNOT; }
		;

pars                            // Content of argument comma separated in function call parentheses
		: exp COMMA pars { $$ = ArrayList_create(sizeof(ASTExpression*)); $$->add($$, (void*) $1); $$->add_list($$, $3); }
    | exp            { $$ = ArrayList_create(sizeof(ASTExpression*)); $$->add($$, (void*) $1); }
		|                { $$ = ArrayList_create(sizeof(ASTExpression*)); }                                                 // Empty
		;

var                            // variable reference (just a name)
		: NAME { $$ = ASTIdentifier_create((char*) $1, (ASTInfo) {yylineno, ASTCHAR}); }
    ;
%%
