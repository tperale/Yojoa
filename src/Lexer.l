%{

/*
 * Lexer.l file
 * To generate the lexical analyzer run: "flex Lexer.l"
 */

#include "Expression.h"
#include "Parser.h"

#include <stdio.h>

%}

%option outfile="Lexer.c" header-file="Lexer.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

alpha   [A-Za-z]
digit   [0-9]
alphanum  [A-Za-z0-9]

WS          [ \r\n\t]*

%%
"int"              return INT;
"char"             return CHAR;
"if"               return IF;
"else"             return ELSE;
"return"           return RETURN;
";"                return SEMICOLON;
","                return COMMA;
"write"            return WRITE;
"read"             return READ;
"length"           return LENGTH;
"while"            return WHILE;

"("                return LPAR;
")"                return RPAR;

"{"                return LBRACE;
"}"                return RBRACE;
"["                return LBRACK;
"]"                return RBRACK;
"="                return ASSIGN;

"+"                return PLUS;
"*"                return TIMES;
"/"                return DIVIDE;
"-"                return MINUS;
"=="               return EQUAL;
">"                return GREATER;
"<"                return LESS;
"!="               return NEQUAL;
"!"                return NOT;

"''{alpha}'"       {
                   yylval.charvalue = yytext;
                   return QCHAR;
                   }

{alpha}{alphanum}* {
                   yylval.strvalue = strdup(yytext);
                   return NAME;
                   }

{digit}+           {
                   yylval.value = atoi(yytext);
                   return NUMBER;
                   }

{WS}               { /* Skip blanks. */ }

.                  {  }
%%

int yyerror(const char *msg) {
    fprintf(stderr,"Error:%s\n",msg); return 0;
}
