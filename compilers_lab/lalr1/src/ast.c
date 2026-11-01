/* Construcción del AST.
 *
 * Cada vez que el parser reduce con la regla k, llama a reducir_ast(k, hijos)
 * con lo que había en la pila para esa regla, y lo que regrese se vuelve el
 * valor del no terminal. Así el árbol se arma de abajo hacia arriba.
 *
 * El AST se queda solo con tres tipos de nodo: asignación, desreferencia e
 * id. Se quita el = y el *, y se saltan las reglas de un solo hijo
 * (S -> R, R -> L, L -> id).
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

static Nodo *con_hijos(Nodo *padre, Nodo *a, Nodo *b)
{
    if (a) agregar_hijo(padre, a);
    if (b) agregar_hijo(padre, b);
    return padre;
}

/* ---------------- AST ---------------- */

static Nodo *hoja_ast(const Token *t)
{
    if (t->tipo == T_ID)
        return nodo_nuevo("id %s", t->lexema);
    return nodo_nuevo("%s", t->lexema);
}

static Nodo *reducir_ast(int k, Nodo **h)
{
    switch (k) {
    case 2:                                 /* S -> L = R */
        return con_hijos(nodo_nuevo("asignación"), h[0], h[2]);
    case 4:                                 /* L -> * R */
        return con_hijos(nodo_nuevo("desreferencia"), h[1], NULL);
    default:                                /* S -> R, L -> id, R -> L: pasa igual */
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
