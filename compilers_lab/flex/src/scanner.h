#ifndef SCANNER_H
#define SCANNER_H

typedef enum ScannerToken {
	TOK_EOF = 0,
	TOK_ERROR = 256,

	/* Estructura del lenguaje */
	TOK_KW_STORY,
	TOK_KW_DEF,

	/* Declaraciones */
	TOK_KW_VAR,
	TOK_KW_CONST,

	/* Tipos de datos */
	TOK_KW_INT,
	TOK_KW_FLOAT,
	TOK_KW_STRING,
	TOK_KW_CHAR,

	/* Control de flujo */
	TOK_KW_IF,
	TOK_KW_ELSE,
	TOK_KW_WHILE,
	TOK_KW_FOR,
	TOK_KW_FOREACH,
	TOK_KW_RETURN,
	TOK_KW_BREAK,
	TOK_KW_CONTINUE,

	/* Funciones nativas */
	TOK_KW_PRINT,

	/* Identificadores y literales */
	TOK_IDENTIFIER,
	TOK_INT_LITERAL,
	TOK_FLOAT_LITERAL,
	TOK_STRING_LITERAL,
	TOK_CHAR_LITERAL,

	/* Operadores de incremento y asignacion */
	TOK_INC,
	TOK_DEC,
	TOK_PLUS_ASSIGN,
	TOK_MINUS_ASSIGN,
	TOK_MUL_ASSIGN,
	TOK_DIV_ASSIGN,
	TOK_MOD_ASSIGN,
	TOK_ASSIGN,

	/* Operadores de comparacion */
	TOK_EQ,
	TOK_NEQ,
	TOK_LT,
	TOK_LE,
	TOK_GT,
	TOK_GE,

	/* Operadores logicos */
	TOK_AND,
	TOK_OR,
	TOK_NOT,

	/* Operadores aritmeticos */
	TOK_PLUS,
	TOK_MINUS,
	TOK_MUL,
	TOK_DIV,
	TOK_MOD,

	/* Operadores a nivel de bits */
	TOK_SHL,
	TOK_SHR,
	TOK_BIT_AND,
	TOK_BIT_OR,

	/* Delimitadores */
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_LBRACE,
	TOK_RBRACE,
	TOK_LBRACKET,
	TOK_RBRACKET,
	TOK_COMMA,
	TOK_SEMICOLON

} ScannerToken;

const char *scanner_token_name(int token);

#endif // SCANNER_H