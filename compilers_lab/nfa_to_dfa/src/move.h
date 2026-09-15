/* Algoritmo Move. Recibe el NFA, un
   conjunto de estados T y un símbolo, y regresa el conjunto de estados
   alcanzables desde T consumiendo exactamente ese símbolo. */

#ifndef MOVE_H
#define MOVE_H

#include "nfa.h"
#include "set.h"

state_set move(const nfa *n, const state_set *t, char symbol);

#endif
