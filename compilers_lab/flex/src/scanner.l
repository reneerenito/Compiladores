%option noyywrap yylineno

%{
#include "scanner.h"
%}

%x COMMENT

ID          [a-zA-Z_][a-zA-Z0-9_]*
INT         [0-9]+
FLOAT       ({INT}\.[0-9]*([eE][+-]?{INT})?|{INT}[eE][+-]?{INT})
WS          [ \t\r\n]+

%%

"/*"                    { BEGIN(COMMENT); }
<COMMENT>"*/"           { BEGIN(INITIAL); }
<COMMENT>\n             { /* yylineno se actualiza automáticamente */ }
<COMMENT>.              { /* descartar contenido de comentario */ }
<COMMENT><<EOF>>        { BEGIN(INITIAL); return TOK_ERROR; }

"//"[^\n]*              { /* comentario de línea */ }

{WS}                    { /* ignorar espacios */ }

"storia"                 { return TOK_KW_STORY; }
"filma"                 { return TOK_KW_DEF; }

"banana"                { return TOK_KW_VAR; }
"gelato"                { return TOK_KW_CONST; }

"int"                   { return TOK_KW_INT; }
"float"                 { return TOK_KW_FLOAT; }
"string"                { return TOK_KW_STRING; }
"char"                  { return TOK_KW_CHAR; }

"para_tu"               { return TOK_KW_IF; }
"bi_do"                 { return TOK_KW_ELSE; }
"stupa"                 { return TOK_KW_WHILE; }
"hana_dul_sae"          { return TOK_KW_FOR; }
"luk_at_tu"             { return TOK_KW_FOREACH; }
"papoi"                 { return TOK_KW_RETURN; }
"poopaye"               { return TOK_KW_BREAK; }
"pwde_na"               { return TOK_KW_CONTINUE; }

"bello"                 { return TOK_KW_PRINT; }

"++"                    { return TOK_INC; }
"--"                    { return TOK_DEC; }
"+="                    { return TOK_PLUS_ASSIGN; }
"-="                    { return TOK_MINUS_ASSIGN; }
"*="                    { return TOK_MUL_ASSIGN; }
"/="                    { return TOK_DIV_ASSIGN; }
"%="                    { return TOK_MOD_ASSIGN; }
"=="                    { return TOK_EQ; }
"!="                    { return TOK_NEQ; }
"<="                    { return TOK_LE; }
">="                    { return TOK_GE; }

"&&"                    { return TOK_AND; }
"||"                    { return TOK_OR; }

"<<"                    { return TOK_SHL; }
">>"                    { return TOK_SHR; }
"&"                     { return TOK_BIT_AND; }
"|"                     { return TOK_BIT_OR; }

"="                     { return TOK_ASSIGN; }
"<"                     { return TOK_LT; }
">"                     { return TOK_GT; }
"!"                     { return TOK_NOT; }
"+"                     { return TOK_PLUS; }
"-"                     { return TOK_MINUS; }
"*"                     { return TOK_MUL; }
"/"                     { return TOK_DIV; }
"%"                     { return TOK_MOD; }

"("                     { return TOK_LPAREN; }
")"                     { return TOK_RPAREN; }
"{"                     { return TOK_LBRACE; }
"}"                     { return TOK_RBRACE; }
"["                     { return TOK_LBRACKET; }
"]"                     { return TOK_RBRACKET; }
","                     { return TOK_COMMA; }
";"                     { return TOK_SEMICOLON; }

{FLOAT}                 { return TOK_FLOAT_LITERAL; }
{INT}                   { return TOK_INT_LITERAL; }
\'([^'\\\n]|\\.)\'      { return TOK_CHAR_LITERAL; }
\"([^"\\\n]|\\.)*\"     { return TOK_STRING_LITERAL; }
\"([^"\\\n]|\\.)*       { return TOK_ERROR; /* cadena sin cerrar */ }
{ID}                    { return TOK_IDENTIFIER; }

<<EOF>>                 { return TOK_EOF; }

.                       { return TOK_ERROR; }

%%
