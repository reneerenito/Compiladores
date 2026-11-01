/* AST de la gramática: solo tres tipos de nodo.
   S -> R y R -> L no crean nodo, solo pasan el que ya traen. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

static Nodo *nuevo(TipoNodo tipo, Nodo *izq, Nodo *der)
{
    Nodo *n = calloc(1, sizeof(Nodo));
    n->tipo = tipo;
    n->izq = izq;
    n->der = der;
    return n;
}

Nodo *nodo_id(const char *lexema)
{
    Nodo *n = nuevo(N_ID, NULL, NULL);
    strncpy(n->lexema, lexema, sizeof(n->lexema) - 1);
    return n;
}

Nodo *nodo_deref(Nodo *operando)
{
    return nuevo(N_DEREF, operando, NULL);
}

Nodo *nodo_asignacion(Nodo *izq, Nodo *der)
{
    return nuevo(N_ASIGNACION, izq, der);
}

static void imprimir(const Nodo *n, const char *prefijo, int es_ultimo, int es_raiz)
{
    printf("%s", prefijo);
    if (!es_raiz)
        printf(es_ultimo ? "└── " : "├── ");

    switch (n->tipo) {
    case N_ASIGNACION: printf("Asignacion\n"); break;
    case N_DEREF:      printf("Deref\n"); break;
    case N_ID:         printf("Id(%s)\n", n->lexema); break;
    }

    char nuevo_prefijo[256];
    snprintf(nuevo_prefijo, sizeof(nuevo_prefijo), "%s%s", prefijo,
             es_raiz ? "" : (es_ultimo ? "    " : "│   "));

    if (n->izq)
        imprimir(n->izq, nuevo_prefijo, n->der == NULL, 0);
    if (n->der)
        imprimir(n->der, nuevo_prefijo, 1, 0);
}

void ast_imprimir(const Nodo *n)
{
    if (n)
        imprimir(n, "", 1, 1);
}

void ast_liberar(Nodo *n)
{
    if (!n)
        return;
    ast_liberar(n->izq);
    ast_liberar(n->der);
    free(n);
}
