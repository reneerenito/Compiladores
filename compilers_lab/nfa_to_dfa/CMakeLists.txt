cmake_minimum_required(VERSION 3.16)
project(nfa_to_dfa C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Cuando exista el programa principal hay que agregar aqui su ejecutable
# con los fuentes de cada parte

enable_testing()

add_executable(test_move tests/test_move.c src/move.c src/set.c)
target_include_directories(test_move PRIVATE src)
add_test(NAME move COMMAND test_move)
