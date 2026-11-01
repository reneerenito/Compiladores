
# Práctica 6: parser LALR(1) para asignaciones con apuntadores

Recibe expresiones de asignación con apuntadores, verifica si son válidas según la gramática y, si lo son, imprime su árbol de sintaxis abstracta (AST).

La tabla LALR(1) está escrita a mano en `tabla.c`, a partir del autómata LR(1) del reporte. El programa no calcula la tabla, solamente la utiliza.

## Cómo correrlo

Necesitas un compilador de C y `make`.

Desde la carpeta `lalr1/`:

```bash
make
./parser "*p = q"
```

Salida:

```text
Entrada:   *p = q
Tokens:    * id = id
Resultado: VÁLIDA

AST:
asignación
|-- desreferencia
|   `-- id p
`-- id q
```

Si la cadena es incorrecta, indica el estado y los tokens esperados:

```bash
./parser "x ="
```

```text
Resultado: RECHAZADA
Error en el fin de la entrada: en el estado I6 se esperaba uno de: *, id
```

Otras opciones:

- `--traza` muestra la pila y las acciones shift/reduce paso a paso.
- `--derivacion` imprime el árbol de derivación con las reglas utilizadas.
- `--tokens` recibe los tokens directamente: `./parser --tokens "id = id"`.
- `-f archivo` lee la entrada desde un archivo.

También se pueden combinar:

```bash
./parser --traza --derivacion "**p = q"
```

## Pruebas

```bash
make test
```

Ejecuta los casos de `pruebas/casos.txt` mediante `correr_pruebas.sh`.

Incluimos 10 casos: 5 válidos y 5 inválidos. Todos pasaron correctamente:

```text
Total: 10
Correctas: 10
Fallidas: 0
```

Para agregar una prueba, se añade una línea a `pruebas/casos.txt` con el formato `resultado_esperado|cadena`, donde `0` significa aceptada y `1` rechazada.

## Qué hay en cada archivo

- `gramatica.c/h` define las 6 producciones y los símbolos.
- `tabla.c/h` contiene las tablas ACCION e IR_A, con los estados fusionados.
- `lexer.c/h` convierte la entrada en tokens.
- `driver.c/h` implementa el algoritmo shift/reduce.
- `ast.c/h` construye el árbol sintáctico.
- `main.c` recibe la entrada y muestra el resultado.
- `pruebas/casos.txt` contiene los casos de prueba.
- `correr_pruebas.sh` ejecuta y verifica las pruebas.
- `Makefile` permite compilar, probar y limpiar el ejecutable.

## Cómo funciona el motor

El parser utiliza una pila de estados y consulta la tabla ACCION según el estado actual y el siguiente token.

- **shift**: consume el token y avanza al siguiente estado.
- **reduce**: aplica una producción, consulta IR_A y construye el nodo del árbol.
- **aceptar**: la cadena es válida.
- **error**: la cadena es rechazada.

El autómata canónico LR(1) tiene 14 estados, que se compactan en 10 mediante las fusiones LALR(1).

## Los tokens

- `id`: identificadores como `x`, `ptr` o `variable1`.
- `*`: operador de desreferencia.
- `=`: operador de asignación.

Los espacios se ignoran. Los números aislados y los caracteres no reconocidos producen un error léxico.
