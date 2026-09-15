/* Conjunto de estados del autómata. Se representa con un arreglo de
   banderas donde la posición indica el estado, así agregar y consultar
   cuestan tiempo constante y la unión no genera duplicados. */

#ifndef SET_H
#define SET_H

typedef struct
{
    char *member;
    int capacity;
} state_set;

state_set set_create(int capacity);
void set_add(state_set *s, int state);
int set_contains(const state_set *s, int state);
int set_size(const state_set *s);
int set_is_empty(const state_set *s);
void set_free(state_set *s);

#endif
