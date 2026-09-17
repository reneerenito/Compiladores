cmake_minimum_required(VERSION 3.16)
project(nfa_to_dfa C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Cuando exista el programa principal hay que agregar aqui su ejecutable
# con los fuentes de cada parte

enable_testing()

# Pruebas de Move
add_executable(test_move tests/test_move.c src/move.c src/set.c)
target_include_directories(test_move PRIVATE src)
add_test(NAME move COMMAND test_move)

# Pruebas de Epsilon-Closure
add_executable(test_ec tests/test_ec.c src/ec.c src/set.c)
target_include_directories(test_ec PRIVATE src)
add_test(NAME e_closure COMMAND test_ec)

# Pruebas de Subconjuntos
add_executable(test_subset_c
  tests/test_subset_construction.c
  src/subset_construction.c
  src/queue.c
  src/alphabet.c
  src/move.c
  src/set.c
  src/ec.c)
target_include_directories(test_subset_c PRIVATE src)
add_test(NAME subset_c COMMAND test_subset_c)
