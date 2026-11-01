/* Uso:  ./lalr1 [-t] [-v] ["texto"]
     -t  la entrada son nombres de tokens ("* id = id") en vez de código
     -v  imprime cada paso del shift/reduce
   Sin texto lee una línea de la entrada estándar. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "driver.h"
#include "lexer.h"

int main(int argc, char **argv)
{
    int por_tokens = 0, traza = 0;
    const char *texto = NULL;
    char linea[1024];

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0)
            por_tokens = 1;
        else if (strcmp(argv[i], "-v") == 0)
            traza = 1;
        else
            texto = argv[i];
    }

    if (!texto) {
        if (!fgets(linea, sizeof(linea), stdin)) {
            fprintf(stderr, "no hay entrada\n");
            return 1;
        }
        texto = linea;
    }

    Token *tokens;
    int n;
    char error[256];

    int r = por_tokens ? tokens_desde_nombres(texto, &tokens, &n, error, sizeof(error))
                       : tokenizar(texto, &tokens, &n, error, sizeof(error));
    if (r < 0) {
        printf("Cadena inválida: %s\n", error);
        return 1;
    }

    Nodo *ast;
    r = analizar(tokens, n, traza, &ast, error, sizeof(error));
    free(tokens);
    if (r < 0) {
        printf("Cadena inválida: %s\n", error);
        return 1;
    }

    printf("Cadena válida\nAST:\n");
    ast_imprimir(ast);
    ast_liberar(ast);
    return 0;
}
