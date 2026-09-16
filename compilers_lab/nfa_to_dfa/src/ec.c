#include "ec.h"
#include <stdlib.h>

state_set e_closure(const nfa *n, const state_set *t) {
  state_set closure = set_create(n->num_states);
  int *stack = malloc(n->num_states * sizeof(int));
  int top = 0;

  // La clausura empieza con T y la pila se inicializa con sus elementos
  for (int state = 0; state < n->num_states; state++) {
    if (set_contains(t, state)) {
      set_add(&closure, state);
      stack[top++] = state;
    }
  }

  while (top > 0) {
    int state = stack[--top];

    for (int i = 0; i < n->num_transitions; i++) {
      nfa_transition transition = n->transitions[i];

      if (transition.from == state && transition.symbol == 0 &&
          !set_contains(&closure, transition.to)) {
        set_add(&closure, transition.to);
        stack[top++] = transition.to;
      }
    }
  }

  free(stack);
  return closure;
}
