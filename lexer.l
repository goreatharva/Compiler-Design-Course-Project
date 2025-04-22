%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
%}

%option noyywrap
%option yylineno

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*

%%

"int"           { return INT; }
"float"         { return FLOAT; }
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"return"        { return RETURN; }
"scanf"         { return SCANF; }

"printf"        { yylval.str = strdup("printf"); return IDENTIFIER; }

{ID}            { yylval.str = strdup(yytext); return IDENTIFIER; }
{DIGIT}+        { yylval.num = atoi(yytext); return INTEGER; }
\"([^\\\"]|\\.)*\" { yylval.str = strdup(yytext); return STRING_LIT; }

"=="            { return EQ; }
"!="            { return NE; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }
"++"            { return INC; }
"--"            { return DEC; }

"{"             { return '{'; }
"}"             { return '}'; }
"("             { return '('; }
")"             { return ')'; }
";"             { return ';'; }
","             { return ','; }
"="             { return '='; }
"<"             { return '<'; }
">"             { return '>'; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }

[ \t\r\n]+      ;

"//".*          ;

"/*"([^*]|\*+[^*/])*"*"+"/"    ;

. {
    fprintf(stderr, "Illegal character: %s at line %d\n", yytext, yylineno);
    exit(1);
}

%%
