/* La gramática de Mini-Haskell: terminales, no terminales y producciones. */
#ifndef GRAMATICA_H
#define GRAMATICA_H

#define NUM_TERMINALES     26   /* los 25 de la gramática más $ */
#define NUM_NO_TERMINALES  17
#define NUM_PRODUCCIONES   42   /* la 0 es la de la gramática aumentada */
#define NUM_ESTADOS        77
#define FIN                25   /* posición de $ en TERMINALES */

/* Los no terminales en el mismo orden que las columnas de IR_A. */
enum {
    PROGRAM, DECLS, DECL, SIG, TYPE, BTYPE, FUN, PARAMS, EXPR,
    OR, AND, CMP, ADD, MUL, APP, ATOM, LIST
};

typedef struct {
    int izq;            /* no terminal de la izquierda */
    int largo;          /* cuántos símbolos hay a la derecha */
    const char *texto;  /* la regla escrita, para imprimirla */
} Produccion;

extern const char *TERMINALES[NUM_TERMINALES];
extern const char *NO_TERMINALES[NUM_NO_TERMINALES];
extern const Produccion PRODUCCIONES[NUM_PRODUCCIONES];

#endif
