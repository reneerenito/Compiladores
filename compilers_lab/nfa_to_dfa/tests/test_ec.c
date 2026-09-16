/* Pruebas unitarias del algoritmo Epsilon-Closure. El vector principal es el
   sugerido en la práctica: un NFA con transiciones epsilon de 0 hacia 1,
   de 1 hacia 2 y un ciclo de 2 hacia 1. La clausura epsilon del conjunto
   que contiene únicamente al estado 0 debe regresar exactamente {0, 1, 2}
   sin caer en un ciclo infinito. */

#include "ec.h"
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
  printf("Pruebas del algoritmo Epsilon-Closure\n\n");

  /* NFA del vector de prueba sugerido en la práctica:
       0 --epsilon--> 1
       1 --epsilon--> 2
       2 --epsilon--> 1
     El estado 3 no es alcanzable mediante epsilon desde 0. */
  nfa_transition transiciones[] = {
      {0, 0, 1},
      {1, 0, 2},
      {2, 0, 1},
  };

  nfa n = {4, 0, 3, transiciones, 3};

  {
    state_set t = set_create(n.num_states);
    set_add(&t, 0);

    state_set r = e_closure(&n, &t);

    int esperados[] = {0, 1, 2};

    check(es_exactamente(&r, esperados, 3),
          "Epsilon-Closure de {0} regresa {0, 1, 2} y resuelve el ciclo");

    set_free(&r);
    set_free(&t);
  }

  {
    /* Un estado sin transiciones epsilon debe permanecer en la clausura */
    state_set t = set_create(n.num_states);
    set_add(&t, 3);

    state_set r = e_closure(&n, &t);

    int esperados[] = {3};

    check(es_exactamente(&r, esperados, 1),
          "un estado sin transiciones epsilon permanece en la clausura");

    set_free(&r);
    set_free(&t);
  }

  {
    /* La clausura de un conjunto vacío también debe ser vacía */
    state_set t = set_create(n.num_states);

    state_set r = e_closure(&n, &t);

    check(set_is_empty(&r),
          "el conjunto vacio produce una clausura vacia");

    set_free(&r);
    set_free(&t);
  }

  {
    /* Las transiciones con símbolos reales no forman parte de la clausura */
    nfa_transition con_simbolo[] = {
        {0, 0, 1},
        {1, 'a', 2},
        {1, 0, 3},
    };

    nfa n2 = {4, 0, 3, con_simbolo, 3};

    state_set t = set_create(n2.num_states);
    set_add(&t, 0);

    state_set r = e_closure(&n2, &t);

    int esperados[] = {0, 1, 3};

    check(es_exactamente(&r, esperados, 3),
          "las transiciones con simbolos reales se ignoran");

    set_free(&r);
    set_free(&t);
  }

  {
    /* La clausura también debe funcionar con varios estados iniciales */
    state_set t = set_create(n.num_states);
    set_add(&t, 0);
    set_add(&t, 3);

    state_set r = e_closure(&n, &t);

    int esperados[] = {0, 1, 2, 3};

    check(es_exactamente(&r, esperados, 4),
          "la clausura funciona con varios estados iniciales");

    set_free(&r);
    set_free(&t);
  }

  printf("\n%d pruebas, %d fallidas\n", total, fallidas);

  return fallidas > 0;
}