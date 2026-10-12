/* Parser SLR(1) de Mini-Haskell.
 *
 *   ./parser "inc x = x + 1 ;"
 *   ./parser -f programa.mhs
 *   ./parser --tokens "var var = var + num ;"
 *   ./parser --traza --derivacion "y = a + b * c ;"
 *
 * Sale con 0 si la cadena es válida y con 1 si no.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "driver.h"
#include "gramatica.h"
#include "lexer.h"

static char *leer_todo(FILE *f)
{
    size_t cap = 1024, n = 0;
    char *texto = malloc(cap);
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (n + 1 == cap)
            texto = realloc(texto, cap *= 2);
        texto[n++] = (char)c;
    }
    texto[n] = '\0';
    return texto;
}

/* Imprime el texto en una sola línea, con un solo espacio entre palabras. */
static void imprimir_en_una_linea(const char *texto)
{
    int espacio = 0, inicio = 1;
    for (; *texto; texto++) {
        if (*texto == ' ' || *texto == '\n' || *texto == '\t' || *texto == '\r') {
            espacio = 1;
        } else {
            if (espacio && !inicio)
                putchar(' ');
            putchar(*texto);
            espacio = 0;
            inicio = 0;
        }
    }
    putchar('\n');
}

static void uso(void)
{
    fprintf(stderr,
            "Uso: ./parser [opciones] \"código\"\n"
            "  -f archivo     lee el código de un archivo\n"
            "  --tokens       la entrada ya son tokens (var, num, ...)\n"
            "  --traza        muestra la pila paso a paso\n"
            "  --derivacion   muestra también el árbol con todas las reglas\n");
}

int main(int argc, char **argv)
{
    int modo_tokens = 0, traza = 0, derivacion = 0;
    const char *archivo = NULL, *codigo = NULL;

    for (int a = 1; a < argc; a++) {
        if (strcmp(argv[a], "--tokens") == 0)
            modo_tokens = 1;
        else if (strcmp(argv[a], "--traza") == 0)
            traza = 1;
        else if (strcmp(argv[a], "--derivacion") == 0)
            derivacion = 1;
        else if (strcmp(argv[a], "-f") == 0 && a + 1 < argc)
            archivo = argv[++a];
        else if (strcmp(argv[a], "-h") == 0 || strcmp(argv[a], "--help") == 0) {
            uso();
            return 2;
        } else
            codigo = argv[a];
    }

    char *texto;
    if (archivo) {
        FILE *f = fopen(archivo, "r");
        if (!f) {
            fprintf(stderr, "No pude abrir %s\n", archivo);
            return 2;
        }
        texto = leer_todo(f);
        fclose(f);
    } else if (codigo) {
        texto = malloc(strlen(codigo) + 1);
        strcpy(texto, codigo);
    } else {
        texto = leer_todo(stdin);
    }

    printf("Entrada:   ");
    imprimir_en_una_linea(texto);

    Token *tokens;
    int n;
    char error[256];
    int ok = modo_tokens ? tokens_desde_nombres(texto, &tokens, &n, error, sizeof error)
                         : tokenizar(texto, &tokens, &n, error, sizeof error);
    free(texto);
    if (ok != 0) {
        printf("Resultado: RECHAZADA\nError léxico: %s\n", error);
        return 1;
    }

    printf("Tokens:   ");
    for (int i = 0; i < n; i++)
        printf(" %s", TERMINALES[tokens[i].tipo]);
    printf(n == 0 ? " (vacía)\n" : "\n");
    if (traza)
        printf("\n");

    Resultado r = analizar(tokens, n, ACCIONES_AST, traza);
    if (traza)
        printf("\n");

    int salida;
    if (r.aceptada) {
        printf("Resultado: VÁLIDA\n\nAST:\n");
        imprimir_arbol(r.arbol);
        if (derivacion) {
            Resultado d = analizar(tokens, n, ACCIONES_DERIVACION, 0);
            printf("\nÁrbol de derivación (las reglas que se usaron al reducir):\n");
            imprimir_arbol(d.arbol);
        }
        salida = 0;
    } else {
        printf("Resultado: RECHAZADA\n%s\n", r.error);
        salida = 1;
    }

    liberar_nodos();
    free(tokens);
    return salida;
}
