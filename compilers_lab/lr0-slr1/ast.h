#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef struct Nodo {
    char *etiqueta;
    struct Nodo **hijos;
    int num_hijos;
    int capacidad;
} Nodo;

/* Lo que hace el parser cada vez que lee un token (hoja) o reduce una regla. */
typedef struct {
    Nodo *(*hoja)(const Token *t);
    Nodo *(*reducir)(int k, Nodo **hijos);
} Acciones;

extern const Acciones ACCIONES_AST;         /* arma el AST limpio */
extern const Acciones ACCIONES_DERIVACION;  /* arma el árbol con todas las reglas */

void imprimir_arbol(const Nodo *raiz);
void liberar_nodos(void);   /* borra todos los nodos creados hasta ahora */

#endif
