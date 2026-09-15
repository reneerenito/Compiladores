# P2 - NFA a DFA

Práctica 2 de Compiladores. Convierte un NFA en un DFA mediante la construcción de subconjuntos.

## Estado

| Componente                   | Estado    | Archivos       |
| ---------------------------- | --------- | -------------- |
| Conjuntos de estados         | Hecho     | `src/set.c/h`  |
| Algoritmo Move               | Hecho     | `src/move.c/h` |
| e-closure                    | Pendiente |                |
| Construcción de subconjuntos | Pendiente |                |

La representación base está en `src/nfa.h`. Las transiciones se guardan como tercias con estado origen, símbolo y estado destino, y el símbolo 0 se reserva para epsilon. Los conjuntos de estados son arreglos de banderas, así que agregar y consultar un estado cuesta tiempo constante y la unión no duplica.

`move(n, T, a)` regresa el conjunto de estados alcanzables desde `T` consumiendo exactamente el símbolo `a`, ignorando transiciones epsilon y de otros símbolos.

## Pruebas

Cada componente tiene su ejecutable de pruebas en `tests/`, integrado con CTest.

```bash
cmake -S . -B build && make -C build && ctest --test-dir build --output-on-failure
```
