#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM_FACIL 15
#define DIM_MEDIO 20
#define NUM_PALAVRAS 10

typedef struct Palavra {
    char palavra[20];
    char pista[100];
    char pista_extra[100];
    int x, y;
    char direcao;
    int ativa;
    int tentativas;
    struct Palavra *prox;
} Palavra;

void inicializarTabuleiro(char **tabuleiro, int dim);
void exibirTabuleiro(char **tabuleiro, int dim);
void exibirMenu();
int escolherDificuldade();
Palavra *carregarPalavras(int dificuldade);
void jogar(Palavra *listaPalavras, int dim, int *completou);
void liberarPalavras(Palavra *head);
int verificarPalavra(char *palavra, int x, int y, char direcao, char *correct_word, int dim);
int inserirPalavra(char *palavra, int x, int y, char direcao, char **tabuleiro, int dim);
Palavra *inserirOrdenado(Palavra *head, Palavra *novaPalavra);

char **tabuleiro;
int pontuacao = 1000;

int main() {
    int dificuldade;
    int dim;
    int completou = 0;

    while (1) {
        exibirMenu();
        dificuldade = escolherDificuldade();

        if (dificuldade == 0) {
            printf("Saindo do jogo...\n");
            break;
        }

        if (dificuldade == 1) {
            dim = DIM_FACIL;
        } else if (dificuldade == 2) {
            dim = DIM_MEDIO;
        } else {
            continue;
        }

        // Alocar tabuleiro dinamicamente
        tabuleiro = (char **)malloc(dim * sizeof(char *));
        for (int i = 0; i < dim; i++) {
            tabuleiro[i] = (char *)malloc(dim * sizeof(char));
        }

        inicializarTabuleiro(tabuleiro, dim);
        Palavra *listaPalavras = carregarPalavras(dificuldade);
        jogar(listaPalavras, dim, &completou);
        liberarPalavras(listaPalavras);

        // Liberar memória do tabuleiro
        for (int i = 0; i < dim; i++) {
            free(tabuleiro[i]);
        }
        free(tabuleiro);

        if (completou) {
            printf("Parabéns, você resolveu todas as palavras!\n");
            break;
        }
    }

    // Imprimir a pontuação final
    printf("Sua pontuação final é: %d\n", pontuacao);

    return 0;
}

void inicializarTabuleiro(char **tabuleiro, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            tabuleiro[i][j] = '.';
        }
    }
}

