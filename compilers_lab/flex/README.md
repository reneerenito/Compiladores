# P4 - Flex

Práctica 4 de Compiladores. Implementación de un analizador léxico con Flex para un lenguaje con temática de storytelling y Minions.

El scanner recibe un flujo de caracteres y lo convierte en tokens con el formato `[TOKEN:LEXEMA]`.

## Estructura

- `src/scanner.l`: reglas léxicas y expresiones regulares de Flex
- `src/scanner.h`: definición de tokens
- `src/scanner.c`: ejecución del scanner y nombres de los tokens
- `tests/prueba_basica.minion`: prueba básica del lenguaje
- `tests/prueba_tokens.minion`: prueba de palabras reservadas, literales y operadores
- `tests/prueba_errores.minion`: prueba de errores léxicos

## Lenguaje

Se utilizan palabras reservadas con temática de Minions para representar construcciones comunes de programación.

| Palabra | Equivalente |
| ------- | ----------- |
| `story` | programa principal |
| `scene` | definición de función |
| `banana` | variable |
| `gelato` | constante |
| `bello` | print |
| `papoy` | if |
| `bee-do` | else |
| `poopaye` | while |
| `tatata` | for |
| `para-tu` | foreach |
| `gru` | return |
| `kevin` | break |
| `dave` | continue |

Los tipos `int`, `float`, `string` y `char`, así como los operadores y delimitadores, conservan su sintaxis convencional.

## Compilar y correr

Los siguientes comandos se ejecutan desde `src/`.

Generar el scanner con Flex:

```bash
flex scanner.l
```

Compilar:

```bash
gcc lex.yy.c scanner.c -o scanner
```

Ejecutar de forma interactiva:

```bash
./scanner
```

Por ejemplo:

```text
banana int bananas = 3;
```

produce:

```text
[KW_VAR:banana]
[KW_INT:int]
[IDENTIFIER:bananas]
[ASSIGN:=]
[INT_LITERAL:3]
[SEMICOLON:;]
```

Para finalizar la entrada interactiva se puede usar `Ctrl + D`.

## Pruebas

Ejecutar la prueba básica:

```bash
./scanner < ../tests/prueba_basica.minion
```

Ejecutar la prueba de tokens:

```bash
./scanner < ../tests/prueba_tokens.minion
```

Ejecutar la prueba de errores léxicos:

```bash
./scanner < ../tests/prueba_errores.minion
```

El scanner reconoce palabras reservadas, identificadores, literales, operadores aritméticos, lógicos y a nivel de bits, delimitadores, comentarios y errores léxicos.