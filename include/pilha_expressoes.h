#ifndef PILHA_EXPRESSOES_H
#define PILHA_EXPRESSOES_H

// Nó para a pilha encadeada de caracteres e operadores
typedef struct NoChar {
    char dado;
    struct NoChar *prox;
} NoChar;

// Descritor da pilha de caracteres
typedef struct {
    NoChar *topo;
} PilhaChar;

// Nó para a pilha encadeada de números reais
typedef struct NoDouble {
    double dado;
    struct NoDouble *prox;
} NoDouble;

// Descritor da pilha de números reais
typedef struct {
    NoDouble *topo;
} PilhaDouble;

/* PROTÓTIPOS DAS FUNÇÕES PRIMITIVAS DE PILHA */
void inicializarPilhaChar(PilhaChar *p);
int vaziaChar(PilhaChar *p);
void pushChar(PilhaChar *p, char c);
char popChar(PilhaChar *p);
char peekChar(PilhaChar *p);
void liberarPilhaChar(PilhaChar *p);

void inicializarPilhaDouble(PilhaDouble *p);
int vaziaDouble(PilhaDouble *p);
void pushDouble(PilhaDouble *p, double d);
double popDouble(PilhaDouble *p);
void liberarPilhaDouble(PilhaDouble *p);

/* PROTÓTIPOS DE PROCESSAMENTO DE EXPRESSÕES */
int precedencia(char op);
int ehOperador(char c);
int ehExpressaoValida(const char *expr);
void infixaParaPosfixa(const char *infixa, char *posfixa);
void infixaParaPrefixa(const char *infixa, char *prefixa);
double avaliarPosfixa(const char *posfixa, int *erroDivZero);

/* SUBMENU */
void executarModuloPilha(void);

#endif