void exibirTabuleiro(char **tabuleiro, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void exibirMenu() {
    printf("Bem-vindo ao jogo de palavras cruzadas!\n");
    printf("Escolha o nível de dificuldade:\n");
    printf("1. Fácil\n");
    printf("2. Médio\n");
    printf("0. Sair\n");
}

int escolherDificuldade() {
    int escolha;
    char buffer[10];
    printf("Digite sua escolha (0 para Sair, 1 para Fácil, 2 para Médio): ");
    fgets(buffer, 10, stdin);
    escolha = (int)strtol(buffer, NULL, 10);
    while (escolha < 0 || escolha > 2) {
        printf("Escolha inválida, tente novamente: ");
        fgets(buffer, 10, stdin);
        escolha = (int)strtol(buffer, NULL, 10);
    }
    return escolha;
}

Palavra *criarPalavra(const char *palavra, const char *pista, const char *pista_extra, int x, int y, char direcao) {
    Palavra *novaPalavra = (Palavra *)malloc(sizeof(Palavra));
    if (novaPalavra == NULL) {
        printf("Erro de alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    strcpy(novaPalavra->palavra, palavra);
    strcpy(novaPalavra->pista, pista);
    strcpy(novaPalavra->pista_extra, pista_extra);
    novaPalavra->x = x;
    novaPalavra->y = y;
    novaPalavra->direcao = direcao;
    novaPalavra->ativa = 1;
    novaPalavra->tentativas = 0;
    novaPalavra->prox = NULL;
    return novaPalavra;
}

int inserirPalavra(char *palavra, int x, int y, char direcao, char **tabuleiro, int dim) {
    int len = strlen(palavra);
    if (direcao == 'H') { // Inserção horizontal
        if (y + len > dim) return 0; // Palavra excede o limite do tabuleiro
        for (int i = 0; i < len; i++) {
            if (tabuleiro[x][y + i] != '.' && tabuleiro[x][y + i] != palavra[i]) {
                return 0; // Conflito encontrado
            }
        }
        for (int i = 0; i < len; i++) {
            tabuleiro[x][y + i] = palavra[i];
        }
    } else if (direcao == 'V') { // Inserção vertical
        if (x + len > dim) return 0; // Palavra excede o limite do tabuleiro
        for (int i = 0; i < len; i++) {
            if (tabuleiro[x + i][y] != '.' && tabuleiro[x + i][y] != palavra[i]) {
                return 0; // Conflito encontrado
            }
        }
        for (int i = 0; i < len; i++) {
            tabuleiro[x + i][y] = palavra[i];
        }
    }
    return 1; // Palavra inserida com sucesso
}

Palavra *inserirOrdenado(Palavra *head, Palavra *novaPalavra) {
    if (head == NULL || strcmp(novaPalavra->palavra, head->palavra) < 0) {
        novaPalavra->prox = head;
        return novaPalavra;
    }
    Palavra *atual = head;
    while (atual->prox != NULL && strcmp(novaPalavra->palavra, atual->prox->palavra) > 0) {
        atual = atual->prox;
    }
    novaPalavra->prox = atual->prox;
    atual->prox = novaPalavra;
    return head;
}

Palavra *carregarPalavras(int dificuldade) {
    Palavra *head = NULL;
    if (dificuldade == 1) { // Fácil
        char *palavras_facil[NUM_PALAVRAS] = {"casa", "gato", "sol", "livro", "arroz", "mesa", "teto", "chuva", "noite", "flor"};
        char *pistas_facil[NUM_PALAVRAS] = {
                "Lugar onde moramos", "Animal doméstico que mia", "Astro-rei", "Objeto usado para ler",
                "Comida básica do brasileiro", "Móvel usado para refeições", "Parte superior de um cômodo",
                "Precipitação comum do tempo", "Oposto de dia", "Bela e cheirosa, cresce no jardim"
        };
        char *pistas_extras_facil[NUM_PALAVRAS] = {
                "Tem portas e janelas", "Famoso por ter nove vidas", "Aquece nosso planeta", "Pode ser digital ou de papel",
                "Geralmente cozido em água", "Tem quatro pernas e é usada para comer", "Fica no alto das casas",
                "Associada a um guarda-chuva", "Quando as estrelas saem", "Pode ser de várias cores e aromas"
        };
        int posicoes_x[NUM_PALAVRAS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int posicoes_y[NUM_PALAVRAS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        char direcoes[NUM_PALAVRAS] = {'V', 'H', 'V', 'H', 'V', 'H', 'V', 'H', 'V', 'H'};
        for (int i = 0; i < NUM_PALAVRAS; i++) {
            Palavra *novaPalavra = criarPalavra(palavras_facil[i], pistas_facil[i], pistas_extras_facil[i], posicoes_x[i], posicoes_y[i], direcoes[i]);
            head = inserirOrdenado(head, novaPalavra);
        }
    } else { // Médio
        char *palavras_medio[NUM_PALAVRAS] = {"biblioteca", "universidade", "tecnologia", "montanha", "elefante",
                                              "computador", "orquidea", "revolucao", "galaxia", "nebulosa"};
        char *pistas_medio[NUM_PALAVRAS] = {
                "Lugar cheio de livros", "Centro de ensino superior", "Campo ligado a inovações", "Grande elevação natural da terra",
                "Grande mamífero com uma longa tromba", "Máquina para processar informações", "Tipo de flor que pode viver em árvores",
                "Mudança brusca e radical", "Conjunto enorme de estrelas e planetas", "Nuvem de gás no espaço"
        };
        char *pistas_extras_medio[NUM_PALAVRAS] = {
                "Tem muitos livros e silêncio", "Geralmente tem vários cursos", "Essencial no mundo moderno",
                "Pode ter neve no topo", "Tem presas grandes e pesadas", "Essencial para trabalhos modernos",
                "Delicada e muito apreciada por jardineiros", "Pode ser social, cultural ou política",
                "O sistema solar faz parte de uma", "Visível no céu como uma mancha luminosa"
        };
        int posicoes_x[NUM_PALAVRAS] = {0, 2, 4, 6, 8, 10, 12, 3, 7, 17};
        int posicoes_y[NUM_PALAVRAS] = {0, 2, 4, 6, 8, 10, 12, 1, 3, 2};
        char direcoes[NUM_PALAVRAS] = {'V', 'H', 'V', 'H', 'V', 'H', 'V', 'H', 'V', 'H'};
        for (int i = 0; i < NUM_PALAVRAS; i++) {
            Palavra *novaPalavra = criarPalavra(palavras_medio[i], pistas_medio[i], pistas_extras_medio[i], posicoes_x[i], posicoes_y[i], direcoes[i]);
            head = inserirOrdenado(head, novaPalavra);
        }
    }
    return head;
}

int verificarPalavra(char *palavra, int x, int y, char direcao, char *correct_word, int dim) {
    int len = strlen(palavra);

    // Verificar se a palavra corresponde à palavra correta
    if (strcmp(palavra, correct_word) != 0) {
        printf("Palavra não corresponde à palavra correta.\n"); // Depuração
        return 0; // Palavra incorreta
    }

    // Verificações adicionais para posicionamento e conflitos
    if (direcao == 'H') {
        if (y + len > dim) {
            printf("Palavra excede os limites horizontais do tabuleiro. x=%d, y=%d, len=%d\n", x, y, len); // Depuração
            return 0;
        }
        for (int i = 0; i < len; i++) {
            if (tabuleiro[x][y + i] != '.' && tabuleiro[x][y + i] != palavra[i]) {
                printf("Conflito encontrado na posição (%d, %d). x=%d, y=%d\n", x, y + i, x, y); // Depuração
                return 0;
            }
        }
    } else if (direcao == 'V') {
        if (x + len > dim) {
            printf("Palavra excede os limites verticais do tabuleiro. x=%d, y=%d, len=%d\n", x, y, len); // Depuração
            return 0;
        }
        for (int i = 0; i < len; i++) {
            if (tabuleiro[x + i][y] != '.' && tabuleiro[x + i][y] != palavra[i]) {
                printf("Conflito encontrado na posição (%d, %d). x=%d, y=%d\n", x + i, y, x, y); // Depuração
                return 0;
            }
        }
    }

    return 1;
}

void jogar(Palavra *listaPalavras, int dim, int *completou) {
    inicializarTabuleiro(tabuleiro, dim);
    int jogo_ativo = 1;
    char palavra_usuario[20];
    int indice_palavra, encontrou;
    Palavra *palavra_atual;
    char buffer[20];

    while (jogo_ativo) {
        jogo_ativo = 0;
        int contagem = 0;
        palavra_atual = listaPalavras;
        while (palavra_atual != NULL) {
            if (palavra_atual->ativa) {
                printf("%d. Pista: %s\n", contagem, palavra_atual->pista);
                jogo_ativo = 1;
            }
            palavra_atual = palavra_atual->prox;
            contagem++;
        }

        if (jogo_ativo) {
            printf("Selecione o número da palavra que deseja resolver (ou digite -1 para voltar ao menu principal): ");
            fgets(buffer, sizeof(buffer), stdin);
            sscanf(buffer, "%d", &indice_palavra);

            if (indice_palavra == -1) {
                printf("Saindo do jogo...\n");
                return;
            }

            contagem = 0;
            palavra_atual = listaPalavras;
            encontrou = 0;
            while (palavra_atual != NULL && !encontrou) {
                if (contagem == indice_palavra && palavra_atual->ativa) {
                    encontrou = 1;
                } else {
                    palavra_atual = palavra_atual->prox;
                    contagem++;
                }
            }

            if (!encontrou) {
                printf("Seleção inválida, tente novamente.\n");
                continue;
            }

            printf("Insira sua palavra (use até 19 caracteres): ");
            fgets(buffer, sizeof(buffer), stdin);
            sscanf(buffer, "%19s", palavra_usuario);

            printf("Verificando palavra: %s\n", palavra_usuario); // Para depuração

            if (verificarPalavra(palavra_usuario, palavra_atual->x, palavra_atual->y, palavra_atual->direcao, palavra_atual->palavra, dim)) {
                inserirPalavra(palavra_usuario, palavra_atual->x, palavra_atual->y, palavra_atual->direcao, tabuleiro, dim);
                palavra_atual->ativa = 0;
                printf("Palavra correta! Pontuação: %d\n", pontuacao);
            } else {
                palavra_atual->tentativas++;
                pontuacao -= 10; // Deduzir pontos por palavra incorreta
                printf("Palavra incorreta!\n");
                if (palavra_atual->tentativas % 3 == 0) {
                    char resposta;
                    printf("Deseja receber uma dica extra? (s/n): ");
                    fgets(buffer, sizeof(buffer), stdin);
                    sscanf(buffer, " %c", &resposta);
                    if (resposta == 's' || resposta == 'S') {
                        printf("Dica extra: %s\n", palavra_atual->pista_extra);
                    }
                }
            }

            exibirTabuleiro(tabuleiro, dim);
        }
    }

    *completou = 1;
}

void liberarPalavras(Palavra *head) {
    Palavra *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->prox;
        free(tmp);
    }
}
