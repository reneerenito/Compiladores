#include "gramatica.h"

const char *TERMINALES[NUM_TERMINALES] = { "=", "*", "id", "$" };
const char *NO_TERMINALES[NUM_NO_TERMINALES] = { "S", "L", "R" };

const Produccion PRODUCCIONES[NUM_PRODUCCIONES] = {
    {-1,   0, "(sin usar)"},
    {-1,   1, "S' -> S"},
    {NT_S, 3, "S -> L = R"},
    {NT_S, 1, "S -> R"},
    {NT_L, 2, "L -> * R"},
    {NT_L, 1, "L -> id"},
    {NT_R, 1, "R -> L"},
};