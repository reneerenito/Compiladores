/* Pruebas unitarias del algoritmo Move. El vector principal es el que
   sugiere la practica, un estado origen 0 con dos transiciones hacia 1
   y 2 con el símbolo a y una hacia 3 con el símbolo b, donde Move con el
   conjunto que solo contiene al 0 y el símbolo a debe regresar
   exactamente los estados 1 y 2. */

#include "move.h"
#include <stdio.h>

static int total = 0;
static int fallidas = 0;

static void check(int condicion, const char *nombre) {
  total++;
  if (condicion) {
    printf("  ok     %s\n", nombre);
  } else {
    fallidas++;
    printf("  FALLO  %s\n", nombre);
  }
}

/* Compara que el conjunto contenga exactamente los estados esperados */
static int es_exactamente(const state_set *s, const int *estados, int cuantos) {
  if (set_size(s) != cuantos)
    return 0;
  for (int i = 0; i < cuantos; i++)
    if (!set_contains(s, estados[i]))
      return 0;
  return 1;
}

int main(void) {
  printf("Pruebas del algoritmo Move\n\n");

  /* NFA del vector de prueba del enunciado */
  nfa_transition transiciones[] = {
      {0, 'a', 1},
      {0, 'a', 2},
      {0, 'b', 3},
  };
  nfa n = {4, 0, 3, transiciones, 3};

  state_set t = set_create(n.num_states);
  set_add(&t, 0);

  {
    state_set r = move(&n, &t, 'a');
    int esperados[] = {1, 2};
    check(es_exactamente(&r, esperados, 2),
          "Move con el conjunto {0} y el simbolo a regresa {1, 2}");
    set_free(&r);
  }

  {
    state_set r = move(&n, &t, 'b');
    int esperados[] = {3};
    check(es_exactamente(&r, esperados, 1),
          "Move con el conjunto {0} y el simbolo b regresa {3}");
    set_free(&r);
  }

  {
    state_set r = move(&n, &t, 'c');
    check(set_is_empty(&r),
          "un simbolo sin transiciones regresa el conjunto vacio");
    set_free(&r);
  }

  {
    state_set sin_salidas = set_create(n.num_states);
    set_add(&sin_salidas, 1);
    state_set r = move(&n, &sin_salidas, 'a');
    check(set_is_empty(&r),
          "un estado sin transiciones de salida regresa el conjunto vacio");
    set_free(&r);
    set_free(&sin_salidas);
  }

  {
    /* Mismo NFA pero con una transicion epsilon desde el origen. El
       resultado no debe cambiar porque Move solo consume el simbolo */
    nfa_transition con_epsilon[] = {
        {0, 'a', 1},
        {0, 'a', 2},
        {0, 'b', 3},
        {0, 0, 3},
    };
    nfa n2 = {4, 0, 3, con_epsilon, 4};
    state_set r = move(&n2, &t, 'a');
    int esperados[] = {1, 2};
    check(es_exactamente(&r, esperados, 2),
          "las transiciones epsilon se ignoran");
    set_free(&r);
  }

  {
    /* T con varios estados. Se agregan transiciones desde 1 y 2 para
       comprobar que la union junta los destinos de todos los estados */
    nfa_transition varias[] = {
        {0, 'a', 1},
        {0, 'a', 2},
        {1, 'a', 3},
        {2, 'a', 1},
    };
    nfa n3 = {4, 0, 3, varias, 4};
    state_set varios = set_create(n3.num_states);
    set_add(&varios, 0);
    set_add(&varios, 1);
    set_add(&varios, 2);
    state_set r = move(&n3, &varios, 'a');
    int esperados[] = {1, 2, 3};
    check(
        es_exactamente(&r, esperados, 3),
        "la union sobre varios estados junta todos los destinos sin duplicar");
    set_free(&r);
    set_free(&varios);
  }

  {
    state_set vacio = set_create(n.num_states);
    state_set r = move(&n, &vacio, 'a');
    check(set_is_empty(&r), "el conjunto vacio regresa el conjunto vacio");
    set_free(&r);
    set_free(&vacio);
  }

  set_free(&t);

  printf("\n%d pruebas, %d fallidas\n", total, fallidas);
  return fallidas > 0;
}
