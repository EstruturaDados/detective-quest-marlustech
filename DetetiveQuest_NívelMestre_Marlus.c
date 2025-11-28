#include <stdio.h>          // Permite usar printf, scanf, puts, etc.
#include <stdlib.h>         // Permite usar malloc, free, exit
#include <string.h>         // Permite strcpy (copiar texto), strcmp (comparar), strstr (procurar palavra)
#include <ctype.h>          // Permite usar toupper() – transformar letra em maiúscula

// ------------------------------------------------------------
// 1) Estrutura da sala – cada cômodo da mansão
// ------------------------------------------------------------
typedef struct Sala {           // Crio um novo tipo chamado "Sala"
    char nome[50];                 // Guarda o nome da sala (ex: "Biblioteca")
    char pista[100];               // Guarda o texto da pista secreta encontrada ali
    struct Sala *esquerda;         // Ponteiro para a sala à esquerda (ou NULL se não existir)
    struct Sala *direita;          // Ponteiro para a sala à direita (ou NULL se não existir)
} Sala;                            // Agora posso usar "Sala" em vez de "struct Sala"

// ------------------------------------------------------------
// 2) Estrutura do nó da Tabela Hash (onde guardamos pista → suspeito)
// ------------------------------------------------------------
typedef struct HashNode {       // Cada nó da tabela hash
    char pista[100];               // Texto da pista coletada
    char suspeito[50];             // Nome do suspeito ligado a essa pista
    struct HashNode *prox;         // Ponteiro para o próximo nó (caso tenha colisão)
} HashNode;                        // Este nó será usado na lista encadeada da hash

// ------------------------------------------------------------
// 3) A Tabela Hash em si – array de ponteiros
// ------------------------------------------------------------
#define TAM_HASH 29                     // Tamanho da tabela (29 é número primo → menos colisões)
HashNode *tabelaHash[TAM_HASH] = {NULL};  // Crio 29 posições, todas começam com NULL (vazias)

// ------------------------------------------------------------
// 4) Função de hash – transforma uma string em número entre 0 e 28
// ------------------------------------------------------------
int funcaoHash(const char *chave) {     // Recebe qualquer texto (a pista)
    unsigned long hash = 5381;          // Valor inicial mágico (algoritmo djb2)
    int c;                              // Vai guardar cada caractere da string
    while ((c = toupper(*chave++)))     // Enquanto não chegar no fim da string
        hash = ((hash << 5) + hash) + c;  // hash = hash * 33 + letra (em maiúscula)
    return hash % TAM_HASH;             // Retorna um número entre 0 e 28
}

// ------------------------------------------------------------
// 5) Insere uma pista e seu suspeito na tabela hash
// ------------------------------------------------------------
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);           // Calcula em qual posição (0 a 28) vai ficar

    HashNode *novo = (HashNode*)malloc(sizeof(HashNode));  // Cria novo nó
    strcpy(novo->pista, pista);               // Copia a pista para dentro do nó
    strcpy(novo->suspeito, suspeito);         // Copia o nome do suspeito
    novo->prox = NULL;                        // Ainda não tem próximo

    // Encadeamento: coloca o novo nó no INÍCIO da lista daquela posição
    novo->prox = tabelaHash[indice];          // O novo aponta para o que já estava lá
    tabelaHash[indice] = novo;                // Agora a posição aponta para o novo nó

    printf("   [Hash] Pista \"%s\" vinculada ao suspeito: %s\n", pista, suspeito);
}

// ------------------------------------------------------------
// 6) Mostra TODAS as associações pista → suspeito
// ------------------------------------------------------------
void exibirTodasAssociacoes() {
    printf("\n=== TODAS AS ASSOCIAÇÕES ENCONTRADAS ===\n");
    int encontrou = 0;                        // Para saber se achou alguma coisa
    for (int i = 0; i < TAM_HASH; i++) {      // Percorre todas as 29 posições
        HashNode *atual = tabelaHash[i];      // Pega o primeiro nó dessa posição
        while (atual != NULL) {               // Enquanto tiver nó nessa lista
            printf("   Pista: \"%s\" → Suspeito: %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;              // Vai para o próximo nó da lista
            encontrou = 1;
        }
    }
    if (!encontrou) printf("   Nenhuma associação registrada ainda.\n");
}

// ------------------------------------------------------------
// 7) Descobre quem é o CULPADO (conta quantas vezes cada suspeito aparece)
// ------------------------------------------------------------
void descobrirCulpado() {
    char suspeitos[10][50] = {0};   // Máximo 10 suspeitos diferentes
    int contagem[10] = {0};         // Quantas pistas cada um tem
    int totalSuspeitos = 0;         // Quantos suspeitos diferentes encontramos

    // Percorre toda a tabela hash contando
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *atual = tabelaHash[i];
        while (atual != NULL) {
            int encontrado = 0;
            // Procura se esse suspeito já está na lista
            for (int j = 0; j < totalSuspeitos; j++) {
                if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                    contagem[j]++;      // Já existe → só aumenta a contagem
                    encontrado = 1;
                    break;
                }
            }
            // Se é um suspeito novo
            if (!encontrado && totalSuspeitos < 10) {
                strcpy(suspeitos[totalSuspeitos], atual->suspeito);
                contagem[totalSuspeitos] = 1;
                totalSuspeitos++;
            }
            atual = atual->prox;
        }
    }

    // Encontra quem tem mais pistas
    int maior = 0;
    char culpado[50] = "";
    for (int i = 0; i < totalSuspeitos; i++) {
        if (contagem[i] > maior) {
            maior = contagem[i];
            strcpy(culpado, suspeitos[i]);
        }
    }

    // Exibe o relatório final
    printf("\n==================================================\n");
    printf("           ANÁLISE FINAL DAS EVIDÊNCIAS           \n");
    printf("==================================================\n");
    for (int i = 0; i < totalSuspeitos; i++) {
        printf("   %s → %d pista(s)\n", suspeitos[i], contagem[i]);
    }
    if (maior > 0) {
        printf("\n→ CONCLUSÃO: O CULPADO É: \033[1;31m%s\033[0m!\n", culpado);
        printf("   (Mais pistas apontam para ele)\n");
    } else {
        printf("→ Não há evidências suficientes para acusar ninguém.\n");
    }
    printf("==================================================\n\n");
}

