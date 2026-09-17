#include "set.h"

#ifndef DFA_H
#define DFA_H

typedef struct {
  int from;
  char symbol;
  int to;
} dfa_transition;

/* states[i] guarda el subconjunto de estados del NFA (e-closure) que
   representa al estado i del DFA; sirve para detectar cuando un
   subconjunto ya fue descubierto durante la construcción. accepting_states es
   un unico state_set de capacidad num_states donde la posición i indica
   si el estado i del DFA es de aceptacion. */

typedef struct {
  int num_states;
  int start;
  state_set accepting_states;
  state_set *states;
  char *alphabet;
  int alphabet_size;
  dfa_transition *transitions;
  int num_transitions;
} dfa;

#endif
