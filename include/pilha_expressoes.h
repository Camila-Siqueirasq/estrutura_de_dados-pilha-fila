#ifndef PILHA_EXPRESSOES_H
#define PILHA_EXPRESSOES_H

typedef struct NoChar {
    char dado;
    struct NoChar *prox;
} NoChar;

typedef struct {
    NoChar *topo;
} PilhaChar;

typedef struct NoDouble {
    double dado;
    struct NoDouble *prox;
} NoDouble;

typedef struct {
    NoDouble *topo;
} PilhaDouble;

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

int precedencia(char op);
int ehOperador(char c);
int ehExpressaoValida(const char *expr);
void infixaParaPosfixa(const char *infixa, char *posfixa);
void infixaParaPrefixa(const char *infixa, char *prefixa);
double avaliarPosfixa(const char *posfixa, int *erroDivZero);

void executarModuloPilha(void);

#endif