#ifndef FILA_IMPRESSORA_H
#define FILA_IMPRESSORA_H

typedef struct Nodo {
    int prioridade;
    char ip[16];
    struct Nodo *prox;
} Nodo;

typedef struct {
    Nodo *inicio;
    Nodo *fim;
    int tamanho;
} Fila;

void inicializarFila(Fila *f);
int filaVazia(Fila *f);
void enfileirar(Fila *f, int prioridade, const char *ip);
int desenfileirar(Fila *f, int *prioridade, char *ip);
void liberarFila(Fila *f);
void imprimirFila(const char *nome, Fila *f);

int carregarSolicitacoesDeArquivo(const char *nomeArquivo, Fila *filaEntrada);
void processarDistribuicao(Fila *fEntrada, Fila *f0, Fila *f1, Fila *f2);
void gerarArquivoExemploSeNaoExistir(const char *caminho);

void executarModuloFila(void);

#endif