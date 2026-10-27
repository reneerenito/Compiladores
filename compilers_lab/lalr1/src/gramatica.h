/* La gramática de asignación con apuntadores: terminales, no terminales
   y producciones. */
#ifndef GRAMATICA_H
#define GRAMATICA_H

#define NUM_TERMINALES 4 /* =, *, id y $ */
#define NUM_NO_TERMINALES 3 /* S, L, R (S' no necesita columna) */
#define NUM_PRODUCCIONES 7 /* la 0 es la de la gramática aumentada */
#define NUM_ESTADOS 10 /* los 14 estados LR(1) quedan en 10 al fusionar */
#define FIN 3 /* posición de $ en TERMINALES */

/* Los terminales en el mismo orden que las columnas de ACCION. */
enum { T_IGUAL, T_ASTERISCO, T_ID };

/* Los no terminales en el mismo orden que las columnas de IR_A.
   Llevan el prefijo NT_ para no chocar con las macros S(n) y R(k) de tabla.h. */
enum { NT_S, NT_L, NT_R };

typedef struct {
    int izq; /* no terminal de la izquierda */
    int largo; /* cuántos símbolos hay a la derecha */
    const char *texto; /* la regla escrita, para imprimirla */
} Produccion;

extern const char *TERMINALES[NUM_TERMINALES];
extern const char *NO_TERMINALES[NUM_NO_TERMINALES];
extern const Produccion PRODUCCIONES[NUM_PRODUCCIONES];

#endif