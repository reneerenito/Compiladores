#include <stdio.h>

#include "scanner.h"

extern int yylex(void);
extern char *yytext;

int main(void)
{
	int token;

	while ((token = yylex()) != TOK_EOF) {
		printf("[%s:%s]\n", scanner_token_name(token), yytext);
	}

	return 0;
}

const char *scanner_token_name(int token)
{
	switch (token) {
		case TOK_EOF: return "EOF";
		case TOK_ERROR: return "ERROR";

		/* Estructura del lenguaje */
		case TOK_KW_STORY: return "KW_STORY";
		case TOK_KW_DEF: return "KW_DEF";

		/* Declaraciones */
		case TOK_KW_VAR: return "KW_VAR";
		case TOK_KW_CONST: return "KW_CONST";

		/* Tipos de datos */
		case TOK_KW_INT: return "KW_INT";
		case TOK_KW_FLOAT: return "KW_FLOAT";
		case TOK_KW_STRING: return "KW_STRING";
		case TOK_KW_CHAR: return "KW_CHAR";

		/* Control de flujo */
		case TOK_KW_IF: return "KW_IF";
		case TOK_KW_ELSE: return "KW_ELSE";
		case TOK_KW_WHILE: return "KW_WHILE";
		case TOK_KW_FOR: return "KW_FOR";
		case TOK_KW_FOREACH: return "KW_FOREACH";
		case TOK_KW_RETURN: return "KW_RETURN";
		case TOK_KW_BREAK: return "KW_BREAK";
		case TOK_KW_CONTINUE: return "KW_CONTINUE";

		/* Funciones nativas */
		case TOK_KW_PRINT: return "KW_PRINT";

		/* Identificadores y literales */
		case TOK_IDENTIFIER: return "IDENTIFIER";
		case TOK_INT_LITERAL: return "INT_LITERAL";
		case TOK_FLOAT_LITERAL: return "FLOAT_LITERAL";
		case TOK_STRING_LITERAL: return "STRING_LITERAL";
		case TOK_CHAR_LITERAL: return "CHAR_LITERAL";

		/* Operadores de incremento y asignacion */
		case TOK_INC: return "INC";
		case TOK_DEC: return "DEC";
		case TOK_PLUS_ASSIGN: return "PLUS_ASSIGN";
		case TOK_MINUS_ASSIGN: return "MINUS_ASSIGN";
		case TOK_MUL_ASSIGN: return "MUL_ASSIGN";
		case TOK_DIV_ASSIGN: return "DIV_ASSIGN";
		case TOK_MOD_ASSIGN: return "MOD_ASSIGN";
		case TOK_ASSIGN: return "ASSIGN";

		/* Operadores de comparacion */
		case TOK_EQ: return "EQ";
		case TOK_NEQ: return "NEQ";
		case TOK_LT: return "LT";
		case TOK_LE: return "LE";
		case TOK_GT: return "GT";
		case TOK_GE: return "GE";

		/* Operadores logicos */
		case TOK_AND: return "AND";
		case TOK_OR: return "OR";
		case TOK_NOT: return "NOT";

		/* Operadores aritmeticos */
		case TOK_PLUS: return "PLUS";
		case TOK_MINUS: return "MINUS";
		case TOK_MUL: return "MUL";
		case TOK_DIV: return "DIV";
		case TOK_MOD: return "MOD";

		/* Operadores a nivel de bits */
		case TOK_SHL: return "SHL";
		case TOK_SHR: return "SHR";
		case TOK_BIT_AND: return "BIT_AND";
		case TOK_BIT_OR: return "BIT_OR";

		/* Delimitadores */
		case TOK_LPAREN: return "LPAREN";
		case TOK_RPAREN: return "RPAREN";
		case TOK_LBRACE: return "LBRACE";
		case TOK_RBRACE: return "RBRACE";
		case TOK_LBRACKET: return "LBRACKET";
		case TOK_RBRACKET: return "RBRACKET";
		case TOK_COMMA: return "COMMA";
		case TOK_SEMICOLON: return "SEMICOLON";

		default: return "UNKNOWN";
	}
}