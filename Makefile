CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lm

all: trabalho_ed.exe

trabalho_ed.exe: src/main.c src/pilha_expressoes.c src/fila_impressora.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run: trabalho_ed.exe
	./trabalho_ed.exe

clean:
	del /Q /F trabalho_ed.exe 2>nul