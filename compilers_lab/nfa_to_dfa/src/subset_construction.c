#include "subset_construction.h"
#include "alphabet.h"
#include "ec.h"
#include "move.h"
#include "queue.h"
#include <stdlib.h>

 
dfa subset_construction(const nfa *n){

  if (n->num_states <= 0){
	dfa no_dfa = {0};
	return no_dfa;
  }
  
  int alphabet_size;
  char *alphabet = nfa_alphabet(n, &alphabet_size);

  /* Construimos la campacidad del conjunto de estados Q_D */
  int states_capacity = n->num_states;
  state_set *states = malloc(states_capacity * sizeof(state_set));
  int num_states = 0;

  /* Construimos la capacidad de las transisciones */
  int transitions_capacity = states_capacity * alphabet_size;
  dfa_transition *transitions = malloc(transitions_capacity * sizeof(dfa_transition));
  int num_transitions = 0;

  // s0 <- e_closure({q0})
  state_set initial = set_create(n->num_states);
  set_add(&initial, n->start);
  state_set s0 = e_closure(n, &initial);
  set_free(&initial);

  states[num_states++] = s0; 
  queue q = queue_create();
  queue_enqueue(&q, 0);

    /* while Cola != vacio */
  while (!queue_is_empty(&q)) {
    /* U <- desencolar(Cola) */
    int u = queue_dequeue(&q);
 
    /* for all a en alfabeto do */
    for (int i = 0; i < alphabet_size; i++) {
      char a = alphabet[i];
 
      /* V <- e-Closure(Move(U, a)) */
      state_set moved = move(n, &states[u], a);
      state_set v = e_closure(n, &moved);
      set_free(&moved);
 
      /* if V != vacio then */
      if (set_is_empty(&v)) {
        set_free(&v);
        continue;
      }
 
      /* if V no está en QD then */
      int v_index = set_find(states, num_states, &v);
 
      if (v_index == -1) {
        /* QD <- QD union {V} */
        if (num_states == states_capacity) {
          states_capacity = states_capacity*2;
          states = realloc(states, states_capacity * sizeof(state_set));
        }
        states[num_states] = v;
        v_index = num_states;
        num_states++;
 
        /* encolar(Cola, V) */
        queue_enqueue(&q, v_index);
      } else {
        /* V ya existia como algun estado del DFA; el subconjunto recien
           calculado ya no se necesita. */
        set_free(&v);
      }
 
      /* delta_D(U, a) <- V */
      if (num_transitions == transitions_capacity) {
        transitions_capacity *= 2;
        transitions = realloc(transitions, transitions_capacity * sizeof(dfa_transition));
      }
      transitions[num_transitions].from = u;
      transitions[num_transitions].symbol = a;
      transitions[num_transitions].to = v_index;
      num_transitions++;
    }
  }
 
  queue_free(&q);
 
  /* FD <- { S en QD : S interseca FN } */
  state_set accepting_states = set_create(num_states);
  for (int i = 0; i < num_states; i++) {
    if (set_contains(&states[i], n->accept))
      set_add(&accepting_states, i);
  }
 
  dfa d;
  d.num_states = num_states;
  d.start = 0;
  d.alphabet = alphabet;
  d.alphabet_size = alphabet_size;
  d.accepting_states = accepting_states;
  d.states = states;
  d.transitions = transitions;
  d.num_transitions = num_transitions;
 
  return d;

}
