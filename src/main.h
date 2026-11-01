/* ========================================================================== */                                                      */
/* TRABALHO PRÁTICO - PILHA E FILA ENCADEADAS (MENU PRINCIPAL)                */
/* ========================================================================== */

#include <stdio.h>
#include "pilha_expressoes.h"
#include "fila_impressora.h"

int main(void) {
    int opcao;

    do {
        printf("\n======================================================\n");
        printf("    TRABALHO PRATICO: ESTRUTURAS DE DADOS I (SEPT/UFPR)\n");
        printf("======================================================\n");
        printf("1. Modulo 1: Aplicacao de Pilha (Expressoes Aritmeticas)\n");
        printf("2. Modulo 2: Aplicacao de Fila (Gerenciador de Impressora)\n");
        printf("3. Sair\n");
        printf("------------------------------------------------------\n");
        printf("Selecione uma opcao: ");

        if (scanf("%d", &opcao) != 1) break;
        while (getchar() != '\n'); // Limpa buffer do teclado

        switch (opcao) {
            case 1:
                executarModuloPilha();
                break;
            case 2:
                executarModuloFila();
                break;
            case 3:
                printf("\nPrograma encerrado com sucesso.\n");
                break;
            default:
                printf("\nOpcao invalida! Escolha entre 1 e 3.\n");
        }
    } while (opcao != 3);

    return 0;
}