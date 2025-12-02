//NIVEL NOVATO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

C --- 1. Definição da Estrutura (Struct) ---
typedef struct Sala {
    char nome[50];          // Nome do cômodo
    struct Sala *esquerda;  // Caminho à esquerda
    struct Sala *direita;   // Caminho à direita
} Sala;

// --- 2. Função para criar uma nova sala (Alocação Dinâmica) ---
Sala* criarSala(char *nomeSala) {
    // Aloca memória para uma nova sala
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    
    // Copia o nome para a estrutura e inicializa os caminhos como NULL (sem saída)
    strcpy(novaSala->nome, nomeSala);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

// --- 3. Função de Exploração (Navegação) ---
void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char opcao;

    printf("\n--- Iniciando a Exploracao em Detective Quest ---\n");

    // Loop de navegação: continua enquanto houver uma sala para visitar
    while (atual != NULL) {
        printf("\nvoce esta em: [ %s ]\n", atual->nome);

        // Verifica se é um nó-folha (sem saídas)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Esta sala nao tem mais saidas. Fim da linha!\n");
            printf("Investigacao concluida neste caminho.\n");
            break; // Sai do loop
        }

        printf("Para onde deseja ir?\n");
        // Mostra opções baseadas na existência das salas
        if (atual->esquerda != NULL) printf("(e) Esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("(d) Direita: %s\n", atual->direita->nome);
        printf("(s) Sair do jogo\n");
        
        printf("Escolha: ");
        scanf(" %c", &opcao); // O espaço antes de %c limpa o buffer do teclado

        // Lógica de movimentação
        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda; // Move o ponteiro para a sala da esquerda
        } 
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita; // Move o ponteiro para a sala da direita
        } 
        else if (opcao == 's') {
            printf("Saindo da mansao...\n");
            break;
        } 
        else {
            printf("Caminho invalido ou inexistente! Tente novamente.\n");
        }
    }
}

// --- Função Principal (Main) ---
int main() {
    // 1. Criação das salas (Nós)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim de Inverno");

    // 2. Montagem do Mapa (Conexões da Árvore)
    // Estrutura:
    //         [Hall]
    //        /      \
    // [Sala Estar]  [Cozinha]
    //    /             \
    //[Biblioteca]     [Jardim]

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    
    salaEstar->esquerda = biblioteca;
    // salaEstar->direita continua NULL
    
    cozinha->direita = jardim;
    // cozinha->esquerda continua NULL

    // 3. Iniciar o Jogo
    explorarSalas(hall);

    // (Opcional) Limpeza de memória não incluída para manter simplicidade do nível novato,
    // mas em um programa real, deveríamos liberar cada nó com free().
    
    return 0;
}