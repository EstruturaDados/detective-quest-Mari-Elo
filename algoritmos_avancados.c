//NIVEL AVENTUREIRO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- ESTRUTURAS DE DADOS ---

// 1. Estrutura para o MAPA (Cômdo)
typedef struct Sala {
    char nome[50];
    char pista[100];       // Pista escondida na sala (pode ser vazia)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// 2. Estrutura para o INVENTÁRIO DE PISTAS (BST)
typedef struct PistaNode {
    char texto[100];            // O conteúdo da pista
    struct PistaNode *esquerda; // Pistas alfabeticamente menores
    struct PistaNode *direita;  // Pistas alfabeticamente maiores
} PistaNode;

// --- FUNÇÕES DO SISTEMA DE PISTAS (BST) ---

// Cria um novo nó de pista na memória
PistaNode* criarNoPista(char *texto) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (novo) {
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
    }
    return novo;
}

// Insere uma pista na BST mantendo a ordem alfabética
PistaNode* inserirPista(PistaNode *raiz, char *texto) {
    // Caso base: se chegamos a um lugar vazio, criamos o nó aqui
    if (raiz == NULL) {
        return criarNoPista(texto);
    }

    // Comparação alfabética (strcmp)
    // < 0: texto é "menor" (vem antes no alfabeto) -> vai para a esquerda
    // > 0: texto é "maior" (vem depois no alfabeto) -> vai para a direita
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } 
    else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    // Se for igual (0), ignoramos para não ter pistas duplicadas

    return raiz;
}

// Exibe as pistas em ordem (Percurso Em-Ordem: Esquerda -> Raiz -> Direita)
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);  // 1. Visita os menores (A-M)
        printf("- %s\n", raiz->texto); // 2. Mostra a atual
        exibirPistas(raiz->direita);   // 3. Visita os maiores (N-Z)
    }
}

// --- FUNÇÕES DO MAPA DA MANSÃO ---

// Cria uma sala do mapa
Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova) {
        strcpy(nova->nome, nome);
        // Se houver pista, copia; senão, deixa vazio
        if (pista != NULL) strcpy(nova->pista, pista);
        else strcpy(nova->pista, "");
        
        nova->esquerda = NULL;
        nova->direita = NULL;
    }
    return nova;
}

// Controla a navegação e a coleta
void explorarSalasComPistas(Sala *raizMapa) {
    Sala *atual = raizMapa;
    PistaNode *raizPistas = NULL; // O inventário começa vazio
    char opcao;

    printf("\n--- Inicio da Investigacao: Detective Quest ---\n");

    while (atual != NULL) {
        printf("\n-----------------------------------\n");
        printf("VOCE ESTA EM: [ %s ]\n", atual->nome);

        // --- Lógica de Coleta de Pistas ---
        // Se a sala tem uma pista e não é uma string vazia
        if (strlen(atual->pista) > 0) {
            printf(">>> Voce encontrou uma pista: \"%s\"\n", atual->pista);
            printf(">>> (Adicionando ao caderno de anotacoes...)\n");
            
            // Insere na BST de pistas
            raizPistas = inserirPista(raizPistas, atual->pista);
            
            // (Opcional) Limpar a pista da sala para não coletar a mesma 2 vezes
            strcpy(atual->pista, ""); 
        } else {
            printf("(Nada de suspeito por aqui...)\n");
        }

        // --- Navegação ---
        printf("\nOpcoes:\n");
        if (atual->esquerda) printf("(e) Ir para esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("(d) Ir para direita: %s\n", atual->direita->nome);
        printf("(s) Sair da mansao e ler pistas\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita) {
            atual = atual->direita;
        } else if (opcao == 's') {
            break;
        } else {
            printf("Caminho invalido!\n");
        }
    }

    // --- Fim do Jogo: Relatório ---
    printf("\n===================================\n");
    printf("       RELATORIO DO DETETIVE       \n");
    printf("===================================\n");
    printf("Pistas coletadas (Ordem Alfabetica):\n");
    
    if (raizPistas == NULL) {
        printf("(Nenhuma pista foi encontrada.)\n");
    } else {
        exibirPistas(raizPistas);
    }
    printf("===================================\n");
}

// --- MAIN ---
int main() {
    // 1. Construção do Mapa (Árvore Binária Normal)
    // Layout:
    //          [Hall]
    //         /      \
    //   [Biblioteca] [Cozinha]
    //      /            \
    // [Escritorio]     [Jardim]

    Sala *hall = criarSala("Hall de Entrada", "Pegada de lama no tapete");
    Sala *biblioteca = criarSala("Biblioteca", "Livro de venenos aberto");
    Sala *cozinha = criarSala("Cozinha", "Faca faltando no suporte");
    Sala *escritorio = criarSala("Escritorio Secreto", "Carta de ameaca na mesa");
    Sala *jardim = criarSala("Jardim", "Luva rasgada perto das rosas");

    // Conectando as salas
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    
    biblioteca->esquerda = escritorio;
    // biblioteca->direita = NULL;
    
    cozinha->direita = jardim;
    // cozinha->esquerda = NULL;

    // 2. Iniciar Exploração
    explorarSalasComPistas(hall);

    return 0;
}