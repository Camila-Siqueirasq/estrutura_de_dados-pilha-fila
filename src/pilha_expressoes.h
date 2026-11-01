/* ========================================================================== */
/* UNIVERSIDADE FEDERAL DO PARANÁ - UFPR                                      */
/* SETOR DE EDUCAÇÃO PROFISSIONAL E TECNOLÓGICA - SEPT                        */
/* TECNOLOGIA EM ANÁLISE E DESENVOLVIMENTO DE SISTEMAS                        */
/* ESTRUTURA DE DADOS I                                                       */
/* TRABALHO PRÁTICO - APLICAÇÃO DE PILHA (EXPRESSÕES ARITMÉTICAS)             */
/*                                                                            */
/* REFERÊNCIAS TEÓRICAS:                                                      */
/* Algoritmo Shunting-yard (Dijkstra, 1961) - Infixa para Pós-fixa/Pré-fixa    */
/* Avaliação de Notação Polonesa Reversa - Tenenbaum et al. (1995)            */
/* ========================================================================== */

#include "pilha_expressoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* PRIMITIVAS: PILHA DE CARACTERES                                            */

void inicializarPilhaChar(PilhaChar *p) { 
    p->topo = NULL; 
}

int vaziaChar(PilhaChar *p) { 
    return (p->topo == NULL); 
}

// Empilha novo caractere no topo da pilha
void pushChar(PilhaChar *p, char c) {
    NoChar *novo = (NoChar*) malloc(sizeof(NoChar));
    if (!novo) { 
        perror("Erro de alocacao de memoria"); 
        exit(EXIT_FAILURE); 
    }
    novo->dado = c;
    novo->prox = p->topo;
    p->topo = novo;
}

// Desempilha e retorna o caractere do topo
char popChar(PilhaChar *p) {
    if (vaziaChar(p)) return '\0';
    NoChar *temp = p->topo;
    char valor = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return valor;
}

// Consulta o caractere do topo sem desempilhar
char peekChar(PilhaChar *p) {
    return vaziaChar(p) ? '\0' : p->topo->dado;
}

// Libera todos os nós restantes da pilha
void liberarPilhaChar(PilhaChar *p) {
    while (!vaziaChar(p)) popChar(p);
}


/* PRIMITIVAS: PILHA DE REAIS (DOUBLE)                                        */

void inicializarPilhaDouble(PilhaDouble *p) { 
    p->topo = NULL; 
}

int vaziaDouble(PilhaDouble *p) { 
    return (p->topo == NULL); 
}

// Empilha valor numérico
void pushDouble(PilhaDouble *p, double d) {
    NoDouble *novo = (NoDouble*) malloc(sizeof(NoDouble));
    if (!novo) { 
        perror("Erro de alocacao de memoria"); 
        exit(EXIT_FAILURE); 
    }
    novo->dado = d;
    novo->prox = p->topo;
    p->topo = novo;
}

// Desempilha valor numérico
double popDouble(PilhaDouble *p) {
    if (vaziaDouble(p)) return 0.0;
    NoDouble *temp = p->topo;
    double valor = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return valor;
}

// Libera memória da pilha de números
void liberarPilhaDouble(PilhaDouble *p) {
    while (!vaziaDouble(p)) popDouble(p);
}


/* REGRAS DE PRECEDÊNCIA E OPERADORES                                         */

// Retorna o peso de precedência dos operadores matemáticos
int precedencia(char op) {
    switch (op) {
        case '^': return 3;
        case '*': case '/': return 2;
        case '+': case '-': return 1;
        default: return 0;
    }
}

// Verifica se o caractere é um operador binário válido
int ehOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}


/* VALIDAÇÃO SINTÁTICA                                                        */

// Valida balanceamento de parênteses, caracteres e operadores adjacentes
int ehExpressaoValida(const char *expr) {
    PilhaChar p;
    inicializarPilhaChar(&p);
    int esperaOperando = 1; // 1 = espera número ou '(', 0 = espera operador ou ')'
    int i = 0;

    while (expr[i] != '\0') {
        if (isspace(expr[i])) { 
            i++; 
            continue; 
        }

        if (expr[i] == '(') {
            if (!esperaOperando) { liberarPilhaChar(&p); return 0; }
            pushChar(&p, '(');
        } else if (expr[i] == ')') {
            if (vaziaChar(&p) || esperaOperando) { liberarPilhaChar(&p); return 0; }
            popChar(&p);
        } else if (ehOperador(expr[i])) {
            if (esperaOperando) { liberarPilhaChar(&p); return 0; }
            esperaOperando = 1;
        } else if (isdigit(expr[i]) || expr[i] == '.') {
            if (!esperaOperando) { liberarPilhaChar(&p); return 0; }
            int temPonto = (expr[i] == '.');
            
            // Consome o número completo (inteiro ou real)
            while (isdigit(expr[i + 1]) || expr[i + 1] == '.') {
                i++;
                if (expr[i] == '.') {
                    if (temPonto) { liberarPilhaChar(&p); return 0; }
                    temPonto = 1;
                }
            }
            esperaOperando = 0;
        } else {
            liberarPilhaChar(&p);
            return 0; // Caractere não reconhecido
        }
        i++;
    }

    int valida = (vaziaChar(&p) && !esperaOperando);
    liberarPilhaChar(&p);
    return valida;
}


