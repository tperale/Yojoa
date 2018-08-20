%{
#include <stdio.h>
#include <string.h>
#include "AST.h"
#include "ASTDeclaration.h"
#include "ASTStatement.h"
#include "ASTExpression.h"
#include "ASTProgram.h"
#include "Parser.h"
#define TOKEN(x) (yylval.token = x)
%}

%option outfile="Lexer.c" header-file="Lexer.h"
%option noyywrap nounput noinput

%%

[ \r\n\t]          ;
"int"              return TOKEN(INT);
"char"             return TOKEN(CHAR);
"if"               return TOKEN(IF);
"else"             return TOKEN(ELSE);
"return"           return TOKEN(RETURN);
";"                return TOKEN(SEMICOLON);
","                return TOKEN(COMMA);
"write"            return TOKEN(WRITE);
"read"             return TOKEN(READ);
"length"           return TOKEN(LENGTH);
"while"            return TOKEN(WHILE);

"("                return TOKEN(LPAR);
")"                return TOKEN(RPAR);

"{"                return TOKEN(LBRACE);
"}"                return TOKEN(RBRACE);
"["                return TOKEN(LBRACK);
"]"                return TOKEN(RBRACK);
"="                return TOKEN(ASSIGN);

"+"                return TOKEN(PLUS);
"*"                return TOKEN(TIMES);
"/"                return TOKEN(DIVIDE);
"-"                return TOKEN(MINUS);
"=="               return TOKEN(EQUAL);
">"                return TOKEN(GREATER);
"<"                return TOKEN(LESS);
"!="               return TOKEN(NEQUAL);
"!"                return TOKEN(NOT);

"'[a-zA-Z_]'"          yylval.qchar = yytext[0]; return QCHAR;
[a-zA-Z_][a-zA-Z0-9_]* yylval.string = strdup(yytext); return NAME;
[0-9]+                 yylval.value = atoi(yytext); return NUMBER;
.                      fprintf(stderr, "Can't parse this token\n"); yyterminate();
%%
