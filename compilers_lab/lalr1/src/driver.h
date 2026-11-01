#ifndef DRIVER_H
#define DRIVER_H

#include <stddef.h>

#include "ast.h"
#include "lexer.h"

/* Corre el ciclo shift/reduce con ACCION e IR_A. Regresa 0 si la cadena es
   válida (y deja el árbol en *ast) o -1 con el mensaje en error.
   Si traza es distinto de 0 imprime cada paso. */
int analizar(const Token *tokens, int n, int traza, Nodo **ast, char *error, size_t tam);

#endif
