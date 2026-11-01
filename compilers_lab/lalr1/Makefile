CC     = cc
CFLAGS = -std=c99 -Wall -Wextra -O2

parser: src/main.c src/driver.c src/ast.c src/lexer.c src/gramatica.c src/tabla.c src/*.h
	$(CC) $(CFLAGS) -o parser src/main.c src/driver.c src/ast.c src/lexer.c src/gramatica.c src/tabla.c

clean:
	rm -f parser

.PHONY: clean
