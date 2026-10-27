/* Lexer para la gramática
   un nombre (letra o _ seguido de letras, dígitos o _)  -> id
   los símbolos = y *
   Cualquier otra cosa es un error léxico. El $ no se agrega al lexer
   el driver lo usa solo cuando se acaban los tokens. */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gramatica.h"
#include "lexer.h"

static int buscar_terminal(const char *s)
{
    for (int i = 0; i < FIN; i++) /* sin contar $ */
        if (strcmp(TERMINALES[i], s) == 0)
            return i;
    return -1;
}

static void agregar(Token **tokens, int *n, int *cap, int tipo, const char *ini, int largo, int col)
{
    if (*n == *cap) {
        *cap = *cap ? *cap * 2 : 16;
        *tokens = realloc(*tokens, *cap * sizeof(Token));
    }
    Token *t = &(*tokens)[(*n)++];
    t->tipo = tipo;
    if (largo > 63)
        largo = 63;
    memcpy(t->lexema, ini, largo);
    t->lexema[largo] = '\0';
    t->columna = col;
}

/* Código normal: "*p = x" -> * id = id */
int tokenizar(const char *texto, Token **tokens, int *n, char *error, size_t tam)
{
    int cap = 0;
    *tokens = NULL;
    *n = 0;

    for (int i = 0; texto[i] != '\0';) {
        char c = texto[i];

        if (isspace((unsigned char)c)) {
            i++;
            continue;
        }

        if (isalpha((unsigned char)c) || c == '_') {
            int j = i;
            while (isalnum((unsigned char)texto[j]) || texto[j] == '_')
                j++;
            agregar(tokens, n, &cap, T_ID, texto + i, j - i, i + 1);
            i = j;
            continue;
        }

        if (c == '=' || c == '*') {
            agregar(tokens, n, &cap, c == '=' ? T_IGUAL : T_ASTERISCO, texto + i, 1, i + 1);
            i++;
            continue;
        }

        if (isdigit((unsigned char)c))
            snprintf(error, tam, "'%c' en la columna %d: la gramática no tiene números solo identificadores", c, i + 1);
        else
            snprintf(error, tam, "carácter no reconocido '%c' en la columna %d", c, i + 1);
        free(*tokens);
        *tokens = NULL;
        *n = 0;
        return -1;
    }
    return 0;
}

/* Para cuando la entrada ya viene como tokens: "* id = id" */
int tokens_desde_nombres(const char *texto, Token **tokens, int *n, char *error, size_t tam)
{
    int cap = 0, num = 0;
    *tokens = NULL;
    *n = 0;

    for (int i = 0; texto[i] != '\0';) {
        if (isspace((unsigned char)texto[i])) {
            i++;
            continue;
        }
        int j = i;
        while (texto[j] != '\0' && !isspace((unsigned char)texto[j]))
            j++;
        char palabra[64];
        int largo = j - i < 63 ? j - i : 63;
        memcpy(palabra, texto + i, largo);
        palabra[largo] = '\0';
        num++;

        int tipo = buscar_terminal(palabra);
        if (tipo < 0) {
            snprintf(error, tam, "'%s' (token #%d) no es un terminal de la gramática (se esperaba =, * o id)", palabra, num);
            free(*tokens);
            *tokens = NULL;
            *n = 0;
            return -1;
        }
        agregar(tokens, n, &cap, tipo, palabra, largo, num);
        i = j;
    }
    return 0;
}