/* CONVERSÃO INFIXA PARA PÓS-FIXA (SHUNTING-YARD)                             */

// Converte notação infixa para RPN separando tokens por espaço
void infixaParaPosfixa(const char *infixa, char *posfixa) {
    PilhaChar p;
    inicializarPilhaChar(&p);
    posfixa[0] = '\0';
    int i = 0;

    while (infixa[i] != '\0') {
        if (isspace(infixa[i])) { 
            i++; 
            continue; 
        }

        // Operando numérico vai direto para a saída
        if (isdigit(infixa[i]) || infixa[i] == '.') {
            char num[64];
            int k = 0;
            while (isdigit(infixa[i]) || infixa[i] == '.') num[k++] = infixa[i++];
            num[k] = '\0';
            strcat(posfixa, num);
            strcat(posfixa, " ");
            continue;
        }

        if (infixa[i] == '(') {
            pushChar(&p, '(');
        } else if (infixa[i] == ')') {
            while (!vaziaChar(&p) && peekChar(&p) != '(') {
                char op[3] = {popChar(&p), ' ', '\0'};
                strcat(posfixa, op);
            }
            if (!vaziaChar(&p)) popChar(&p); // Descarta '('
        } else if (ehOperador(infixa[i])) {
            char opAtual = infixa[i];
            
            // Trata precedência e associatividade (^ à direita, demais à esquerda)
            while (!vaziaChar(&p) && ehOperador(peekChar(&p))) {
                char opTopo = peekChar(&p);
                if ((opAtual != '^' && precedencia(opTopo) >= precedencia(opAtual)) ||
                    (opAtual == '^' && precedencia(opTopo) > precedencia(opAtual))) {
                    char op[3] = {popChar(&p), ' ', '\0'};
                    strcat(posfixa, op);
                } else break;
            }
            pushChar(&p, opAtual);
        }
        i++;
    }

    // Desempilha operadores remanescentes
    while (!vaziaChar(&p)) {
        char op[3] = {popChar(&p), ' ', '\0'};
        strcat(posfixa, op);
    }
    liberarPilhaChar(&p);
}


/* CONVERSÃO INFIXA PARA PRÉ-FIXA                                            */

// Função auxiliar interna para inversão de strings
static void inverterString(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char t = str[i];
        str[i] = str[n - 1 - i];
        str[n - 1 - i] = t;
    }
}

// Converte infixa para pré-fixa invertendo a cadeia e adaptando o algoritmo
void infixaParaPrefixa(const char *infixa, char *prefixa) {
    char rev[512];
    strcpy(rev, infixa);
    inverterString(rev);

    PilhaChar opPilha;
    inicializarPilhaChar(&opPilha);
    char posf[512] = "";
    int i = 0;

    while (rev[i] != '\0') {
        if (isspace(rev[i])) { 
            i++; 
            continue; 
        }

        if (isdigit(rev[i]) || rev[i] == '.') {
            char num[64];
            int k = 0;
            while (isdigit(rev[i]) || rev[i] == '.') num[k++] = rev[i++];
            num[k] = '\0';
            inverterString(num); // Restaura ordenação original dos dígitos
            strcat(posf, num);
            strcat(posf, " ");
            continue;
        }

        // Inversão de escopo dos parênteses
        if (rev[i] == ')') {
            pushChar(&opPilha, ')');
        } else if (rev[i] == '(') {
            while (!vaziaChar(&opPilha) && peekChar(&opPilha) != ')') {
                char op[3] = {popChar(&opPilha), ' ', '\0'};
                strcat(posf, op);
            }
            if (!vaziaChar(&opPilha)) popChar(&opPilha);
        } else if (ehOperador(rev[i])) {
            char opAtual = rev[i];
            while (!vaziaChar(&opPilha) && ehOperador(peekChar(&opPilha))) {
                char opTopo = peekChar(&opPilha);
                if ((opAtual == '^' && precedencia(opTopo) >= precedencia(opAtual)) ||
                    (opAtual != '^' && precedencia(opTopo) > precedencia(opAtual))) {
                    char op[3] = {popChar(&opPilha), ' ', '\0'};
                    strcat(posf, op);
                } else break;
            }
            pushChar(&opPilha, opAtual);
        }
        i++;
    }

    while (!vaziaChar(&opPilha)) {
        char op[3] = {popChar(&opPilha), ' ', '\0'};
        strcat(posf, op);
    }
    liberarPilhaChar(&opPilha);

    // Inverte a lista de tokens para gerar a saída pré-fixa final
    prefixa[0] = '\0';
    char *tokens[128];
    int count = 0;
    char *tk = strtok(posf, " ");
    while (tk) {
        tokens[count++] = tk;
        tk = strtok(NULL, " ");
    }
    for (int j = count - 1; j >= 0; j--) {
        strcat(prefixa, tokens[j]);
        if (j > 0) strcat(prefixa, " ");
    }
}


