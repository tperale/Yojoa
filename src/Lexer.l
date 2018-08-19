%{
#include <stdio.h>
#include <string.h>
#define TOKEN(x) (yyval.token = x)
%}

%option outfile="Lexer.c" header-file="Lexer.h"

alpha   [A-Za-z]
digit   [0-9]
alphanum  [A-Za-z0-9]

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

"'{alpha}'"       {
                   yylval.char = yytext;
                   return TOKEN(QCHAR);
                   }

{alpha}{alphanum}* {
                   yylval.strvalue = strdup(yytext);
                   return TOKEN(NAME);
                   }

{digit}+           {
                   yylval.value = atoi(yytext);
                   return TOKEN(NUMBER);
                   }

.                  yyerror("Can't parse this token"); yyterminate();
%%

int yyerror(const char *msg) {
  fprintf(stderr, "Error:%s\n", msg);
  return 0;
}
