/* El motor del parser SLR(1).
 *
 * Usa tres pilas que suben y bajan juntas:
 *   estados   los números de estado del autómata
 *   simbolos  los nombres de los símbolos (solo para mostrar la traza)
 *   valores   los pedazos de árbol que se van armando
 *
 * En cada paso mira el estado de arriba y el token actual, y busca en ACCION:
 *   shift   mete el token y el nuevo estado, y avanza en la entrada
 *   reduce  saca tantos elementos como mide la regla, mete el no terminal
 *           y el estado que diga IR_A, y arma el nodo del árbol
 *   ACC     la cadena es válida
 *   ER      la cadena tiene un error
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver.h"
#include "gramatica.h"
#include "tabla.h"

static void mostrar_paso(int paso, const int *estados, const char **simbolos,
                         int tope, const Token *tokens, int n, int i,
                         const char *accion) {
  char pila[2048] = "", entrada[2048] = "", pedazo[64];

  snprintf(pila, sizeof pila, "%d", estados[0]);
  for (int k = 1; k <= tope; k++) {
    snprintf(pedazo, sizeof pedazo, " %s %d", simbolos[k], estados[k]);
    strncat(pila, pedazo, sizeof pila - strlen(pila) - 1);
  }
  for (int k = i; k < n; k++) {
    strncat(entrada, TERMINALES[tokens[k].tipo],
            sizeof entrada - strlen(entrada) - 1);
    strncat(entrada, " ", sizeof entrada - strlen(entrada) - 1);
  }
  strncat(entrada, "$", sizeof entrada - strlen(entrada) - 1);

  printf("%4d  %-45s %35s   %s\n", paso, pila, entrada, accion);
}

static void armar_error(Resultado *r, int estado, const Token *tokens, int n,
                        int i) {
  char esperados[256] = "";
  for (int t = 0; t < NUM_TERMINALES; t++) {
    if (ACCION[estado][t] != ER) {
      if (esperados[0] != '\0')
        strcat(esperados, ", ");
      strcat(esperados, TERMINALES[t]);
    }
  }
  if (i >= n)
    snprintf(r->error, sizeof r->error,
             "Error en el fin de la entrada: en el estado I%d se esperaba uno "
             "de: %s",
             estado, esperados);
  else
    snprintf(
        r->error, sizeof r->error,
        "Error en el token #%d '%s': en el estado I%d se esperaba uno de: %s",
        i + 1, tokens[i].lexema, estado, esperados);
}

Resultado analizar(const Token *tokens, int n, Acciones acciones, int traza) {
  Resultado r = {0, NULL, ""};

  /* La pila nunca crece más que el número de tokens + 2,
     porque toda regla tiene al menos un símbolo a la derecha. */
  int *estados = malloc((n + 2) * sizeof(int));
  const char **simbolos = malloc((n + 2) * sizeof(char *));
  Nodo **valores = malloc((n + 2) * sizeof(Nodo *));
  int tope = 0;
  estados[0] = 0;

  int i = 0, paso = 0;
  char accion[160];

  if (traza)
    printf("%4s  %-45s %35s   %s\n", "Paso", "Pila", "Entrada", "Acción");

  for (;;) {
    int estado = estados[tope];
    int token = i < n ? tokens[i].tipo : FIN;
    int celda = ACCION[estado][token];
    paso++;

    if (celda == ER) {
      if (traza)
        mostrar_paso(paso, estados, simbolos, tope, tokens, n, i, "ERROR");
      armar_error(&r, estado, tokens, n, i);
      break;
    }

    if (celda == ACC) {
      if (traza)
        mostrar_paso(paso, estados, simbolos, tope, tokens, n, i, "aceptar");
      r.aceptada = 1;
      r.arbol = valores[tope];
      break;
    }

    if (celda > 0) { /* shift */
      if (traza) {
        snprintf(accion, sizeof accion, "shift %d", celda);
        mostrar_paso(paso, estados, simbolos, tope, tokens, n, i, accion);
      }
      tope++;
      estados[tope] = celda;
      simbolos[tope] = TERMINALES[token];
      valores[tope] = acciones.hoja(&tokens[i]);
      i++;
      continue;
    }

    /* reduce con la regla k */
    int k = -celda;
    const Produccion *p = &PRODUCCIONES[k];
    tope -= p->largo; /* sacamos el lado derecho */
    Nodo *nuevo = acciones.reducir(k, &valores[tope + 1]);
    int siguiente = IR_A[estados[tope]][p->izq];

    if (traza) {
      snprintf(accion, sizeof accion, "reduce r%d (%s), ir_a[%d,%s] = %d", k,
               p->texto, estados[tope], NO_TERMINALES[p->izq], siguiente);
      /* mostramos la pila como estaba antes de reducir */
      mostrar_paso(paso, estados, simbolos, tope + p->largo, tokens, n, i,
                   accion);
    }

    tope++;
    estados[tope] = siguiente;
    simbolos[tope] = NO_TERMINALES[p->izq];
    valores[tope] = nuevo;
  }

  free(estados);
  free(simbolos);
  free(valores);
  return r;
}
