/* Construcción del AST.
 *
 * Cada vez que el parser reduce con la regla k, llama a reducir_ast(k, hijos)
 * con lo que había en la pila para esa regla, y lo que regrese se vuelve el
 * valor del no terminal. Así el árbol se arma de abajo hacia arriba.
 *
 * El AST se queda solo con lo importante: quita ; , ( ) [ ], se salta las
 * reglas de un solo hijo (Expr -> Or -> And -> ... -> Atom) y junta las
 * listas (Decls, Params, List) en un solo nodo con varios hijos.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "gramatica.h"

/* Guardamos todos los nodos que se crean para poder borrarlos de un jalón
   al final, sin tener que andar viendo cuáles se usaron y cuáles no. */
static Nodo **todos = NULL;
static int num_todos = 0, cap_todos = 0;

static Nodo *nodo_nuevo(const char *formato, ...)
{
    char texto[256];
    va_list args;
    va_start(args, formato);
    vsnprintf(texto, sizeof texto, formato, args);
    va_end(args);

    Nodo *n = calloc(1, sizeof(Nodo));
    n->etiqueta = malloc(strlen(texto) + 1);
    strcpy(n->etiqueta, texto);

    if (num_todos == cap_todos) {
        cap_todos = cap_todos ? cap_todos * 2 : 64;
        todos = realloc(todos, cap_todos * sizeof(Nodo *));
    }
    todos[num_todos++] = n;
    return n;
}

static void agregar_hijo(Nodo *padre, Nodo *hijo)
{
    if (padre->num_hijos == padre->capacidad) {
        padre->capacidad = padre->capacidad ? padre->capacidad * 2 : 4;
        padre->hijos = realloc(padre->hijos, padre->capacidad * sizeof(Nodo *));
    }
    padre->hijos[padre->num_hijos++] = hijo;
}

static void renombrar(Nodo *n, const char *etiqueta)
{
    free(n->etiqueta);
    n->etiqueta = malloc(strlen(etiqueta) + 1);
    strcpy(n->etiqueta, etiqueta);
}

/* Junta los nombres de una lista de parámetros: "x y z". */
static const char *juntar_nombres(const Nodo *lista)
{
    static char texto[200];
    texto[0] = '\0';
    for (int i = 0; i < lista->num_hijos; i++) {
        if (i > 0)
            strncat(texto, " ", sizeof texto - strlen(texto) - 1);
        strncat(texto, lista->hijos[i]->etiqueta, sizeof texto - strlen(texto) - 1);
    }
    return texto;
}

static Nodo *con_hijos(Nodo *padre, Nodo *a, Nodo *b)
{
    if (a) agregar_hijo(padre, a);
    if (b) agregar_hijo(padre, b);
    return padre;
}

/* ---------------- AST ---------------- */

static Nodo *hoja_ast(const Token *t)
{
    return nodo_nuevo("%s", t->lexema);
}

