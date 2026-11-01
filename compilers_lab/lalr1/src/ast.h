#ifndef AST_H
#define AST_H

typedef enum { N_ASIGNACION, N_DEREF, N_ID } TipoNodo;

typedef struct Nodo {
    TipoNodo tipo;
    char lexema[64]; /* solo para N_ID */
    struct Nodo *izq; /* N_ASIGNACION: lado izquierdo. N_DEREF: el operando */
    struct Nodo *der; /* N_ASIGNACION: lado derecho */
} Nodo;

Nodo *nodo_id(const char *lexema);
Nodo *nodo_deref(Nodo *operando);
Nodo *nodo_asignacion(Nodo *izq, Nodo *der);

/* Imprime el árbol en texto y libera toda su memoria */
void ast_imprimir(const Nodo *n);
void ast_liberar(Nodo *n);

#endif
