#include "pilha_expressoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void inicializarPilhaChar(PilhaChar *p) { p->topo = NULL; }
int vaziaChar(PilhaChar *p) { return (p->topo == NULL); }

void pushChar(PilhaChar *p, char c) {
    NoChar *novo = (NoChar*) malloc(sizeof(NoChar));
    if (!novo) { perror("Falha malloc"); exit(EXIT_FAILURE); }
    novo->dado = c;
    novo->prox = p->topo;
    p->topo = novo;
}

char popChar(PilhaChar *p) {
    if (vaziaChar(p)) return '\0';
    NoChar *temp = p->topo;
    char valor = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return valor;
}

char peekChar(PilhaChar *p) {
    return vaziaChar(p) ? '\0' : p->topo->dado;
}

void liberarPilhaChar(PilhaChar *p) {
    while (!vaziaChar(p)) popChar(p);
}

void inicializarPilhaDouble(PilhaDouble *p) { p->topo = NULL; }
int vaziaDouble(PilhaDouble *p) { return (p->topo == NULL); }

void pushDouble(PilhaDouble *p, double d) {
    NoDouble *novo = (NoDouble*) malloc(sizeof(NoDouble));
    if (!novo) { perror("Falha malloc"); exit(EXIT_FAILURE); }
    novo->dado = d;
    novo->prox = p->topo;
    p->topo = novo;
}

double popDouble(PilhaDouble *p) {
    if (vaziaDouble(p)) return 0.0;
    NoDouble *temp = p->topo;
    double valor = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return valor;
}

void liberarPilhaDouble(PilhaDouble *p) {
    while (!vaziaDouble(p)) popDouble(p);
}

int precedencia(char op) {
    switch (op) {
        case '^': return 3;
        case '*': case '/': return 2;
        case '+': case '-': return 1;
        default: return 0;
    }
}

int ehOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int ehExpressaoValida(const char *expr) {
    PilhaChar p;
    inicializarPilhaChar(&p);
    int esperaOperando = 1, i = 0;

    while (expr[i] != '\0') {
        if (isspace(expr[i])) { i++; continue; }
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
            return 0;
        }
        i++;
    }
    int valida = (vaziaChar(&p) && !esperaOperando);
    liberarPilhaChar(&p);
    return valida;
}

void infixaParaPosfixa(const char *infixa, char *posfixa) {
    PilhaChar p;
    inicializarPilhaChar(&p);
    posfixa[0] = '\0';
    int i = 0;

    while (infixa[i] != '\0') {
        if (isspace(infixa[i])) { i++; continue; }
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
            if (!vaziaChar(&p)) popChar(&p);
        } else if (ehOperador(infixa[i])) {
            char opAtual = infixa[i];
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
    while (!vaziaChar(&p)) {
        char op[3] = {popChar(&p), ' ', '\0'};
        strcat(posfixa, op);
    }
    liberarPilhaChar(&p);
}

static void inverterString(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char t = str[i];
        str[i] = str[n - 1 - i];
        str[n - 1 - i] = t;
    }
}

void infixaParaPrefixa(const char *infixa, char *prefixa) {
    char rev[512];
    strcpy(rev, infixa);
    inverterString(rev);

    PilhaChar opPilha;
    inicializarPilhaChar(&opPilha);
    char posf[512] = "";
    int i = 0;

    while (rev[i] != '\0') {
        if (isspace(rev[i])) { i++; continue; }
        if (isdigit(rev[i]) || rev[i] == '.') {
            char num[64];
            int k = 0;
            while (isdigit(rev[i]) || rev[i] == '.') num[k++] = rev[i++];
            num[k] = '\0';
            inverterString(num);
            strcat(posf, num);
            strcat(posf, " ");
            continue;
        }
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

double avaliarPosfixa(const char *posfixa, int *erroDivZero) {
    PilhaDouble p;
    inicializarPilhaDouble(&p);
    *erroDivZero = 0;

    char copia[512];
    strcpy(copia, posfixa);
    char *token = strtok(copia, " ");

    while (token != NULL) {
        if (ehOperador(token[0]) && strlen(token) == 1) {
            double b = popDouble(&p);
            double a = popDouble(&p);
            double res = 0.0;
            switch (token[0]) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if (fabs(b) < 1e-9) {
                        *erroDivZero = 1;
                        liberarPilhaDouble(&p);
                        return 0.0;
                    }
                    res = a / b;
                    break;
                case '^': res = pow(a, b); break;
            }
            pushDouble(&p, res);
        } else {
            pushDouble(&p, atof(token));
        }
        token = strtok(NULL, " ");
    }
    double resFinal = popDouble(&p);
    liberarPilhaDouble(&p);
    return resFinal;
}

void executarModuloPilha(void) {
    int opcao;
    char expressao[512], buffer[512];

    do {
        printf("\n--- SUBMENU: APLICACAO DE PILHA (EXPRESSOES) ---\n");
        printf("1. Transforma expressao Infixa em Posfixa\n");
        printf("2. Transforma expressao Infixa em Prefixa\n");
        printf("3. Verifica validade de expressao\n");
        printf("4. Avalia multiplas expressoes aritmeticas\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) break;
        while (getchar() != '\n');

        switch (opcao) {
            case 1:
                printf("Infixa: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = '\0';
                if (!ehExpressaoValida(expressao)) printf("Expressao Invalida!\n");
                else {
                    infixaParaPosfixa(expressao, buffer);
                    printf("Posfixa: %s\n", buffer);
                }
                break;
            case 2:
                printf("Infixa: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = '\0';
                if (!ehExpressaoValida(expressao)) printf("Expressao Invalida!\n");
                else {
                    infixaParaPrefixa(expressao, buffer);
                    printf("Prefixa: %s\n", buffer);
                }
                break;
            case 3:
                printf("Expressao: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = '\0';
                printf("Resultado: %s\n", ehExpressaoValida(expressao) ? "VALIDA" : "INVALIDA");
                break;
            case 4: {
                int qtd;
                printf("Quantidade de expressoes: ");
                scanf("%d", &qtd);
                while (getchar() != '\n');
                char lista[qtd][512];
                for (int i = 0; i < qtd; i++) {
                    printf("[%d]: ", i + 1);
                    fgets(lista[i], sizeof(lista[i]), stdin);
                    lista[i][strcspn(lista[i], "\n")] = '\0';
                }
                printf("\n--- RESULTADOS ---\n");
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
            case 5: break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 5);
}