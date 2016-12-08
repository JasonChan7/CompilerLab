%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "small.tab.h"
int line = 1;
void yyerror(char *);
%}
%%
\/\*(.|\n)*?\*\/ ; /* ignore comments */
[ \t]+ ; /* ignore whitespace */
[\n] {
    line++;
}
 /* match the add-op, mul-op, bracket and etc. */
[-()=+%*/;.] {
    return yytext[0];
}
 /* match the comparison-op */
":=" return BC;
">"  return GT;
"<"  return LT;
">=" return GE;
"<=" return LE;
"==" return EQ;
"!=" return NE;
 /* match the reserved word */
"if"        return IF;
"then"      return THEN;
"else"      return ELSE;
"end"       return END;
"repeat"    return REPEAT;
"until"     return UNTIL;
"read"      return READ;
"write"     return WRITE;
"call"      return CALL;
"const"     return CONST;
"begin"     return BEGIN;
"XOR"       return XOR;
"ODD"       return ODD;
"procedure" return PROC;
"while"     return WHILE;
"do"        return DO;
 /* recognize identifier */
[A-Za-z]([A-Za-z]|[0-9])* {
    yylval.ident = (char*)malloc(strlen(yytext)+1);
    strcpy(yylval.ident, yytext);
    return IDENT;
}
 /* recognize integer */
[0-9]+ {
    yylval.integer = atoi(yytext);
    // fprintf(stdout, "%d\n", yylval.integer);
    return INTEGER;
}
 /* unknown character */
. yyerror("unknown character");
%%
void redirectInput(FILE *input) {
    yyin = input;
}
int yywrap(void) {
    return 1;
}