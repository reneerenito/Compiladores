/* Representa el NFA que usamos en la práctica. Las transiciones se guardan
 * indicando el estado de origen, el símbolo y el estado de destino. Usamos el
 * símbolo 0 para representar las transiciones epsilon. */

#ifndef NFA_H
#define NFA_H

typedef struct {
  int from;
  char symbol;
  int to;
} nfa_transition;

typedef struct {
  int num_states;
  int start;
  int accept;
  nfa_transition *transitions;
  int num_transitions;
} nfa;

#endif
