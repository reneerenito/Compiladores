#ifndef DRIVER_H
#define DRIVER_H

#include "ast.h"
#include "lexer.h"

typedef struct {
    int aceptada;
    Nodo *arbol;        /* el árbol armado, si se aceptó */
    char error[512];    /* qué salió mal, si se rechazó */
} Resultado;

/* Corre el parser sobre los tokens. Si `traza` no es 0, imprime la pila en cada paso. */
Resultado analizar(const Token *tokens, int n, Acciones acciones, int traza);

#endif