// ------------------------------------------------------------
// 8) Funções auxiliares – criar e conectar salas
// ------------------------------------------------------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    if (!nova) exit(1);                     // Se falhar, encerra
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
// 9) Exploração – agora com inteligência criminal!
// ------------------------------------------------------------
void explorarSalas(Sala *atual) {
    if (!atual) return;

    printf("\nVocê está no(a): %s\n", atual->nome);

    // === Coleta automática de pista + dedução do suspeito ===
    if (strlen(atual->pista) > 0) {
        printf("Você encontrou uma pista importante!\n");
        printf("   → \"%s\"\n", atual->pista);

        char suspeito[50];
        if (strstr(atual->pista, "mordomo") || strstr(atual->pista, "cozinha"))
            strcpy(suspeito, "Mordomo James");
        else if (strstr(atual->pista, "sobrinho") || strstr(atual->pista, "cassino"))
            strcpy(suspeito, "Sobrinho Ricardo");
        else if (strstr(atual->pista, "faca") || strstr(atual->pista, "impressões"))
            strcpy(suspeito, "Mordomo James");
        else if (strstr(atual->pista, "chantagem") || strstr(atual->pista, "jogos"))
            strcpy(suspeito, "Sobrinho Ricardo");
        else if (strstr(atual->pista, "jardineiro") || strstr(atual->pista, "janela"))
            strcpy(suspeito, "Jardineiro Luís");
        else
            strcpy(suspeito, "Desconhecido");

        inserirNaHash(atual->pista, suspeito);
        atual->pista[0] = '\0';   // Remove a pista (só coleta uma vez)
    }

    if (!atual->esquerda && !atual->direita) {
        printf("→ Fim do caminho.\n\n");
        return;
    }

    char op;
    printf("Opções: [e] Esquerda  [d] Direita  [h] Ver associações  [c] Deduzir culpado  [s] Sair\n");
    printf("Escolha: ");
    scanf(" %c", &op);

    if (op == 'e' && atual->esquerda) explorarSalas(atual->esquerda);
    else if (op == 'd' && atual->direita) explorarSalas(atual->direita);
    else if (op == 'h' || op == 'H') { exibirTodasAssociacoes(); explorarSalas(atual); }
    else if (op == 'c' || op == 'C') { descobrirCulpado(); explorarSalas(atual); }
    else if (op == 's' || op == 'S') { descobrirCulpado(); return; }
    else { printf("Inválido!\n"); explorarSalas(atual); }
}

// ------------------------------------------------------------
// 10) MAIN – início do jogo
// ------------------------------------------------------------
int main() {
    printf("\033[1;33m"); // Cor amarela no terminal
    printf("====================================================\n");
    printf("     DETECTIVE QUEST – NÍVEL MESTRE                \n");
    printf("     Sistema de Inteligência Criminal com Hash     \n");
    printf("====================================================\033[0m\n\n");

    // Criação das salas com pistas que apontam para culpados
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *biblioteca = criarSala("Biblioteca", "O mordomo foi visto saindo da cozinha às 22h");
    Sala *cozinha = criarSala("Cozinha", "A faca do crime tem impressões do mordomo");
    Sala *salaJantar = criarSala("Sala de Jantar", "A vítima discutiu com o sobrinho no jantar");
    Sala *sotao = criarSala("Sótão", "Carta: 'Pague ou todos saberão dos seus jogos'");
    Sala *quartoSecreto = criarSala("Quarto Secreto", "O sobrinho perdeu tudo no cassino");
    Sala *jardim = criarSala("Jardim de Inverno", "Pegadas do jardineiro perto da janela quebrada");

    conectarSalas(hall, biblioteca, salaJantar);
    conectarSalas(biblioteca, sotao, quartoSecreto);
    conectarSalas(salaJantar, cozinha, jardim);

    printf("Você é o melhor detetive da cidade.\n");
    printf("Resolva o crime antes que o culpado fuja!\n\n");

    explorarSalas(hall);

    printf("Fim da investigação. Você é um MESTRE!\n");
    return 0;
}