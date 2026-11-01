#ifndef FILA_IMPRESSORA_H
#define FILA_IMPRESSORA_H

// Nó para cada solicitação de impressão em rede
typedef struct Nodo {
    int prioridade;          // 1 = Máxima, 2 = Normal, 3 = Baixa
    char ip[16];             // IPv4 do computador de origem
    struct Nodo *prox;
} Nodo;

// Descritor da fila encadeada com ponteiros de início e fim
typedef struct {
    Nodo *inicio;
    Nodo *fim;
    int tamanho;
} Fila;

/* PROTÓTIPOS DAS PRIMITIVAS DE FILA */
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
void enfileirar(Fila *f, int prioridade, const char *ip);
int desenfileirar(Fila *f, int *prioridade, char *ip);
void liberarFila(Fila *f);
void imprimirFila(const char *nome, Fila *f);

/* PROTÓTIPOS DE DISTRIBUIÇÃO E I/O */
int carregarSolicitacoesDeArquivo(const char *nomeArquivo, Fila *filaEntrada);
void processarDistribuicao(Fila *fEntrada, Fila *f0, Fila *f1, Fila *f2);
void gerarArquivoExemploSeNaoExistir(const char *caminho);

/* SUBMENU */
void executarModuloFila(void);

#endif