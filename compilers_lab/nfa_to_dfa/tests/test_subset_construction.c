/* Pruebas unitarias para subset_construction.
 *
 * No usan ningún framework externo: cada caso arma un nfa a mano, llama a
 * subset_construction y verifica el DFA resultante con un pequeño arnés de
 * asserts propio (check(...)). Compilar junto con el resto del proyecto:
 *
 *   gcc -Wall -Wextra -std=c11 -o test_subset_construction \
 *       test_subset_construction.c subset_construction.c alphabet.c \
 *       ec.c move.c set.c queue.c
 *   ./test_subset_construction
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfa.h"
#include "dfa.h"
#include "alphabet.h"
#include "subset_construction.h"

static int tests_run = 0;
static int tests_failed = 0;

/* Reporta un fallo con contexto (línea) pero no aborta la ejecucion, para
   que un solo assert roto no oculte el resto de fallas dentro de la
   misma prueba. */
#define check(condition, message)                                          \
  do {                                                                     \
    if (!(condition)) {                                                    \
      printf("    [FALLO] %s (línea %d)\n", message, __LINE__);            \
      tests_failed++;                                                      \
    }                                                                      \
  } while (0)

#define run_test(fn)                                                       \
  do {                                                                      \
    printf("-- %s\n", #fn);                                                \
    tests_run++;                                                           \
    fn();                                                                  \
  } while (0)

/* Busca si delta_D(from, symbol) == to entre las transiciones del DFA. */
static int has_transition(const dfa *d, int from, char symbol, int to) {
  for (int i = 0; i < d->num_transitions; i++) {
    dfa_transition t = d->transitions[i];
    if (t.from == from && t.symbol == symbol && t.to == to)
      return 1;
  }
  return 0;
}

/* Igual que has_transition, pero sin importar el destino: solo pregunta
   si delta_D(from, symbol) está definida. */
static int has_transition_from(const dfa *d, int from, char symbol) {
  for (int i = 0; i < d->num_transitions; i++) {
    dfa_transition t = d->transitions[i];
    if (t.from == from && t.symbol == symbol)
      return 1;
  }
  return 0;
}

/* Sigue una cadena desde el estado inicial del DFA. Regresa el estado
   final alcanzado, o -1 si algún símbolo no tiene transición definida. */
static int run_dfa(const dfa *d, const char *input) {
  int current = d->start;
  for (const char *c = input; *c != '\0'; c++) {
    int next = -1;
    for (int i = 0; i < d->num_transitions; i++) {
      dfa_transition t = d->transitions[i];
      if (t.from == current && t.symbol == *c) {
        next = t.to;
        break;
      }
    }
    if (next == -1)
      return -1;
    current = next;
  }
  return current;
}

static int accepts(const dfa *d, const char *input) {
  int final_state = run_dfa(d, input);
  if (final_state == -1)
    return 0;
  return set_contains(&d->accepting_states, final_state);
}

/* 
   Visualización sencilla en consola: imprime la tabla de transiciones
   de un NFA o un DFA, para poder comparar "antes y después" del
   algoritmo. No forma parte de las pruebas en sí (no usa check()),
   solo es un apoyo visual que se muestra al correr el ejecutable.
   */

/* Llena buf con los destinos de (state, symbol) en el NFA, separados
   por comas (p. ej. "3,8"), o "-" si no hay ninguno. */
static void nfa_targets(char *buf, size_t buf_size, const nfa *n, int state,
                         char symbol) {
  buf[0] = '\0';
  int first = 1;
  for (int t = 0; t < n->num_transitions; t++) {
    if (n->transitions[t].from == state && n->transitions[t].symbol == symbol) {
      char tmp[8];
      snprintf(tmp, sizeof(tmp), first ? "%d" : ",%d", n->transitions[t].to);
      strncat(buf, tmp, buf_size - strlen(buf) - 1);
      first = 0;
    }
  }
  if (buf[0] == '\0')
    strncat(buf, "-", buf_size - 1);
}

static void print_nfa(const nfa *n) {
  int alphabet_size;
  char *alphabet = nfa_alphabet(n, &alphabet_size);

  printf("=== NFA (%d estados) ===\n", n->num_states);
  printf("-> inicio: %d      aceptacion: %d\n\n", n->start, n->accept);

  printf("%-8s", "estado");
  for (int i = 0; i < alphabet_size; i++)
    printf("%-8c", alphabet[i]);
  printf("%-8s\n", "eps");

  for (int s = 0; s < n->num_states; s++) {
    printf("%s%-6d", s == n->start ? "->" : "  ", s);

    char buf[64];
    for (int i = 0; i < alphabet_size; i++) {
      nfa_targets(buf, sizeof(buf), n, s, alphabet[i]);
      printf("%-8s", buf);
    }
    nfa_targets(buf, sizeof(buf), n, s, 0);
    printf("%-8s%s\n", buf, s == n->accept ? "  <- aceptacion" : "");
  }
  printf("\n");

  free(alphabet);
}

static void print_dfa(const dfa *d) {
  printf("=== DFA (%d estados) ===\n", d->num_states);
  printf("-> inicio: %d\n\n", d->start);

  printf("%-8s", "estado");
  for (int i = 0; i < d->alphabet_size; i++)
    printf("%-8c", d->alphabet[i]);
  printf("\n");

  for (int s = 0; s < d->num_states; s++) {
    printf("%s%-6d", s == d->start ? "->" : "  ", s);

    for (int i = 0; i < d->alphabet_size; i++) {
      char symbol = d->alphabet[i];
      int found = 0;
      for (int t = 0; t < d->num_transitions; t++) {
        if (d->transitions[t].from == s && d->transitions[t].symbol == symbol) {
          printf("%-8d", d->transitions[t].to);
          found = 1;
          break;
        }
      }
      if (!found)
        printf("%-8s", "-");
    }
    printf("%s\n", set_contains(&d->accepting_states, s) ? "  <- aceptacion" : "");
  }
  printf("\n");
}

/* Arma el NFA clásico de Thompson para (a|b)*abb y muestra primero su
   tabla de transiciones y luego la del DFA equivalente, para ver de un
   vistazo lo que hace subset_construction. */
static void demo_visualizacion(void) {
  nfa_transition trans[] = {
      {0, 0, 1}, {0, 0, 7},
      {1, 0, 2}, {1, 0, 4},
      {2, 'a', 3},
      {3, 0, 6},
      {4, 'b', 5},
      {5, 0, 6},
      {6, 0, 1}, {6, 0, 7},
      {7, 'a', 8},
      {8, 'b', 9},
      {9, 'b', 10},
  };
  nfa n = {11, 0, 10, trans, 13};

  printf("Expresion regular: (a|b)*abb\n\n");
  print_nfa(&n);

  dfa d = subset_construction(&n);
  print_dfa(&d);
}

/* Caso 1: NFA sin epsilon para a*b
   0 --a--> 0
   0 --b--> 1 (aceptación)
   El DFA equivalente debe tener exactamente 2 estados. */
static void test_sin_epsilon_a_star_b(void) {
  nfa_transition trans[] = {
      {0, 'a', 0},
      {0, 'b', 1},
  };
  nfa n = {2, 0, 1, trans, 2};

  dfa d = subset_construction(&n);

  check(d.num_states == 2, "a*b debe generar un DFA de 2 estados");
  check(d.alphabet_size == 2, "el alfabeto debe ser {a, b}");
  check(has_transition(&d, d.start, 'a', d.start),
        "leer 'a' en el estado inicial debe quedarse en el mismo estado");
  check(!set_contains(&d.accepting_states, d.start),
        "el estado inicial de a*b no debe ser de aceptación");
  check(accepts(&d, "b"), "'b' debe ser aceptada");
  check(accepts(&d, "aaab"), "'aaab' debe ser aceptada");
  check(!accepts(&d, "a"), "'a' sola no debe ser aceptada");
  check(!accepts(&d, "ba"), "'ba' no debe ser aceptada");
  check(!accepts(&d, ""), "la cadena vacía no debe ser aceptada");
}

/*  Caso 2: NFA de Thompson con epsilon para a|b
   0 --eps--> 1        1 --a--> 2
   0 --eps--> 3        3 --b--> 4
   2 --eps--> 5 (aceptación)
   4 --eps--> 5
   El e-closure del estado inicial debe fusionar las dos ramas en un solo
   estado del DFA, dando como resultado 3 estados en total. */
static void test_con_epsilon_a_or_b(void) {
  nfa_transition trans[] = {
      {0, 0, 1},
      {0, 0, 3},
      {1, 'a', 2},
      {3, 'b', 4},
      {2, 0, 5},
      {4, 0, 5},
  };
  nfa n = {6, 0, 5, trans, 6};

  dfa d = subset_construction(&n);

  check(d.num_states == 3, "a|b debe colapsar a un DFA de 3 estados");
  check(accepts(&d, "a"), "'a' debe ser aceptada");
  check(accepts(&d, "b"), "'b' debe ser aceptada");
  check(!accepts(&d, "ab"), "'ab' no debe ser aceptada");
  check(!accepts(&d, ""), "la cadena vacía no debe ser aceptada");
  check(!set_contains(&d.accepting_states, d.start),
        "el estado inicial de a|b no debe ser de aceptación");
}

/* Caso 3: NFA invalido (num_states <= 0) 
   Debe regresar el dfa "error" ({0} en todos sus campos) sin tronar. */
static void test_nfa_invalido(void) {
  nfa n = {0, 0, 0, NULL, 0};

  dfa d = subset_construction(&n);

  check(d.num_states == 0, "un NFA sin estados debe regresar num_states == 0");
  check(d.states == NULL, "no debe reservarse memoria para states");
  check(d.transitions == NULL, "no debe reservarse memoria para transitions");
  check(d.alphabet == NULL, "no debe reservarse memoria para alphabet");
}

/* Caso 4: NFA sin transiciones (alfabeto vacio)
   Un solo estado que es a la vez inicial y de aceptacion: solo acepta la
   cadena vacia y el alfabeto debe quedar vacío. */
static void test_alfabeto_vacio(void) {
  nfa n = {1, 0, 0, NULL, 0};

  dfa d = subset_construction(&n);

  check(d.num_states == 1, "sin transiciones, el DFA debe tener 1 estado");
  check(d.alphabet_size == 0, "el alfabeto debe quedar vacío");
  check(d.num_transitions == 0, "no debe haber transiciones");
  check(accepts(&d, ""), "la cadena vacía debe ser aceptada");
  check(set_contains(&d.accepting_states, d.start),
        "el único estado es inicial y de aceptación a la vez");
}

/* Caso 5: simbolos repetidos en el NFA
   Verifica que build_alphabet (vía nfa_alphabet) no duplique simbolos
   aunque aparezcan varias veces en las transiciones. */
static void test_alfabeto_sin_duplicados(void) {
  nfa_transition trans[] = {
      {0, 'a', 1},
      {1, 'a', 0},
      {0, 'a', 0},
      {0, 'b', 1},
  };
  nfa n = {2, 0, 1, trans, 4};

  dfa d = subset_construction(&n);

  check(d.alphabet_size == 2, "el alfabeto no debe duplicar símbolos repetidos");

  int seen_a = 0, seen_b = 0;
  for (int i = 0; i < d.alphabet_size; i++) {
    if (d.alphabet[i] == 'a') seen_a++;
    if (d.alphabet[i] == 'b') seen_b++;
  }
  check(seen_a == 1, "'a' debe aparecer una sola vez en el alfabeto");
  check(seen_b == 1, "'b' debe aparecer una sola vez en el alfabeto");
}

/* Caso 6: no deben crearse transiciones hacia el conjunto vacío
   Move+e-closure vacío (ningún símbolo definido desde el estado destino)
   no debe generar una transición ahí, según la línea "if V != vacio" del
   pseudocódigo: delta_D(estado_tras_'a', 'a') no debe existir. */
static void test_no_genera_transiciones_a_vacio(void) {
  nfa_transition trans[] = {
      {0, 'a', 1},
  };
  nfa n = {2, 0, 1, trans, 1};

  dfa d = subset_construction(&n);

  check(d.num_states == 2, "debe haber exactamente 2 estados alcanzables");

  int state_after_a = run_dfa(&d, "a");
  check(state_after_a != -1, "'a' debe llevar a un estado válido");
  check(!has_transition_from(&d, state_after_a, 'a'),
        "el estado 1 del NFA no tiene salida, no debe generarse transición");
}


/* Caso 7: un estado del NFA inalcanzable desde el inicio 
   El estado 2 no tiene ninguna transición de entrada: nunca puede
   formar parte de ningún e-closure alcanzado desde s0, así que no debe
   aparecer en ningún subconjunto del DFA aunque exista en el NFA (que
   sí lo cuenta en num_states). Esto comprueba que la construcción
   explora subconjuntos alcanzables y no enumera todo 2^num_states. */
static void test_estado_inalcanzable_no_aparece(void) {
  nfa_transition trans[] = {
      {0, 'a', 1},
      {2, 'b', 2}, /* estado 2: inalcanzable, con auto-transicion */
  };
  nfa n = {3, 0, 1, trans, 2};

  dfa d = subset_construction(&n);

  check(d.num_states == 2,
        "solo deben explorarse los subconjuntos alcanzables desde s0");
  for (int i = 0; i < d.num_states; i++)
    check(!set_contains(&d.states[i], 2),
          "el estado inalcanzable del NFA no debe aparecer en ningún "
          "subconjunto del DFA");
}

/* Caso 8: cadenas de longitud exacta 5 sobre {a, b} 
   NFA en cadena (ya determinista de fondo, sin fusiones posibles): cada
   estado i tiene una transicion con 'a' y otra con 'b' hacia i+1. Solo
   reconoce cadenas de longitud exactamente 5. Se prueban las 32
   combinaciones posibles de longitud 5 más varias de otras longitudes. */
static void test_longitud_exacta_cinco(void) {
  nfa_transition trans[10];
  int t = 0;
  for (int i = 0; i < 5; i++) {
    trans[t++] = (nfa_transition){i, 'a', i + 1};
    trans[t++] = (nfa_transition){i, 'b', i + 1};
  }
  nfa n = {6, 0, 5, trans, t};

  dfa d = subset_construction(&n);

  check(d.num_states == 6,
        "sin fusión de subconjuntos, debe haber un estado del DFA por "
        "cada estado del NFA");

  char buf[6];
  for (long mask = 0; mask < (1L << 5); mask++) {
    for (int i = 0; i < 5; i++)
      buf[i] = (mask & (1L << i)) ? 'b' : 'a';
    buf[5] = '\0';
    check(accepts(&d, buf), "toda cadena de longitud exacta 5 debe ser aceptada");
  }

  check(!accepts(&d, "aaa"), "una cadena más corta que 5 no debe ser aceptada");
  check(!accepts(&d, "aaaaaa"), "una cadena más larga que 5 no debe ser aceptada");
  check(!accepts(&d, ""), "la cadena vacía no debe ser aceptada");
}

/* Referencia independiente del DFA: ¿la cadena termina en "abb"? Se usa 
   para comparar contra lo que produce subset_construction,
   sin depender de como haya numerado sus estados internamente. */
static int ends_with_abb(const char *s, int len) {
  if (len < 3)
    return 0;
  return s[len - 3] == 'a' && s[len - 2] == 'b' && s[len - 1] == 'b';
}

/* Caso 9: (a|b)*abb, verificado de forma exhaustiva.
   Se generan todas las cadenas sobre {a, b} de longitud 0 a 10
   (2^11 - 1 = 2047 cadenas en total) y se compara la aceptacion del DFA
   contra la referencia independiente ends_with_abb. Tambien se fija el
   tamaño exacto del DFA resultante. . */
static void test_clasico_a_or_b_star_abb(void) {
  nfa_transition trans[] = {
      {0, 0, 1}, {0, 0, 7},
      {1, 0, 2}, {1, 0, 4},
      {2, 'a', 3},
      {3, 0, 6},
      {4, 'b', 5},
      {5, 0, 6},
      {6, 0, 1}, {6, 0, 7},
      {7, 'a', 8},
      {8, 'b', 9},
      {9, 'b', 10},
  };
  nfa n = {11, 0, 10, trans, 13};

  dfa d = subset_construction(&n);

  check(d.num_states == 5,
        "(a|b)*abb debe converger a los 5 estados alcanzables conocidos");
  check(d.num_transitions == 10,
        "cada uno de los 5 estados debe tener transición para 'a' y 'b'");

  char buf[12];
  const int max_len = 10;
  for (int len = 0; len <= max_len; len++) {
    long total = 1L << len;
    for (long mask = 0; mask < total; mask++) {
      for (int i = 0; i < len; i++)
        buf[i] = (mask & (1L << i)) ? 'b' : 'a';
      buf[len] = '\0';

      int expected = ends_with_abb(buf, len);
      int got = accepts(&d, buf);
      if (expected != got) {
        printf("    [FALLO] '%s': se esperaba %s, se obtuvo %s (línea %d)\n",
               buf, expected ? "aceptar" : "rechazar",
               got ? "aceptar" : "rechazar", __LINE__);
        tests_failed++;
      }
    }
  }
}

int main(void) {
  printf("=========== Ejemplo: NFA -> DFA ===========\n\n");
  demo_visualizacion();
  printf("===================================================\n\n");

  run_test(test_sin_epsilon_a_star_b);
  run_test(test_con_epsilon_a_or_b);
  run_test(test_nfa_invalido);
  run_test(test_alfabeto_vacio);
  run_test(test_alfabeto_sin_duplicados);
  run_test(test_no_genera_transiciones_a_vacio);
  run_test(test_estado_inalcanzable_no_aparece);
  run_test(test_longitud_exacta_cinco);
  run_test(test_clasico_a_or_b_star_abb);

  printf("\n%d pruebas ejecutadas, %d verificaciones fallidas.\n", tests_run,
         tests_failed);

  return tests_failed == 0 ? 0 : 1;
}
