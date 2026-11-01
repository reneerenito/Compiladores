CC = gcc
CFLAGS = -Wall -Wextra -std=c99

lalr1: src/main.c src/driver.c src/ast.c src/lexer.c src/tabla.c src/gramatica.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f lalr1
