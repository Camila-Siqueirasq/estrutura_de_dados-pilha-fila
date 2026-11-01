/* ========================================================================== */
/* UNIVERSIDADE FEDERAL DO PARANÁ - UFPR                                      */
/* SETOR DE EDUCAÇÃO PROFISSIONAL E TECNOLÓGICA - SEPT                        */
/* TECNOLOGIA EM ANÁLISE E DESENVOLVIMENTO DE SISTEMAS                        */
/* ESTRUTURA DE DADOS I                                                       */
/* TRABALHO PRÁTICO - APLICAÇÃO DE FILA (ESCALONADOR DE IMPRESSORA)           */
/* ========================================================================== */

#include "fila_impressora.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tratamento multiplataforma de temporização (0.5s = 500ms)
#ifdef _WIN32
    #include <windows.h>
    #define DELAY_500MS() Sleep(500)
#else
    #include <unistd.h>
    #define DELAY_500MS() usleep(500000)
#endif


/* PRIMITIVAS: FILA ENCADEADA                                                 */

void inicializarFila(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
}

int filaVazia(Fila *f) { 
    return (f->inicio == NULL); 
}

// Enfileira nó no final da fila (Complexidade O(1))
void enfileirar(Fila *f, int prioridade, const char *ip) {
    Nodo *novo = (Nodo*) malloc(sizeof(Nodo));
    if (!novo) { 
        perror("Erro de alocacao de memoria"); 
        exit(EXIT_FAILURE); 
    }
    novo->prioridade = prioridade;
    strncpy(novo->ip, ip, sizeof(novo->ip) - 1);
    novo->ip[sizeof(novo->ip) - 1] = '\0';
    novo->prox = NULL;

    if (filaVazia(f)) {
        f->inicio = novo;
    } else {
        f->fim->prox = novo;
    }
    f->fim = novo;
    f->tamanho++;
}

// Desenfileira nó do início da fila (Complexidade O(1))
int desenfileirar(Fila *f, int *prioridade, char *ip) {
    if (filaVazia(f)) return 0;

    Nodo *temp = f->inicio;
    *prioridade = temp->prioridade;
    strcpy(ip, temp->ip);

    f->inicio = temp->prox;
    if (!f->inicio) f->fim = NULL;

    free(temp);
    f->tamanho--;
    return 1;
}

// Esvazia e desaloca todos os nós da fila
void liberarFila(Fila *f) {
    int p; 
    char ip[16];
    while (desenfileirar(f, &p, ip));
}

// Exibe os nós presentes na fila em tempo de execução
void imprimirFila(const char *nome, Fila *f) {
    printf("%-16s [Total: %2d] -> ", nome, f->tamanho);
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    Nodo *atual = f->inicio;
    while (atual) {
        printf("[%s|P%d] ", atual->ip, atual->prioridade);
        atual = atual->prox;
    }
    printf("\n");
}


/* ENTRADA DE ARQUIVO E DISTRIBUIÇÃO COM DELAY                                */

// Carrega os dados formatados do arquivo .txt para a Fila de Entrada
int carregarSolicitacoesDeArquivo(const char *nomeArquivo, Fila *filaEntrada) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return 0;

    int prioridade, total = 0;
    char ip[32];

    // Lê pares no formato: prioridade, IP;
    while (fscanf(fp, " %d , %31[^;] ;", &prioridade, ip) == 2) {
        // Limpa espaços residuais nas extremidades do IP
        char *ini = ip;
        while (*ini == ' ') ini++;
        char *fim = ini + strlen(ini) - 1;
        while (fim > ini && (*fim == ' ' || *fim == '\n' || *fim == '\r')) {
            *fim = '\0';
            fim--;
        }

        if (prioridade >= 1 && prioridade <= 3) {
            enfileirar(filaEntrada, prioridade, ini);
            total++;
        }
    }
    fclose(fp);
    return total;
}

// Descarrega 5 solicitações por ciclo a cada 0.5s direcionando por prioridade
void processarDistribuicao(Fila *fEntrada, Fila *f0, Fila *f1, Fila *f2) {
    int ciclo = 1;
    printf("\n======================================================\n");
    printf("         DISTRIBUICAO ESCALONADA DE IMPRESSAO        \n");
    printf("======================================================\n");

    while (!filaVazia(fEntrada)) {
        printf("\n--- CICLO %02d (Descarregando ate 5 requisicoes) ---\n", ciclo++);

        for (int i = 0; i < 5 && !filaVazia(fEntrada); i++) {
            int p; 
            char ip[16];
            desenfileirar(fEntrada, &p, ip);

            switch (p) {
                case 1: 
                    enfileirar(f0, p, ip); 
                    printf(" -> [P1 - Maxima] IP: %-15s -> Fila 0\n", ip); 
                    break;
                case 2: 
                    enfileirar(f1, p, ip); 
                    printf(" -> [P2 - Normal] IP: %-15s -> Fila 1\n", ip); 
                    break;
                case 3: 
                    enfileirar(f2, p, ip); 
                    printf(" -> [P3 - Baixa ] IP: %-15s -> Fila 2\n", ip); 
                    break;
            }
        }

        // Estado das 4 filas após cada rodada
        printf("\n--- STATUS DAS FILAS NO CICLO ---\n");
        imprimirFila("Fila Entrada", fEntrada);
        imprimirFila("Fila 0 (Max)", f0);
        imprimirFila("Fila 1 (Norm)", f1);
        imprimirFila("Fila 2 (Bx)", f2);

        DELAY_500MS(); // Pausa de 0.5s exigida
    }
    printf("\n================ DISTRIBUICAO CONCLUIDA ================\n");
}

// Cria arquivo padrão com 55 registros caso nenhum arquivo exista
void gerarArquivoExemploSeNaoExistir(const char *caminho) {
    FILE *f = fopen(caminho, "r");
    if (f) { 
        fclose(f); 
        return; 
    }
    f = fopen(caminho, "w");
    if (!f) return;

    for (int i = 1; i <= 55; i++) {
        fprintf(f, "%d, 192.168.1.%d; ", (i % 3) + 1, 10 + i);
    }
    fclose(f);
}


/* SUBMENU DE FILA                                                            */

void executarModuloFila(void) {
    const char *arquivo = "solicitacoes.txt";
    gerarArquivoExemploSeNaoExistir(arquivo);

    Fila fEntrada, f0, f1, f2;
    inicializarFila(&fEntrada);
    inicializarFila(&f0);
    inicializarFila(&f1);
    inicializarFila(&f2);

    int total = carregarSolicitacoesDeArquivo(arquivo, &fEntrada);
    printf("\nTotal de registros carregados de '%s': %d\n", arquivo, total);

    if (total > 0) {
        imprimirFila("Entrada Inicial", &fEntrada);
        processarDistribuicao(&fEntrada, &f0, &f1, &f2);
    } else {
        printf("Erro ao ler dados ou arquivo vazio.\n");
    }

    liberarFila(&fEntrada);
    liberarFila(&f0);
    liberarFila(&f1);
    liberarFila(&f2);
}