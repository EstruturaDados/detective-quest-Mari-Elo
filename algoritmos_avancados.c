//NIVEL MESTRE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- CONSTANTES ---
#define TAM_HASH 31 // Número primo para melhor distribuição

// =============================================================
// 1. ESTRUTURAS DE DADOS
// =============================================================

// --- ESTRUTURA 1: MAPA (Árvore Binária) ---
typedef struct Sala {
    char nome[50];
    char pista[50]; // Pista encontrada na sala
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- ESTRUTURA 2: INVENTÁRIO (BST) ---
typedef struct PistaNode {
    char texto[50];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// --- ESTRUTURA 3: LÓGICA (Tabela Hash) ---
// Nó para tratamento de colisões (Encadeamento)
typedef struct HashItem {
    char pista[50];    // Chave
    char suspeito[50]; // Valor
    struct HashItem *proximo;
} HashItem;

// A Tabela Hash em si (array de ponteiros)
HashItem* tabelaHash[TAM_HASH];

// =============================================================
// 2. IMPLEMENTAÇÃO DA TABELA HASH (Associação)
// =============================================================

// Função de Hash: Soma ASCII % Tamanho
int funcaoHash(char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

// Inserir associação Pista -> Suspeito na Hash
void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);
    
    HashItem *novo = (HashItem*) malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = NULL;

    // Inserção no início da lista (tratamento de colisão)
    if (tabelaHash[indice] == NULL) {
        tabelaHash[indice] = novo;
    } else {
        novo->proximo = tabelaHash[indice];
        tabelaHash[indice] = novo;
    }
}

// Buscar suspeito baseado na pista
char* encontrarSuspeito(char *pista) {
    int indice = funcaoHash(pista);
    HashItem *atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL; // Pista não cadastrada na base de dados
}

// Inicializa a tabela com NULL
void inicializarHash() {
    for(int i = 0; i < TAM_HASH; i++) tabelaHash[i] = NULL;
}

// =============================================================
// 3. IMPLEMENTAÇÃO DA BST (Inventário)
// =============================================================

PistaNode* adicionarPista(PistaNode *raiz, char *texto) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = adicionarPista(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->direita = adicionarPista(raiz->direita, texto);
    
    return raiz;
}

void exibirPistasColetadas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistasColetadas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistasColetadas(raiz->direita);
    }
}

// =============================================================
// 4. IMPLEMENTAÇÃO DO MAPA (Árvore Binária)
// =============================================================

Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    if (pista) strcpy(nova->pista, pista);
    else strcpy(nova->pista, "");
    
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// =============================================================
// 5. LÓGICA DE JULGAMENTO
// =============================================================

// Variável auxiliar para contagem na recursão
int provasContraSuspeito = 0;

// Percorre a BST, consulta a Hash e conta evidências
void verificarSuspeitoFinal(PistaNode *raizPistas, char *acusado) {
    if (raizPistas != NULL) {
        // 1. Visita recursiva (Esquerda)
        verificarSuspeitoFinal(raizPistas->esquerda, acusado);

        // 2. Processamento (Nó Atual)
        char *suspeitoApontado = encontrarSuspeito(raizPistas->texto);
        
        if (suspeitoApontado != NULL) {
            printf("Verificando pista '%s'... Aponta para: %s\n", raizPistas->texto, suspeitoApontado);
            if (strcmp(suspeitoApontado, acusado) == 0) {
                provasContraSuspeito++;
            }
        }

        // 3. Visita recursiva (Direita)
        verificarSuspeitoFinal(raizPistas->direita, acusado);
    }
}

// =============================================================
// 6. LOOP PRINCIPAL E EXECUÇÃO
// =============================================================

void explorarSalas(Sala *raizMapa) {
    Sala *atual = raizMapa;
    PistaNode *raizPistas = NULL;
    char opcao;

    printf("\n--- MISTÉRIO NA MANSÃO ENIGMA ---\n");

    while (atual != NULL) {
        printf("\n-----------------------------------\n");
        printf("LOCAL: [ %s ]\n", atual->nome);

        // Verifica e coleta pista
        if (strlen(atual->pista) > 0) {
            printf(">>> Voce encontrou: \"%s\"\n", atual->pista);
            raizPistas = adicionarPista(raizPistas, atual->pista);
            strcpy(atual->pista, ""); // Remove a pista da sala após coleta
        } else {
            printf("(Nenhuma nova pista aqui...)\n");
        }

        printf("\n[e] Esquerda | [d] Direita | [s] Sair e Acusar\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita) atual = atual->direita;
        else if (opcao == 's') break;
        else printf("Caminho bloqueado!\n");
    }

    // --- FASE DE JULGAMENTO ---
    char acusado[50];
    printf("\n===================================\n");
    printf("       SESSÃO DE JULGAMENTO        \n");
    printf("===================================\n");
    printf("Pistas Coletadas:\n");
    exibirPistasColetadas(raizPistas);
    
    printf("\nQuem e o culpado? (Ex: Mordomo, Viuva, Jardineiro): ");
    scanf("%s", acusado);

    printf("\n--- Analisando evidencias... ---\n");
    provasContraSuspeito = 0; // Reseta contador global
    verificarSuspeitoFinal(raizPistas, acusado);

    printf("\n--- VEREDICTO ---\n");
    if (provasContraSuspeito >= 2) {
        printf("SUCESSO! Voce reuniu %d provas contra %s.\n", provasContraSuspeito, acusado);
        printf("A policia foi chamada e o caso foi encerrado!\n");
    } else {
        printf("FRACASSO. Voce tem apenas %d prova(s) contra %s.\n", provasContraSuspeito, acusado);
        printf("O juiz considerou as provas insuficientes. O culpado escapou!\n");
    }
}

int main() {
    inicializarHash();

    // 1. CONFIGURAR A "INTELIGÊNCIA" (Hash Table)
    // Definimos quem é ligado a qual pista
    inserirNaHash("Faca", "Mordomo");
    inserirNaHash("Relogio Quebrado", "Mordomo");
    inserirNaHash("Veneno", "Viuva");
    inserirNaHash("Luva de Couro", "Jardineiro");

    // 2. CONFIGURAR O MAPA (Árvore Binária)
    /* Layout:
            [Hall]
           /      \
      [Cozinha]  [Biblioteca]
      (Faca)      (Veneno)
        /           \
     [Quarto]     [Jardim]
     (Relogio)     (Luva)
    */
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *cozinha = criarSala("Cozinha", "Faca");
    Sala *biblioteca = criarSala("Biblioteca", "Veneno");
    Sala *quarto = criarSala("Quarto Principal", "Relogio Quebrado");
    Sala *jardim = criarSala("Jardim", "Luva de Couro");

    hall->esquerda = cozinha;
    hall->direita = biblioteca;
    cozinha->esquerda = quarto;
    biblioteca->direita = jardim;

    // 3. INICIAR O JOGO
    explorarSalas(hall);

    return 0;
}