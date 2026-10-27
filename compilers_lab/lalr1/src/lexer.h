#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef struct {
    int tipo; /* posición en TERMINALES */
    char lexema[64];
    int columna;
} Token;

/* Pasan el texto a una lista de tokens. Regresan 0 si todo salió bien o -1 con el mensaje en error */
int tokenizar(const char *texto, Token **tokens, int *n, char *error, size_t tam);
int tokens_desde_nombres(const char *texto, Token **tokens, int *n, char *error, size_t tam);

#endif