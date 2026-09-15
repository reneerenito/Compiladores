#include "move.h"

/* Seguimos el pseudocódigo. R empieza vacío y recorremos
   las transiciones para encontrar las que salen de algún estado de T
   usando el símbolo indicado. Si coinciden, añadimos su estado destino.
   Las transiciones epsilon y las de otros símbolos se ignoran aquí,
   ya que la e-closure se encarga de ellas después. Como usamos
   conjuntos, un estado no se añade más de una vez. */

state_set move(const nfa *n, const state_set *t, char symbol) {
  state_set r = set_create(n->num_states);

  /* El 0 representa epsilon y Move solo consume símbolos reales */
  if (symbol == 0)
    return r;

  for (int i = 0; i < n->num_transitions; i++) {
    nfa_transition tr = n->transitions[i];
    if (tr.symbol == symbol && set_contains(t, tr.from))
      set_add(&r, tr.to);
  }

  return r;
}