static Nodo *reducir_ast(int k, Nodo **h)
{
    Nodo *n;
    switch (k) {
    case 1:                                 /* Program -> Decls */
        renombrar(h[0], "Programa");
        return h[0];
    case 2: case 14: case 40:               /* listas: se agrega el nuevo elemento */
        agregar_hijo(h[0], h[k == 40 ? 2 : 1]);
        return h[0];
    case 3:                                 /* Decls -> Decl */
        return con_hijos(nodo_nuevo("declaraciones"), h[0], NULL);
    case 15:                                /* Params -> var */
        return con_hijos(nodo_nuevo("parametros"), h[0], NULL);
    case 41:                                /* List -> Expr */
        return con_hijos(nodo_nuevo("lista"), h[0], NULL);
    case 6:                                 /* Sig -> var :: Type ; */
        return con_hijos(nodo_nuevo("Firma %s ::", h[0]->etiqueta), h[2], NULL);
    case 7:                                 /* Type -> Btype -> Type */
        return con_hijos(nodo_nuevo("->"), h[0], h[2]);
    case 11:                                /* Btype -> [ Type ] */
        return con_hijos(nodo_nuevo("[ ]"), h[1], NULL);
    case 12:                                /* Fun -> var = Expr ; */
        return con_hijos(nodo_nuevo("Función %s", h[0]->etiqueta), h[2], NULL);
    case 13:                                /* Fun -> var Params = Expr ; */
        n = nodo_nuevo("Función %s (%s)", h[0]->etiqueta, juntar_nombres(h[1]));
        return con_hijos(n, h[3], NULL);
    case 16:                                /* lambda */
        return con_hijos(nodo_nuevo("lambda %s", juntar_nombres(h[1])), h[3], NULL);
    case 17:                                /* if-then-else */
        n = nodo_nuevo("if");
        agregar_hijo(n, con_hijos(nodo_nuevo("condición"), h[1], NULL));
        agregar_hijo(n, con_hijos(nodo_nuevo("then"), h[3], NULL));
        agregar_hijo(n, con_hijos(nodo_nuevo("else"), h[5], NULL));
        return n;
    case 18:                                /* let Decls in Expr */
        n = con_hijos(nodo_nuevo("let"), h[1], NULL);
        agregar_hijo(n, con_hijos(nodo_nuevo("in"), h[3], NULL));
        return n;
    case 20: case 22: case 24: case 25:     /* operadores binarios: el operador */
    case 27: case 28: case 30:              /* queda arriba y los operandos abajo */
        return con_hijos(nodo_nuevo("%s", h[1]->etiqueta), h[0], h[2]);
    case 32:                                /* App -> App Atom, o sea  f x */
        return con_hijos(nodo_nuevo("aplicación"), h[0], h[1]);
    case 37:                                /* ( Expr ): los paréntesis sobran */
        return h[1];
    case 38:                                /* [ ] */
        return nodo_nuevo("[]");
    case 39:                                /* [ List ] */
        return h[1];
    default:                                /* reglas de un solo hijo: pasa igual */
        return h[0];
    }
}

/* ---------------- Árbol de derivación ---------------- */

static Nodo *hoja_derivacion(const Token *t)
{
    return nodo_nuevo("%s '%s'", TERMINALES[t->tipo], t->lexema);
}

static Nodo *reducir_derivacion(int k, Nodo **h)
{
    Nodo *n = nodo_nuevo("%s   [r%d]", PRODUCCIONES[k].texto, k);
    for (int i = 0; i < PRODUCCIONES[k].largo; i++)
        agregar_hijo(n, h[i]);
    return n;
}

const Acciones ACCIONES_AST = {hoja_ast, reducir_ast};
const Acciones ACCIONES_DERIVACION = {hoja_derivacion, reducir_derivacion};

/* ---------------- Imprimir y liberar ---------------- */

static void imprimir_rama(const Nodo *n, char *prefijo, int es_ultimo)
{
    printf("%s%s%s\n", prefijo, es_ultimo ? "`-- " : "|-- ", n->etiqueta);

    size_t largo = strlen(prefijo);
    if (largo + 5 < 1024)   /* por si el árbol es muy profundo */
        strcat(prefijo, es_ultimo ? "    " : "|   ");
    for (int i = 0; i < n->num_hijos; i++)
        imprimir_rama(n->hijos[i], prefijo, i == n->num_hijos - 1);
    prefijo[largo] = '\0';
}

void imprimir_arbol(const Nodo *raiz)
{
    char prefijo[1024] = "";
    printf("%s\n", raiz->etiqueta);
    for (int i = 0; i < raiz->num_hijos; i++)
        imprimir_rama(raiz->hijos[i], prefijo, i == raiz->num_hijos - 1);
}

void liberar_nodos(void)
{
    for (int i = 0; i < num_todos; i++) {
        free(todos[i]->etiqueta);
        free(todos[i]->hijos);
        free(todos[i]);
    }
    free(todos);
    todos = NULL;
    num_todos = cap_todos = 0;
}
