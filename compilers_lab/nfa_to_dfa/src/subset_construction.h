/* Algoritmo de construccion de subconjuntos. Recibe un NFA
   y regresa el DFA equivalente, construido a partir de las e-closures y
   las transiciones Move sobre subconjuntos de estados del NFA. */
 
#ifndef SUBSET_CONSTRUCTION_H
#define SUBSET_CONSTRUCTION_H
 
#include "nfa.h"
#include "dfa.h"
 
dfa subset_construction(const nfa *n);
 
#endif
