%{
#include "Expression.h"
#include "Parser.h"
#include "Lexer.c"

int yyerror(SExpression **expression, yyscan_t scanner, const char *msg) {
    // Add error handling routine as needed
}

%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output  "Parser.c"
%defines "Parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { SExpression **expression }
%parse-param { yyscan_t scanner }

%union {
    int value;
    char charvalue;
    char* strvalue;
    ASTNode* expression;
}

%token <value> NAME              // A String starting with a letter followed by 0 or more letters, digits or undercores.
%token <value> NUMBER         // Is a string of digits
%token <value> QCHAR            // Is a character between single quotes

%token INT CHAR                   // Type tokens
%token IF ELSE RETURN SEMICOLON COMMA WRITE READ LENGTH WHILE

%token LPAR RPAR
%token LBRACE RBRACE
%token LBRACK RBRACK
%token ASSIGN

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
	  : type NAME LPAR formal_pars RPAR block
		;

formal_pars                       // formal_pars is the declaration of arguments in parentheses
    : formal_par COMMA formal_pars // Can be either multiple declaration separated by commas
    | formal_par // a simple declaration
		| // or no declaration
		;

formal_par                        // The argument type declaration in parantheses
  	: type NAME
		;

block                             // The content of a function, if, while. Variable declarations are always done on the top of the block.
		: LBRACE var_declarations statements RBRACE // ex: { int foo; }
		;

var_declarations                   // How to do a variable declaration
    : var_declaration var_declarations
    |
		;

var_declaration                   // How to do a variable declaration
    : type NAME SEMICOLON // ex: int foo;
		;

type                              // Their are only two primitive data types (char, int) and the composed data types
		: INT
		| CHAR
		| type LBRACK exp RBRACK // array type
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
