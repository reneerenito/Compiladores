CC     = cc
CFLAGS = -std=c99 -Wall -Wextra -O2

parser: main.c driver.c ast.c lexer.c gramatica.c tabla.c *.h
	$(CC) $(CFLAGS) -o parser main.c driver.c ast.c lexer.c gramatica.c tabla.c

test: parser
	sh pruebas/correr_pruebas.sh

clean:
	rm -f parser

.PHONY: test clean
