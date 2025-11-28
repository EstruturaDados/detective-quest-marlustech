#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------
// Estrutura da sala – agora com pista secreta
// ------------------------------------------------------------
typedef struct Sala {
    char nome[50];
    char pista[100];            // Texto da pista que será coletada
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ------------------------------------------------------------
// Estrutura do nó da Árvore Binária de Busca (BST) de pistas
// ------------------------------------------------------------
typedef struct Pista {
    char texto[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// ------------------------------------------------------------
// Raiz da BST de pistas (global)
// ------------------------------------------------------------
Pista *raizPistas = NULL;       // Começa vazia

// ------------------------------------------------------------
// Cria uma sala nova
// ------------------------------------------------------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    if (!nova) exit(1);
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

void conectarSalas(Sala *pai, Sala *esq, Sala *dir) {
    pai->esquerda = esq;
    pai->direita  = dir;
}

// ------------------------------------------------------------
// Insere pista na BST (ordem alfabética)
// ------------------------------------------------------------
Pista* inserirPista(Pista *raiz, const char *texto) {
    if (raiz == NULL) {                             // Chegou no lugar certo
        Pista *nova = (Pista*)malloc(sizeof(Pista));
        strcpy(nova->texto, texto);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(texto, raiz->texto) < 0)             // Menor → esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0)        // Maior → direita
        raiz->direita = inserirPista(raiz->direita, texto);
    return raiz;
}

// ------------------------------------------------------------
// Lista pistas em ordem alfabética (percurso em-ordem)
// ------------------------------------------------------------
void listarPistas(Pista *raiz) {
    if (raiz == NULL) {
        printf("   → Nenhuma pista coletada ainda.\n");
        return;
    }
    listarPistas(raiz->esquerda);           // 1º menores
    printf("   ✓ %s\n", raiz->texto);       // 2º atual
    listarPistas(raiz->direita);            // 3º maiores
}

// ------------------------------------------------------------
// Exploração com coleta automática de pistas
// ------------------------------------------------------------
void explorarSalas(Sala *atual) {
    if (!atual) return;

    printf("\nVocê está no(a): %s\n", atual->nome);

    if (strlen(atual->pista) > 0) {                 // Tem pista?
        printf("Você encontrou uma pista!\n");
        printf("   → \"%s\"\n", atual->pista);
        raizPistas = inserirPista(raizPistas, atual->pista);
        atual->pista[0] = '\0';                     // Remove (só coleta 1 vez)
    }

    if (!atual->esquerda && !atual->direita) {
        printf("→ Fim do caminho.\n\n");
        return;
    }

    char op;
    printf("Opções:\n");
    if (atual->esquerda)  printf("  [e] Esquerda  → %s\n", atual->esquerda->nome);
    if (atual->direita)   printf("  [d] Direita   → %s\n", atual->direita->nome);
    printf("  [p] Revisar pistas coletadas\n");
    printf("  [s] Sair\n");
    printf("Escolha: ");
    scanf(" %c", &op);

    if (op == 'e' && atual->esquerda) explorarSalas(atual->esquerda);
    else if (op == 'd' && atual->direita) explorarSalas(atual->direita);
    else if (op == 'p' || op == 'P') {
        printf("\n=== PISTAS COLETADAS (ordem alfabética) ===\n");
        listarPistas(raizPistas);
        printf("==========================================\n\n");
        explorarSalas(atual);
    }
    else if (op == 's' || op == 'S') {
        printf("\nResumo final das pistas:\n");
        listarPistas(raizPistas);
        return;
    }
    else {
        printf("Inválido!\n");
        explorarSalas(atual);
    }
}

// ------------------------------------------------------------
// Main – monta a mansão com pistas
// ------------------------------------------------------------
int main() {
    printf("==============================================\n");
    printf("   DETECTIVE QUEST – NÍVEL AVENTUREIRO       \n");
    printf("   Colete e organize pistas automaticamente! \n");
    printf("==============================================\n\n");

    Sala *hall          = criarSala("Hall de Entrada", "");
    Sala *biblioteca    = criarSala("Biblioteca", "O mordomo estava na cozinha às 22h");
    Sala *cozinha       = criarSala("Cozinha", "A faca tem impressões digitais");
    Sala *salaJantar    = criarSala("Sala de Jantar", "A vítima jantou com o sobrinho");
    Sala *sotao         = criarSala("Sótão", "Carta de chantagem encontrada");
    Sala *quartoSecreto = criarSala("Quarto Secreto", "O sobrinho deve uma fortuna");
    Sala *jardim        = criarSala("Jardim de Inverno", "Pegadas levam à janela quebrada");

    conectarSalas(hall, biblioteca, salaJantar);
    conectarSalas(biblioteca, sotao, quartoSecreto);
    conectarSalas(salaJantar, cozinha, jardim);

    printf("Você entrou na mansão do crime...\n\n");
    explorarSalas(hall);

    printf("\nFim da investigação – Nível Aventureiro concluído!\n");
    return 0;
}