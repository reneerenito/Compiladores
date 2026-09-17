/* Obtiene el alfabeto Sigma de un NFA: los símbolos distintos de épsilon
   (0) que aparecen en sus transiciones. Es una propiedad del NFA en sí,
   no del algoritmo que lo consume, por eso vive en su propio módulo. */
 
#ifndef ALPHABET_H
#define ALPHABET_H
 
#include "nfa.h"
 
char *nfa_alphabet(const nfa *n, int *out_size);
 
#endif
