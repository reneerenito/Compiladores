/* Lexer chiquito para Mini-Haskell.
   - nombre que empieza en minúscula o _  -> var
   - nombre que empieza en mayúscula      -> con
   - número                               -> num
   - if, then, else, let, in              -> palabras reservadas
   - y los símbolos de la gramática (::, ->, ==, etc.) */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gramatica.h"
#include "lexer.h"

static int buscar_terminal(const char *s)
{
    for (int i = 0; i < FIN; i++)
        if (strcmp(TERMINALES[i], s) == 0)
            return i;
    return -1;
}

static int es_reservada(const char *s)
{
    static const char *reservadas[] = {"if", "then", "else", "let", "in"};
    for (int i = 0; i < 5; i++)
        if (strcmp(reservadas[i], s) == 0)
            return 1;
    return 0;
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

int tokenizar(const char *texto, Token **tokens, int *n, char *error, size_t tam)
{
    /* Los de dos caracteres van primero, si no "->" se leería como "-". */
    static const char *simbolos[] = {
        "::", "->", "||", "&&", "==", ";", "[", "]", "=", "\\", "<", "+", "-", "*", "(", ")", ","
    };
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
            while (isalnum((unsigned char)texto[j]) || texto[j] == '_' || texto[j] == '\'')
                j++;
            char palabra[64];
            int largo = j - i < 63 ? j - i : 63;
            memcpy(palabra, texto + i, largo);
            palabra[largo] = '\0';

            int tipo;
            if (es_reservada(palabra))
                tipo = buscar_terminal(palabra);
            else
                tipo = isupper((unsigned char)c) ? 1 : 0;   /* con o var */
            agregar(tokens, n, &cap, tipo, texto + i, j - i, i + 1);
            i = j;
            continue;
        }

        if (isdigit((unsigned char)c)) {
            int j = i;
            while (isdigit((unsigned char)texto[j]))
                j++;
            agregar(tokens, n, &cap, 2, texto + i, j - i, i + 1);
            i = j;
            continue;
        }

        int encontrado = 0;
        for (size_t k = 0; k < sizeof simbolos / sizeof simbolos[0]; k++) {
            size_t largo = strlen(simbolos[k]);
            if (strncmp(texto + i, simbolos[k], largo) == 0) {
                agregar(tokens, n, &cap, buscar_terminal(simbolos[k]), texto + i, (int)largo, i + 1);
                i += (int)largo;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            snprintf(error, tam, "carácter no reconocido '%c' en la columna %d", c, i + 1);
            free(*tokens);
            *tokens = NULL;
            return -1;
        }
    }
    return 0;
}

/* Para cuando la entrada ya viene como tokens: "var = var + num ;" */
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
            snprintf(error, tam, "'%s' (token #%d) no es un terminal de la gramática", palabra, num);
            free(*tokens);
            *tokens = NULL;
            return -1;
        }
        agregar(tokens, n, &cap, tipo, palabra, largo, num);
        i = j;
    }
    return 0;
}
