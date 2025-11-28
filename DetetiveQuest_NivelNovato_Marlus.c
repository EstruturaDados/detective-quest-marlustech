#include <stdio.h>          // Permite usar comandos de entrada/saída (printf, scanf, etc.)
#include <stdlib.h>         // Permite usar malloc (alocar memória), free e exit
#include <string.h>         // Permite usar strcpy (copiar texto de um lugar para outro)

/* ------------------------------------------------------------
   1) Estrutura da sala (nó da árvore)
   ------------------------------------------------------------ */
typedef struct Sala {           // Crio um novo tipo chamado "Sala"
    char nome[50];                 // Campo que guarda o nome do cômodo (máx 49 letras + \0)
    struct Sala *esquerda;         // Ponteiro que aponta para a sala à esquerda (ou NULL)
    struct Sala *direita;          // Ponteiro que aponta para a sala à direita (ou NULL)
} Sala;                            // Agora posso usar só "Sala" em vez de "struct Sala" sempre

/* ------------------------------------------------------------
   2) Função que cria uma nova sala (alocação dinâmica)
   ------------------------------------------------------------ */
Sala* criarSala(const char *nomeDaSala) {   // Recebe um texto e devolve o endereço da sala criada
    Sala *nova = (Sala*)malloc(sizeof(Sala));  // Pede memória nova do tamanho de uma Sala
                                               // (Sala*) converte o ponteiro genérico para ponteiro de Sala
    if (nova == NULL) {                        // Se malloc falhou (computador sem memória)
        printf("Erro: memória insuficiente!\n"); // Mostra mensagem de erro
        exit(1);                                 // Encerra o programa imediatamente
    }
    strcpy(nova->nome, nomeDaSala);            // Copia o texto recebido para dentro da sala
                                               // nova->nome = acessa o campo nome da sala apontada por "nova"
    nova->esquerda = NULL;                     // Ainda não tem porta à esquerda
    nova->direita  = NULL;                     // Ainda não tem porta à direita
    return nova;                               // Devolve o endereço da sala recém-criada
}

/* ------------------------------------------------------------
   3) Função que conecta duas salas (facilita a montagem da árvore)
      pai->esquerda = filhoEsq;   ou   pai->direita = filhoDir;
   ------------------------------------------------------------ */
void conectarSalas(Sala *pai, Sala *filhoEsquerda, Sala *filhoDireita) {
    // Recebe o pai e os dois possíveis filhos (pode ser NULL)
    pai->esquerda = filhoEsquerda;   // Liga a porta esquerda do pai ao filho da esquerda
    pai->direita  = filhoDireita;    // Liga a porta direita do pai ao filho da direita
}                                    // Se passar NULL, significa que não tem aquela porta

/* ------------------------------------------------------------
   4) Função recursiva que permite ao jogador explorar a mansão
   ------------------------------------------------------------ */
void explorarSalas(Sala *atual) {       // Recebe a sala onde o jogador está agora
    if (atual == NULL) return;                  // Segurança (nunca deve acontecer)

    printf("\nVocê está no(a): %s\n", atual->nome);   // Mostra o nome da sala atual

    /* Se for uma folha (sem filhos) → fim do caminho */
    if (atual->esquerda == NULL && atual->direita == NULL) {
        printf("→ Não há mais portas. Você chegou ao fim deste caminho!\n\n");
        return;                                // Sai da função → volta para a sala anterior
    }

    /* Exibe as opções disponíveis */
    char opcao;                                // Variável que vai guardar a letra digitada
    printf("Para onde deseja ir?\n");
    if (atual->esquerda != NULL)               // Só mostra se realmente existir a sala
        printf("  [e] Esquerda  → %s\n", atual->esquerda->nome);
    if (atual->direita != NULL)
        printf("  [d] Direita   → %s\n", atual->direita->nome);

    printf("  [s] Sair do jogo\n");
    printf("Sua escolha: ");
    scanf(" %c", &opcao);                      // Lê apenas 1 letra. O espaço antes do %c é MÁGICO:
                                               // ele ignora o Enter que ficou no buffer

    /* Processa a escolha do jogador */
    if (opcao == 'e' || opcao == 'E') {         // Jogador quer ir para esquerda
        if (atual->esquerda != NULL)
            explorarSalas(atual->esquerda);    // CHAMA A PRÓPRIA FUNÇÃO de novo (recursão!)
        else
            printf("Não existe porta à esquerda!\n");
    }
    else if (opcao == 'd' || opcao == 'D') {   // Jogador quer ir para direita
        if (atual->direita != NULL)
            explorarSalas(atual->direita);     // Recursão para a direita
        else
            printf("Não existe porta à direita!\n");
    }
    else if (opcao == 's' || opcao == 'S') {   // Jogador quer sair do jogo
        printf("\nVocê saiu da mansão. Até a próxima investigação!\n\n");
        return;                                // Sai da função e termina a exploração
    }
    else {                                     // Qualquer outra tecla
        printf("Opção inválida! Tente novamente.\n");
        explorarSalas(atual);                  // Pergunta de novo na mesma sala (recursão)
    }
}

/* ------------------------------------------------------------
   5) Função main – monta a árvore fixa e inicia o jogo
   ------------------------------------------------------------ */
int main() {                               // Todo programa em C começa aqui
    printf("========================================\n");
    printf("   DETECTIVE QUEST – A MANSÃO MISTERIOSA\n");
    printf("========================================\n\n");

    /* Criação das salas (uma por uma) */
    Sala *hall          = criarSala("Hall de Entrada");     // Sala principal
    Sala *biblioteca    = criarSala("Biblioteca");
    Sala *cozinha       = criarSala("Cozinha");
    Sala *salaJantar    = criarSala("Sala de Jantar"); 
    Sala *sotao         = criarSala("Sótão");
    Sala *quartoSecreto = criarSala("Quarto Secreto");
    Sala *jardim        = criarSala("Jardim de Inverno");

    /* Conectando as salas (montando a árvore) */
    conectarSalas(hall,          biblioteca,    salaJantar);     // Do hall saem 2 portas
    conectarSalas(biblioteca,    sotao,         quartoSecreto); // Da biblioteca saem 2 portas
    conectarSalas(salaJantar,    cozinha,       jardim);        // Da sala de jantar saem 2 portas
    // As 4 últimas salas (sótão, quarto secreto, cozinha e jardim) não têm filhos → são folhas

    /* Início da exploração */
    printf("Você acabou de entrar na mansão...\n");
    printf("Procure pistas em cada cômodo!\n\n");

    explorarSalas(hall);    // Começa o jogo a partir do Hall de Entrada

    printf("Obrigado por jogar Detective Quest – Nível Novato!\n");
    return 0;               // Indica que o programa terminou com sucesso
}