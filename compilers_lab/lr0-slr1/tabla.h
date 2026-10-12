#ifndef TABLA_H
#define TABLA_H

#include "gramatica.h"

/* Así se guardan las celdas: un número positivo es shift, uno negativo es
   reduce, ACC es aceptar y ER (0) es error. Al estado 0 nunca se hace
   shift, así que el 0 queda libre para marcar el error. */
#define S(n)  (n)
#define R(k)  (-(k))
#define ACC   999
#define ER    0

extern const short ACCION[NUM_ESTADOS][NUM_TERMINALES];
extern const short IR_A[NUM_ESTADOS][NUM_NO_TERMINALES];

#endif
