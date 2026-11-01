/*
 * Tabla LALR(1) de la gramática de asignación con apuntadores, escrita a mano
 * a partir del autómata canónico LR(1) (estados I0 a I13).
 *
 *   1) S' -> S     2) S -> L = R     3) S -> R
 *   4) L -> * R    5) L -> id        6) R -> L
 *
 * Cómo leer las celdas de ACCION:
 *   S(n)  leer el token y pasar al estado n (shift)
 *   R(k)  reducir con la producción k (los números son los de gramatica.c)
 *   ACC   aceptar
 *   ER    error, no hay nada que hacer con ese token
 *
 * El autómata LR(1) tiene 14 estados. Cuatro parejas tienen el mismo núcleo
 * y solo cambian en los lookaheads, así que se fusionan y quedan 10:
 *
 *   I4  + I11  ->  I4   [L -> * . R]   lookaheads {=, $} U {$}  = {=, $}
 *   I5  + I12  ->  I5   [L -> id .]    lookaheads {=, $} U {$}  = {=, $}
 *   I7  + I13  ->  I7   [L -> * R .]   lookaheads {=, $} U {$}  = {=, $}
 *   I8  + I10  ->  I8   [R -> L .]     lookaheads {=, $} U {$}  = {=, $}
 *
 * Las transiciones que iban a I10, I11, I12 o I13 ahora van a I8, I4, I5
 * e I7. La fusión no crea conflictos reducción/reducción: cada estado
 * fusionado tiene un solo ítem completo, entonces solo hay una reducción
 * posible por celda.
 *
 * Los estados con comentario son los fusionados y el I2, que es el que
 * chocaba en SLR(1).
 */

#include "tabla.h"

const short ACCION[NUM_ESTADOS][NUM_TERMINALES] = {
    /*            =      *     id      $  */
    /* I0  */ {   ER,  S(4),  S(5),    ER},
    /* I1  */ {   ER,    ER,    ER,   ACC},
    /* I2: [S -> L . = R, $] y [R -> L ., $]. En SLR(1) se reducía R -> L con
       todo Follow(R) = {=, $} y eso pisaba el shift de =. En LR(1) el
       lookahead de R -> L . aquí es solo $, así que con = seguimos leyendo
       y con $ reducimos. La fusión no toca este estado, y el choque no regresa. */
    /* I2  */ { S(6),    ER,    ER,  R(6)},
    /* I3  */ {   ER,    ER,    ER,  R(3)},
    /* I4 (I4 + I11): [L -> * . R] con lookaheads {=, $}. Solo hay shifts,
       así que la unión de lookaheads no cambia ninguna celda. */
    /* I4  */ {   ER,  S(4),  S(5),    ER},
    /* I5 (I5 + I12): [L -> id .]. I5 reducía con = y $, I12 solo con $. */
    /* I5  */ { R(5),    ER,    ER,  R(5)},
    /* I6: en LR(1) iba a I11 con * e I12 con id; ahora son I4 e I5. */
    /* I6  */ {   ER,  S(4),  S(5),    ER},
    /* I7 (I7 + I13): [L -> * R .]. I7 reducía con = y $, I13 solo con $. */
    /* I7  */ { R(4),    ER,    ER,  R(4)},
    /* I8 (I8 + I10): [R -> L .]. I8 reducía con = y $, I10 solo con $. */
    /* I8  */ { R(6),    ER,    ER,  R(6)},
    /* I9  */ {   ER,    ER,    ER,  R(2)},
};

const short IR_A[NUM_ESTADOS][NUM_NO_TERMINALES] = {
    /*            S      L      R  */
    /* I0  */ {     1,     2,     3},
    /* I1  */ {     0,     0,     0},
    /* I2  */ {     0,     0,     0},
    /* I3  */ {     0,     0,     0},
    /* I4  */ {     0,     8,     7},
    /* I5  */ {     0,     0,     0},
    /* I6: en LR(1) iba a I10 con L e I9 con R; I10 ahora es parte de I8. */
    /* I6  */ {     0,     8,     9},
    /* I7  */ {     0,     0,     0},
    /* I8  */ {     0,     0,     0},
    /* I9  */ {     0,     0,     0},
};
