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

void set_free(state_set *s)
{
    free(s->member);
    s->member = NULL;
    s->capacity = 0;
}