/* AVALIAÇÃO DE EXPRESSÃO PÓS-FIXA                                           */

// Calcula o valor numérico da expressão pós-fixa usando pilha de operandos
double avaliarPosfixa(const char *posfixa, int *erroDivZero) {
    PilhaDouble p;
    inicializarPilhaDouble(&p);
    *erroDivZero = 0;

    char copia[512];
    strcpy(copia, posfixa);
    char *token = strtok(copia, " ");

    while (token != NULL) {
        if (ehOperador(token[0]) && strlen(token) == 1) {
            // Desempilha operandos: b (topo) e a (subtopo)
            double b = popDouble(&p);
            double a = popDouble(&p);
            double res = 0.0;

            switch (token[0]) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if (fabs(b) < 1e-9) {
                        *erroDivZero = 1; // Sinaliza erro de divisão por zero
                        liberarPilhaDouble(&p);
                        return 0.0;
                    }
                    res = a / b;
                    break;
                case '^': res = pow(a, b); break;
            }
            pushDouble(&p, res);
        } else {
            // Converte string para double e empilha
            pushDouble(&p, atof(token));
        }
        token = strtok(NULL, " ");
    }

    double resFinal = popDouble(&p);
    liberarPilhaDouble(&p);
    return resFinal;
}


/* SUBMENU DE PILHA                                                           */

void executarModuloPilha(void) {
    int opcao;
    char expressao[512], buffer[512];

    do {
        printf("\n============================================\n");
        printf("   APLICACAO DE PILHA - EXPRESSOES         \n");
        printf("============================================\n");
        printf("1. Transforma expressao Infixa em Posfixa\n");
        printf("2. Transforma expressao Infixa em Prefixa\n");
        printf("3. Verifica se a expressao e valida\n");
        printf("4. Avalia expressoes aritmeticas\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) break;
        while (getchar() != '\n'); // Limpa buffer do teclado

        switch (opcao) {
            case 1:
                printf("\nDigite a expressao infixa: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = '\0';
                if (!ehExpressaoValida(expressao)) {
                    printf(">> Expressao invalida!\n");
                } else {
                    infixaParaPosfixa(expressao, buffer);
                    printf(">> Posfixa: %s\n", buffer);
                }
                break;

            case 2:
                printf("\nDigite a expressao infixa: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = '\0';
                if (!ehExpressaoValida(expressao)) {
                    printf(">> Expressao invalida!\n");
                } else {
                    infixaParaPrefixa(expressao, buffer);
                    printf(">> Prefixa: %s\n", buffer);
                }
                break;

            case 3:
                printf("\nDigite a expressao para validar: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = '\0';
                printf(">> Status: Expressao %s.\n", ehExpressaoValida(expressao) ? "VALIDA" : "INVALIDA");
                break;

            case 4: {
                int qtd;
                printf("\nQuantidade de expressoes: ");
                scanf("%d", &qtd);
                while (getchar() != '\n');

                char lista[qtd][512];
                for (int i = 0; i < qtd; i++) {
                    printf("Expressao [%d]: ", i + 1);
                    fgets(lista[i], sizeof(lista[i]), stdin);
                    lista[i][strcspn(lista[i], "\n")] = '\0';
                }

                printf("\n--- RESULTADOS OBTIDOS ---\n");
                for (int i = 0; i < qtd; i++) {
                    if (!ehExpressaoValida(lista[i])) {
                        printf("%s = INVALIDA\n", lista[i]);
                    } else {
                        int err = 0;
                        infixaParaPosfixa(lista[i], buffer);
                        double r = avaliarPosfixa(buffer, &err);
                        if (err) printf("%s = ERRO: Divisao por zero\n", lista[i]);
                        else printf("%s = %.4f\n", lista[i], r);
                    }
                }
                break;
            }

            case 5:
                printf("Retornando ao Menu Principal...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 5);
}