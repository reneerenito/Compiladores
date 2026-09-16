/* Algoritmo e-closure. Recibe el NFA y un conjunto de estados T, y regresa
   todos los estados alcanzables desde T usando cero o mas transiciones
   epsilon. */

#ifndef EC_H
#define EC_H

#include "nfa.h"
#include "set.h"

state_set e_closure(const nfa *n, const state_set *t);

#endif
