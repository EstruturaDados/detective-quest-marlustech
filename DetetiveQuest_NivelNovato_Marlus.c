#include <stdio.h>          // Para usar printf, scanf, etc.
#include <stdlib.h>         // Para malloc e exit
#include <string.h>         // Para strcpy (copiar strings)

// ------------------------------------------------------------
// Estrutura da sala – cada cômodo da mansão
// ------------------------------------------------------------
typedef struct Sala {           // Defino um novo tipo chamado "Sala"
    char nome[50];                 // Nome do cômodo (ex: "Biblioteca")
    struct Sala *esquerda;         // Ponteiro para a sala à esquerda (NULL = não tem)
    struct Sala *direita;          // Ponteiro para a sala à direita  (NULL = não tem)
} Sala;                            // Agora posso usar apenas "Sala"

// ------------------------------------------------------------
// Função que cria uma sala nova com alocação dinâmica
// ------------------------------------------------------------
Sala* criarSala(const char *nomeDaSala) {           // Recebe o nome e devolve o ponteiro
    Sala *nova = (Sala*)malloc(sizeof(Sala));       // Pede memória do tamanho de uma Sala
    if (nova == NULL) {                             // Se não conseguiu alocar
        printf("Erro: memória insuficiente!\n");    // Avisa o erro
        exit(1);                                    // Encerra o programa
    }
    strcpy(nova->nome, nomeDaSala);                 // Copia o nome para dentro da sala
    nova->esquerda = NULL;                          // Ainda não tem filho esquerdo
    nova->direita  = NULL;                          // Ainda não tem filho direito
    return nova;                                    // Devolve o endereço da sala criada
}

// ------------------------------------------------------------
// Função auxiliar para conectar pai com filhos
// ------------------------------------------------------------
void conectarSalas(Sala *pai, Sala *filhoEsquerda, Sala *filhoDireita) {
    pai->esquerda = filhoEsquerda;      // Liga a porta esquerda
    pai->direita  = filhoDireita;       // Liga a porta direita
}

// ------------------------------------------------------------
// Função recursiva que permite explorar a mansão
// ------------------------------------------------------------
void explorarSalas(Sala *atual) {       // Recebe a sala onde o jogador está
    if (atual == NULL) return;          // Proteção (nunca deve acontecer)

    printf("\nVocê está no(a): %s\n", atual->nome);  // Mostra a sala atual

    // Se é uma folha (não tem filhos) → fim do caminho
    if (atual->esquerda == NULL && atual->direita == NULL) {
        printf("→ Não há mais portas. Você chegou ao fim deste caminho!\n\n");
        return;
    }

    char opcao;                                     // Guarda a letra digitada
    printf("Para onde deseja ir?\n");
    if (atual->esquerda != NULL)                    // Só mostra se existir
        printf("  [e] Esquerda  → %s\n", atual->esquerda->nome);
    if (atual->direita != NULL)
        printf("  [d] Direita   → %s\n", atual->direita->nome);
    printf("  [s] Sair do jogo\n");
    printf("Sua escolha: ");
    scanf(" %c", &opcao);                           // Lê uma letra (espaço antes evita bug)

    if (opcao == 'e' || opcao == 'E') {             // Jogador escolheu esquerda
        if (atual->esquerda != NULL)
            explorarSalas(atual->esquerda);         // Recursão!
        else
            printf("Não existe porta à esquerda!\n");
    }
    else if (opcao == 'd' || opcao == 'D') {        // Jogador escolheu direita
        if (atual->direita != NULL)
            explorarSalas(atual->direita);
        else
            printf("Não existe porta à direita!\n");
    }
    else if (opcao == 's' || opcao == 'S') {        // Jogador quer sair
        printf("\nVocê saiu da mansão. Até a próxima!\n\n");
        return;
    }
    else {                                          // Qualquer outra tecla
        printf("Opção inválida! Tente novamente.\n");
        explorarSalas(atual);                       // Pergunta de novo
    }
}

// ------------------------------------------------------------
// Função principal – monta a mansão e inicia o jogo
// ------------------------------------------------------------
int main() {
    printf("========================================\n");
    printf("   DETECTIVE QUEST – NÍVEL NOVATO      \n");
    printf("========================================\n\n");

    // Criação de todas as salas
    Sala *hall          = criarSala("Hall de Entrada");
    Sala *biblioteca    = criarSala("Biblioteca");
    Sala *salaJantar    = criarSala("Sala de Jantar");
    Sala *sotao         = criarSala("Sótão");
    Sala *quartoSecreto = criarSala("Quarto Secreto");
    Sala *cozinha       = criarSala("Cozinha");
    Sala *jardim        = criarSala("Jardim de Inverno");

    // Montagem da árvore usando a função conectarSalas
    conectarSalas(hall,          biblioteca,    salaJantar);
    conectarSalas(biblioteca,    sotao,         quartoSecreto);
    conectarSalas(salaJantar,    cozinha,       jardim);

    printf("Você entrou na mansão misteriosa...\n");
    printf("Explore com cuidado!\n\n");

    explorarSalas(hall);        // Começa a aventura

    printf("Obrigado por jogar o Nível Novato!\n");
    return 0;                   // Programa terminou com sucesso
}