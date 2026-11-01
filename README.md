# Estruturas de Dados (Pilha e Fila Encadeadas)

Implementação modular em linguagem C de aplicações práticas de estruturas de dados dinâmicas lineares: manipulação/avaliação de expressões aritméticas através de pilhas encadeadas e simulação de um escalonador de impressão em rede com prioridades utilizando filas encadeadas.

---

## Estrutura do Projeto

O código-fonte segue a convenção de compilação modular em C, isolando assinaturas de tipos e funções em cabeçalhos (`.h`) e a lógica executável em ficheiros de implementação (`.c`):

```text
trabalho_ed/
├── include/
│   ├── pilha_expressoes.h   # Assinaturas e tipos da aplicação de pilha
│   └── fila_impressora.h    # Assinaturas e tipos da aplicação de fila
├── src/
│   ├── pilha_expressoes.c   # Implementação dos algoritmos de conversão e cálculo
│   ├── fila_impressora.c    # Implementação da fila encadeada e do escalonador
│   └── main.c               # Ponto de entrada com menu unificado
├── Makefile                 # Automação de compilação
├── .gitignore               # Regras de exclusão de binários no Git
└── README.md                # Documentação técnica do projeto