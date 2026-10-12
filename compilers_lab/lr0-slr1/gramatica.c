#include "gramatica.h"

/* Mismo orden que las columnas de ACCION en tabla.c. */
const char *TERMINALES[NUM_TERMINALES] = {
    "var", "con", "num", "::", ";", "->", "[", "]", "=", "\\", "if", "then",
    "else", "let", "in", "||", "&&", "==", "<", "+", "-", "*", "(", ")", ",", "$"
};

const char *NO_TERMINALES[NUM_NO_TERMINALES] = {
    "Program", "Decls", "Decl", "Sig", "Type", "Btype", "Fun", "Params", "Expr",
    "Or", "And", "Cmp", "Add", "Mul", "App", "Atom", "List"
};

/* Numeradas igual que en la Tabla 1 del reporte. */
const Produccion PRODUCCIONES[NUM_PRODUCCIONES] = {
    {-1,      1, "Program' -> Program"},
    {PROGRAM, 1, "Program -> Decls"},
    {DECLS,   2, "Decls -> Decls Decl"},
    {DECLS,   1, "Decls -> Decl"},
    {DECL,    1, "Decl -> Sig"},
    {DECL,    1, "Decl -> Fun"},
    {SIG,     4, "Sig -> var :: Type ;"},
    {TYPE,    3, "Type -> Btype -> Type"},
    {TYPE,    1, "Type -> Btype"},
    {BTYPE,   1, "Btype -> con"},
    {BTYPE,   1, "Btype -> var"},
    {BTYPE,   3, "Btype -> [ Type ]"},
    {FUN,     4, "Fun -> var = Expr ;"},
    {FUN,     5, "Fun -> var Params = Expr ;"},
    {PARAMS,  2, "Params -> Params var"},
    {PARAMS,  1, "Params -> var"},
    {EXPR,    4, "Expr -> \\ Params -> Expr"},
    {EXPR,    6, "Expr -> if Expr then Expr else Expr"},
    {EXPR,    4, "Expr -> let Decls in Expr"},
    {EXPR,    1, "Expr -> Or"},
    {OR,      3, "Or -> Or || And"},
    {OR,      1, "Or -> And"},
    {AND,     3, "And -> And && Cmp"},
    {AND,     1, "And -> Cmp"},
    {CMP,     3, "Cmp -> Add == Add"},
    {CMP,     3, "Cmp -> Add < Add"},
    {CMP,     1, "Cmp -> Add"},
    {ADD,     3, "Add -> Add + Mul"},
    {ADD,     3, "Add -> Add - Mul"},
    {ADD,     1, "Add -> Mul"},
    {MUL,     3, "Mul -> Mul * App"},
    {MUL,     1, "Mul -> App"},
    {APP,     2, "App -> App Atom"},
    {APP,     1, "App -> Atom"},
    {ATOM,    1, "Atom -> var"},
    {ATOM,    1, "Atom -> con"},
    {ATOM,    1, "Atom -> num"},
    {ATOM,    3, "Atom -> ( Expr )"},
    {ATOM,    2, "Atom -> [ ]"},
    {ATOM,    3, "Atom -> [ List ]"},
    {LIST,    3, "List -> List , Expr"},
    {LIST,    1, "List -> Expr"},
};
