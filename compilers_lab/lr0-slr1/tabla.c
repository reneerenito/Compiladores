/*
 * Tabla SLR(1) de Mini-Haskell, escrita a mano a partir del reporte
 * (autómata LR(0) con estados I0 a I76 y conjuntos Follow de la Tabla 3).
 *
 * Cómo leer las celdas de ACCION:
 *   S(n)  leer el token y pasar al estado n (shift)
 *   R(k)  reducir con la producción k (los números son los del reporte)
 *   ACC   aceptar
 *   ER    error, no hay nada que hacer con ese token
 *
 * Los estados con comentario son los 13 que chocaban en LR(0): ahí la
 * reducción se ponía en todas las columnas y pisaba un shift. En SLR(1)
 * solo reducimos con los tokens de Follow, y el choque desaparece.
 */

#include "tabla.h"

const short ACCION[NUM_ESTADOS][NUM_TERMINALES] = {
    /*           var    con    num     ::      ;     ->      [      ]      =      \     if   then   else    let     in     ||     &&     ==      <      +      -      *      (      )      ,      $  */
    /* I0  */ { S(6),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I1  */ {   ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,   ACC},
    /* I2: ¿termina el programa o viene otra declaración? Con var seguimos leyendo;
       solo con $ reducimos Program -> Decls. */
    /* I2  */ { S(6),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(1)},
    /* I3  */ { R(3),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(3),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(3)},
    /* I4  */ { R(4),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(4),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(4)},
    /* I5  */ { R(5),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(5),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(5)},
    /* I6  */ {S(11),    ER,    ER,  S(8),    ER,    ER,    ER,    ER,  S(9),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I7  */ { R(2),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(2),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(2)},
    /* I8  */ {S(15), S(14),    ER,    ER,    ER,    ER, S(16),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I9  */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /*           var    con    num     ::      ;     ->      [      ]      =      \     if   then   else    let     in     ||     &&     ==      <      +      -      *      (      )      ,      $  */
    /* I10 */ {S(34),    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(33),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I11 */ {R(15),    ER,    ER,    ER,    ER, R(15),    ER,    ER, R(15),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I12 */ {   ER,    ER,    ER,    ER, S(35),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I13: ¿tipo simple o flecha? Con -> seguimos leyendo; con ; o ] reducimos Type -> Btype. */
    /* I13 */ {   ER,    ER,    ER,    ER,  R(8), S(36),    ER,  R(8),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I14 */ {   ER,    ER,    ER,    ER,  R(9),  R(9),    ER,  R(9),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I15 */ {   ER,    ER,    ER,    ER, R(10), R(10),    ER, R(10),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I16 */ {S(15), S(14),    ER,    ER,    ER,    ER, S(16),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I17 */ {   ER,    ER,    ER,    ER, S(38),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I18 */ {S(11),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I19 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /*           var    con    num     ::      ;     ->      [      ]      =      \     if   then   else    let     in     ||     &&     ==      <      +      -      *      (      )      ,      $  */
    /* I20 */ { S(6),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I21: con || la expresión sigue; con cualquier otra cosa de Follow(Expr) reducimos Expr -> Or. */
    /* I21 */ {   ER,    ER,    ER,    ER, R(19),    ER,    ER, R(19),    ER,    ER,    ER, R(19), R(19),    ER,    ER, S(42),    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(19), R(19),    ER},
    /* I22: con && la expresión sigue; si no, reducimos Or -> And. */
    /* I22 */ {   ER,    ER,    ER,    ER, R(21),    ER,    ER, R(21),    ER,    ER,    ER, R(21), R(21),    ER,    ER, R(21), S(43),    ER,    ER,    ER,    ER,    ER,    ER, R(21), R(21),    ER},
    /* I23 */ {   ER,    ER,    ER,    ER, R(23),    ER,    ER, R(23),    ER,    ER,    ER, R(23), R(23),    ER,    ER, R(23), R(23),    ER,    ER,    ER,    ER,    ER,    ER, R(23), R(23),    ER},
    /* I24: con ==, <, + o - la expresión sigue; si no, reducimos Cmp -> Add. */
    /* I24 */ {   ER,    ER,    ER,    ER, R(26),    ER,    ER, R(26),    ER,    ER,    ER, R(26), R(26),    ER,    ER, R(26), R(26), S(44), S(45), S(46), S(47),    ER,    ER, R(26), R(26),    ER},
    /* I25: con * sigue la multiplicación; si no, reducimos Add -> Mul. */
    /* I25 */ {   ER,    ER,    ER,    ER, R(29),    ER,    ER, R(29),    ER,    ER,    ER, R(29), R(29),    ER,    ER, R(29), R(29), R(29), R(29), R(29), R(29), S(48),    ER, R(29), R(29),    ER},
    /* I26: si viene otro átomo (var, con, num, ( o [) es una aplicación tipo f x;
       si no, reducimos Mul -> App. */
    /* I26 */ {S(28), S(29), S(30),    ER, R(31),    ER, S(32), R(31),    ER,    ER,    ER, R(31), R(31),    ER,    ER, R(31), R(31), R(31), R(31), R(31), R(31), R(31), S(31), R(31), R(31),    ER},
    /* I27 */ {R(33), R(33), R(33),    ER, R(33),    ER, R(33), R(33),    ER,    ER,    ER, R(33), R(33),    ER,    ER, R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33), R(33),    ER},
    /* I28 */ {R(34), R(34), R(34),    ER, R(34),    ER, R(34), R(34),    ER,    ER,    ER, R(34), R(34),    ER,    ER, R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34), R(34),    ER},
    /* I29 */ {R(35), R(35), R(35),    ER, R(35),    ER, R(35), R(35),    ER,    ER,    ER, R(35), R(35),    ER,    ER, R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35), R(35),    ER},
    /*           var    con    num     ::      ;     ->      [      ]      =      \     if   then   else    let     in     ||     &&     ==      <      +      -      *      (      )      ,      $  */
    /* I30 */ {R(36), R(36), R(36),    ER, R(36),    ER, R(36), R(36),    ER,    ER,    ER, R(36), R(36),    ER,    ER, R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36), R(36),    ER},
    /* I31 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I32 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32), S(51),    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I33 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I34 */ {R(14),    ER,    ER,    ER,    ER, R(14),    ER,    ER, R(14),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I35 */ { R(6),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(6),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,  R(6)},
    /* I36 */ {S(15), S(14),    ER,    ER,    ER,    ER, S(16),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I37 */ {   ER,    ER,    ER,    ER,    ER,    ER,    ER, S(56),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I38 */ {R(12),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(12),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(12)},
    /* I39 */ {S(34),    ER,    ER,    ER,    ER, S(57),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /*           var    con    num     ::      ;     ->      [      ]      =      \     if   then   else    let     in     ||     &&     ==      <      +      -      *      (      )      ,      $  */
    /* I40 */ {   ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(58),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I41 */ { S(6),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(59),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I42 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I43 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I44 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I45 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I46 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I47 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I48 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I49 */ {R(32), R(32), R(32),    ER, R(32),    ER, R(32), R(32),    ER,    ER,    ER, R(32), R(32),    ER,    ER, R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32), R(32),    ER},
    /*           var    con    num     ::      ;     ->      [      ]      =      \     if   then   else    let     in     ||     &&     ==      <      +      -      *      (      )      ,      $  */
    /* I50 */ {   ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(67),    ER,    ER},
    /* I51 */ {R(38), R(38), R(38),    ER, R(38),    ER, R(38), R(38),    ER,    ER,    ER, R(38), R(38),    ER,    ER, R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38), R(38),    ER},
    /* I52 */ {   ER,    ER,    ER,    ER,    ER,    ER,    ER, S(68),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(69),    ER},
    /* I53 */ {   ER,    ER,    ER,    ER,    ER,    ER,    ER, R(41),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(41),    ER},
    /* I54 */ {   ER,    ER,    ER,    ER, S(70),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I55 */ {   ER,    ER,    ER,    ER,  R(7),    ER,    ER,  R(7),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I56 */ {   ER,    ER,    ER,    ER, R(11), R(11),    ER, R(11),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I57 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I58 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I59 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /*           var    con    num     ::      ;     ->      [      ]      =      \     if   then   else    let     in     ||     &&     ==      <      +      -      *      (      )      ,      $  */
    /* I60: con && primero se agrupa el && (pesa más que ||); si no, reducimos Or -> Or || And. */
    /* I60 */ {   ER,    ER,    ER,    ER, R(20),    ER,    ER, R(20),    ER,    ER,    ER, R(20), R(20),    ER,    ER, R(20), S(43),    ER,    ER,    ER,    ER,    ER,    ER, R(20), R(20),    ER},
    /* I61 */ {   ER,    ER,    ER,    ER, R(22),    ER,    ER, R(22),    ER,    ER,    ER, R(22), R(22),    ER,    ER, R(22), R(22),    ER,    ER,    ER,    ER,    ER,    ER, R(22), R(22),    ER},
    /* I62: con + o - la suma queda dentro de la comparación; si no, reducimos Cmp -> Add == Add. */
    /* I62 */ {   ER,    ER,    ER,    ER, R(24),    ER,    ER, R(24),    ER,    ER,    ER, R(24), R(24),    ER,    ER, R(24), R(24),    ER,    ER, S(46), S(47),    ER,    ER, R(24), R(24),    ER},
    /* I63: igual que I62 pero con <. */
    /* I63 */ {   ER,    ER,    ER,    ER, R(25),    ER,    ER, R(25),    ER,    ER,    ER, R(25), R(25),    ER,    ER, R(25), R(25),    ER,    ER, S(46), S(47),    ER,    ER, R(25), R(25),    ER},
    /* I64: con * primero se multiplica (precedencia); si no, reducimos Add -> Add + Mul. */
    /* I64 */ {   ER,    ER,    ER,    ER, R(27),    ER,    ER, R(27),    ER,    ER,    ER, R(27), R(27),    ER,    ER, R(27), R(27), R(27), R(27), R(27), R(27), S(48),    ER, R(27), R(27),    ER},
    /* I65: igual que I64 pero con la resta. */
    /* I65 */ {   ER,    ER,    ER,    ER, R(28),    ER,    ER, R(28),    ER,    ER,    ER, R(28), R(28),    ER,    ER, R(28), R(28), R(28), R(28), R(28), R(28), S(48),    ER, R(28), R(28),    ER},
    /* I66: si viene otro átomo es una aplicación dentro del producto; si no, reducimos Mul -> Mul * App. */
    /* I66 */ {S(28), S(29), S(30),    ER, R(30),    ER, S(32), R(30),    ER,    ER,    ER, R(30), R(30),    ER,    ER, R(30), R(30), R(30), R(30), R(30), R(30), R(30), S(31), R(30), R(30),    ER},
    /* I67 */ {R(37), R(37), R(37),    ER, R(37),    ER, R(37), R(37),    ER,    ER,    ER, R(37), R(37),    ER,    ER, R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37), R(37),    ER},
    /* I68 */ {R(39), R(39), R(39),    ER, R(39),    ER, R(39), R(39),    ER,    ER,    ER, R(39), R(39),    ER,    ER, R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39), R(39),    ER},
    /* I69 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /*           var    con    num     ::      ;     ->      [      ]      =      \     if   then   else    let     in     ||     &&     ==      <      +      -      *      (      )      ,      $  */
    /* I70 */ {R(13),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(13),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(13)},
    /* I71 */ {   ER,    ER,    ER,    ER, R(16),    ER,    ER, R(16),    ER,    ER,    ER, R(16), R(16),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(16), R(16),    ER},
    /* I72 */ {   ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(75),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER},
    /* I73 */ {   ER,    ER,    ER,    ER, R(18),    ER,    ER, R(18),    ER,    ER,    ER, R(18), R(18),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(18), R(18),    ER},
    /* I74 */ {   ER,    ER,    ER,    ER,    ER,    ER,    ER, R(40),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(40),    ER},
    /* I75 */ {S(28), S(29), S(30),    ER,    ER,    ER, S(32),    ER,    ER, S(18), S(19),    ER,    ER, S(20),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, S(31),    ER,    ER,    ER},
    /* I76 */ {   ER,    ER,    ER,    ER, R(17),    ER,    ER, R(17),    ER,    ER,    ER, R(17), R(17),    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER,    ER, R(17), R(17),    ER},
};

/* GOTO: a qué estado se va después de reducir a un no terminal.
   0 significa que no hay transición (al estado 0 nunca se regresa). */
const short IR_A[NUM_ESTADOS][NUM_NO_TERMINALES] = {
    /*         Program   Decls    Decl     Sig    Type   Btype     Fun  Params    Expr      Or     And     Cmp     Add     Mul     App    Atom    List  */
    /* I0  */ {     1,      2,      3,      4,      0,      0,      5,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I1  */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I2  */ {     0,      0,      7,      4,      0,      0,      5,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I3  */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I4  */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I5  */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I6  */ {     0,      0,      0,      0,      0,      0,      0,     10,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I7  */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I8  */ {     0,      0,      0,      0,     12,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I9  */ {     0,      0,      0,      0,      0,      0,      0,      0,     17,     21,     22,     23,     24,     25,     26,     27,      0},
    /*         Program   Decls    Decl     Sig    Type   Btype     Fun  Params    Expr      Or     And     Cmp     Add     Mul     App    Atom    List  */
    /* I10 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I11 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I12 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I13 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I14 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I15 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I16 */ {     0,      0,      0,      0,     37,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I17 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I18 */ {     0,      0,      0,      0,      0,      0,      0,     39,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I19 */ {     0,      0,      0,      0,      0,      0,      0,      0,     40,     21,     22,     23,     24,     25,     26,     27,      0},
    /*         Program   Decls    Decl     Sig    Type   Btype     Fun  Params    Expr      Or     And     Cmp     Add     Mul     App    Atom    List  */
    /* I20 */ {     0,     41,      3,      4,      0,      0,      5,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I21 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I22 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I23 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I24 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I25 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I26 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     49,      0},
    /* I27 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I28 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I29 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /*         Program   Decls    Decl     Sig    Type   Btype     Fun  Params    Expr      Or     And     Cmp     Add     Mul     App    Atom    List  */
    /* I30 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I31 */ {     0,      0,      0,      0,      0,      0,      0,      0,     50,     21,     22,     23,     24,     25,     26,     27,      0},
    /* I32 */ {     0,      0,      0,      0,      0,      0,      0,      0,     53,     21,     22,     23,     24,     25,     26,     27,     52},
    /* I33 */ {     0,      0,      0,      0,      0,      0,      0,      0,     54,     21,     22,     23,     24,     25,     26,     27,      0},
    /* I34 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I35 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I36 */ {     0,      0,      0,      0,     55,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I37 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I38 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I39 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /*         Program   Decls    Decl     Sig    Type   Btype     Fun  Params    Expr      Or     And     Cmp     Add     Mul     App    Atom    List  */
    /* I40 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I41 */ {     0,      0,      7,      4,      0,      0,      5,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I42 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     60,     23,     24,     25,     26,     27,      0},
    /* I43 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     61,     24,     25,     26,     27,      0},
    /* I44 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     62,     25,     26,     27,      0},
    /* I45 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     63,     25,     26,     27,      0},
    /* I46 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     64,     26,     27,      0},
    /* I47 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     65,     26,     27,      0},
    /* I48 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     66,     27,      0},
    /* I49 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /*         Program   Decls    Decl     Sig    Type   Btype     Fun  Params    Expr      Or     And     Cmp     Add     Mul     App    Atom    List  */
    /* I50 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I51 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I52 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I53 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I54 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I55 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I56 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I57 */ {     0,      0,      0,      0,      0,      0,      0,      0,     71,     21,     22,     23,     24,     25,     26,     27,      0},
    /* I58 */ {     0,      0,      0,      0,      0,      0,      0,      0,     72,     21,     22,     23,     24,     25,     26,     27,      0},
    /* I59 */ {     0,      0,      0,      0,      0,      0,      0,      0,     73,     21,     22,     23,     24,     25,     26,     27,      0},
    /*         Program   Decls    Decl     Sig    Type   Btype     Fun  Params    Expr      Or     And     Cmp     Add     Mul     App    Atom    List  */
    /* I60 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I61 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I62 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I63 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I64 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I65 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I66 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     49,      0},
    /* I67 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I68 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I69 */ {     0,      0,      0,      0,      0,      0,      0,      0,     74,     21,     22,     23,     24,     25,     26,     27,      0},
    /*         Program   Decls    Decl     Sig    Type   Btype     Fun  Params    Expr      Or     And     Cmp     Add     Mul     App    Atom    List  */
    /* I70 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I71 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I72 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I73 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I74 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
    /* I75 */ {     0,      0,      0,      0,      0,      0,      0,      0,     76,     21,     22,     23,     24,     25,     26,     27,      0},
    /* I76 */ {     0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0},
};
