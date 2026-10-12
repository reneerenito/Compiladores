# Práctica 5: parser SLR(1) para Mini-Haskell

Recibe código de Mini-Haskell (un pedacito de Haskell), dice si es válido y, si lo es,
imprime su árbol de sintaxis abstracta.

La tabla SLR(1) está escrita a mano en `tabla.c`, copiada de la que sacamos
en el reporte. El programa no calcula la tabla, solo la sigue.

## Cómo correrlo

Necesitas un compilador de C y `make`.

```
make
./parser "inc x = x + 1 ;"
```

Y sale:

```
Entrada:   inc x = x + 1 ;
Tokens:    var var = var + num ;
Resultado: VÁLIDA

AST:
Programa
`-- Función inc (x)
    `-- +
        |-- x
        `-- 1
```

Si la cadena está mal, te dice dónde y qué esperaba:

```
./parser "f = if x then y ;"
Resultado: RECHAZADA
Error en el token #7 ';': en el estado I72 se esperaba uno de: else
```

Otras opciones:

- `--traza` muestra la pila en cada paso, útil para seguir el algoritmo a mano.
- `--derivacion` imprime también el árbol completo con la regla que se usó en cada reducción.
- `--tokens` sirve si quieres pasarle los tokens directamente: `./parser --tokens "var = var + num ;"`.
- `-f archivo` lee el código de un archivo.

## Pruebas

```
make test
```

Corre las cadenas de `pruebas/casos.txt` (9 válidas y 6 inválidas) y avisa
si alguna no dio lo que esperábamos. Para agregar una prueba, solo agrega
una línea a ese archivo.

## Qué hay en cada archivo

- `tabla.c` tiene las tablas ACCION e IR_A. Los estados que chocaban en LR(0) tienen un comentario.
- `driver.c` es el motor: la pila y el ciclo de shift y reduce.
- `ast.c` arma el árbol cada vez que se reduce una regla.
- `lexer.c` convierte el texto en tokens.
- `gramatica.c` tiene las 41 reglas, numeradas igual que en el reporte.
- `main.c` lee la entrada e imprime el resultado.

## Cómo funciona el motor

Hay una pila de estados que empieza en 0. En cada paso se mira el estado de
arriba y el siguiente token, y se busca qué dice la tabla:

- **shift**: se mete el token a la pila con el nuevo estado y se avanza.
- **reduce**: se sacan de la pila los símbolos de la regla, se mete el no terminal y se consulta IR_A para saber a qué estado ir. En ese momento también se arma el nodo del árbol.
- **aceptar**: la cadena es válida.
- **casilla vacía**: hay un error.

## Los tokens

- `var`: nombres que empiezan en minúscula (`x`, `suma`)
- `con`: nombres que empiezan en mayúscula (`Int`, `True`)
- `num`: números (`42`)
- Palabras reservadas: `if`, `then`, `else`, `let`, `in`
- Símbolos: `::` `;` `->` `[` `]` `=` `\` `||` `&&` `==` `<` `+` `-` `*` `(` `)` `,`
