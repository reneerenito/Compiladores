/* Motor LALR(1): una pila de estados y otra, en paralelo, con el nodo del AST
   de cada símbolo (NULL para los terminales que no generan nodo). */

#include <stdio.h>
#include <stdlib.h>

#include "driver.h"
#include "gramatica.h"
#include "tabla.h"

/* Hace la acción semántica de la producción k y regresa el nodo resultante.
   nodos apunta al primer símbolo de la parte derecha. */
static Nodo *reducir_nodo(int k, Nodo **nodos)
{
    switch (k) {
    case 2: return nodo_asignacion(nodos[0], nodos[2]); /* S -> L = R */
    case 4: return nodo_deref(nodos[1]);                /* L -> * R   */
    default: return nodos[0];                           /* S -> R, L -> id (nodo ya creado), R -> L */
    }
}

int analizar(const Token *tokens, int n, int traza, Nodo **ast, char *error, size_t tam)
{
    int *estados = malloc((n + 2) * sizeof(int));
    Nodo **nodos = malloc((n + 2) * sizeof(Nodo *));
    int tope = 0, i = 0, resultado = -1;
    estados[0] = 0;
    nodos[0] = NULL;
    *ast = NULL;

    for (;;) {
        int estado = estados[tope];
        int tipo = i < n ? tokens[i].tipo : FIN;
        int accion = ACCION[estado][tipo];

        if (traza)
            printf("estado %d, token %s -> ", estado, TERMINALES[tipo]);

        if (accion == ER) {
            if (i < n)
                snprintf(error, tam, "error sintáctico en el token #%d '%s' (estado %d)", i + 1, tokens[i].lexema, estado);
            else
                snprintf(error, tam, "error sintáctico: la cadena terminó antes de tiempo (estado %d)", estado);
            if (traza)
                printf("error\n");
            break;
        }

        if (accion == ACC) {
            if (traza)
                printf("aceptar\n");
            *ast = nodos[tope];
            resultado = 0;
            break;
        }

        if (accion > 0) { /* shift */
            if (traza)
                printf("shift %d\n", accion);
            tope++;
            estados[tope] = accion;
            nodos[tope] = tipo == T_ID ? nodo_id(tokens[i].lexema) : NULL;
            i++;
        } else { /* reduce */
            int k = -accion;
            int largo = PRODUCCIONES[k].largo;
            if (traza)
                printf("reduce %s\n", PRODUCCIONES[k].texto);
            Nodo *nodo = reducir_nodo(k, &nodos[tope - largo + 1]);
            tope -= largo;
            tope++;
            estados[tope] = IR_A[estados[tope - 1]][PRODUCCIONES[k].izq];
            nodos[tope] = nodo;
        }
    }

    if (resultado != 0)
        for (int j = 1; j <= tope; j++)
            ast_liberar(nodos[j]);
    free(estados);
    free(nodos);
    return resultado;
}
