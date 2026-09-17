#include "set.h"
#include <stdlib.h>
#include <string.h>

state_set set_create(int capacity)
{
    state_set s;
    s.capacity = capacity;
    s.member = calloc(capacity, sizeof(char));
    return s;
}

void set_add(state_set *s, int state)
{
    if (state >= 0 && state < s->capacity)
        s->member[state] = 1;
}

int set_contains(const state_set *s, int state)
{
    if (state < 0 || state >= s->capacity)
        return 0;
    return s->member[state];
}

int set_size(const state_set *s)
{
    int count = 0;
    for (int i = 0; i < s->capacity; i++)
        if (s->member[i])
            count++;
    return count;
}

int set_is_empty(const state_set *s)
{
    return set_size(s) == 0;
}

int set_equals(const state_set *a, const state_set *b)
{
  if (a->capacity != b->capacity)
	return 0;
  
  for (int state = 0; state < a->capacity; state++)
	if (set_contains(a, state) != set_contains(b, state))
	  return 0;
  
  return 1;
}

int set_find(const state_set *sets, int count, const state_set *target)
{
    for (int i = 0; i < count; i++)
        if (set_equals(&sets[i], target))
            return i;
 
    return -1;
}


void set_free(state_set *s)
{
    free(s->member);
    s->member = NULL;
    s->capacity = 0;
}
