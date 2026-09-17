#include "alphabet.h"
#include <stdlib.h>
 
char *nfa_alphabet(const nfa *n, int *out_size) {
  /* En el peor caso todas las transiciones usan un símbolo distinto, así
     que num_transitions es una cota exacta para el arreglo. */
  char *alphabet = malloc(n->num_transitions * sizeof(char));
  int count = 0;
 
  for (int i = 0; i < n->num_transitions; i++) {
    char symbol = n->transitions[i].symbol;
 
    if (symbol == 0)
      continue;
 
    int already_in_alphabet = 0;
    for (int j = 0; j < count; j++) {
      if (alphabet[j] == symbol) {
        already_in_alphabet = 1;
        break;
      }
    }
 
    if (!already_in_alphabet)
      alphabet[count++] = symbol;
  }
 
  *out_size = count;
  return alphabet;
}
