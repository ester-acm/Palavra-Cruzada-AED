#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM 10
#define NUM_PALAVRAS 10

typedef struct {
    char palavra[20];
    char pista[100];
    char pista_extra[100];
    int x, y;
    char direcao;
    int ativa;
    int tentativas;  // Contador de tentativas erradas
} Palavra;

void inicializar_tabuleiro(char tabuleiro[DIM][DIM]);
void exibir_tabuleiro(char tabuleiro[DIM][DIM]);
void exibir_menu();
int escolher_dificuldade();
void carregar_palavras(int dificuldade);
void jogar(int dificuldade);
int verificar_palavra(char *palavra, int x, int y, char direcao);
void inserir_palavra(char *palavra, int x, int y, char direcao);
void solicitar_dica(int indice_palavra);

Palavra palavras[NUM_PALAVRAS];
char tabuleiro[DIM][DIM];
int pontuacao = 1000;

int main() {
    int dificuldade;
    exibir_menu();
    dificuldade = escolher_dificuldade();
    carregar_palavras(dificuldade);
    jogar(dificuldade);
    return 0;
}

void inicializar_tabuleiro(char tabuleiro[DIM][DIM]) {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            tabuleiro[i][j] = '.';
        }
    }
}

void exibir_tabuleiro(char tabuleiro[DIM][DIM]) {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void exibir_menu() {
    printf("Bem-vindo ao jogo de palavras cruzadas!\n");
    printf("Escolha o nível de dificuldade:\n");
    printf("1. Fácil\n");
    printf("2. Médio\n");
}

int escolher_dificuldade() {
    int escolha;
    printf("Digite sua escolha (1 para Fácil, 2 para Médio): ");
    scanf("%d", &escolha);
    while (escolha < 1 || escolha > 2) {
        printf("Escolha inválida, tente novamente: ");
        scanf("%d", &escolha);
    }
    return escolha;
}

void carregar_palavras(int dificuldade) {
    memset(palavras, 0, sizeof(palavras));
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
            strcpy(palavras[i].palavra, palavras_facil[i]);
            strcpy(palavras[i].pista, pistas_facil[i]);
            strcpy(palavras[i].pista_extra, pistas_extras_facil[i]);
            palavras[i].x = posicoes_x[i];
            palavras[i].y = posicoes_y[i];
            palavras[i].direcao = direcoes[i];
            palavras[i].ativa = 1;
            palavras[i].tentativas = 0;
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
        int posicoes_x[NUM_PALAVRAS] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4};
        int posicoes_y[NUM_PALAVRAS] = {0, 2, 0, 2, 0, 2, 0, 2, 0, 2};
        char direcoes[NUM_PALAVRAS] = {'V', 'H', 'V', 'H', 'V', 'H', 'V', 'H', 'V', 'H'};
        for (int i = 0; i < NUM_PALAVRAS; i++) {
            strcpy(palavras[i].palavra, palavras_medio[i]);
            strcpy(palavras[i].pista, pistas_medio[i]);
            strcpy(palavras[i].pista_extra, pistas_extras_medio[i]);
            palavras[i].x = posicoes_x[i];
            palavras[i].y = posicoes_y[i];
            palavras[i].direcao = direcoes[i];
            palavras[i].ativa = 1;
            palavras[i].tentativas = 0;
        }
    }
}

int verificar_palavra(char *palavra, int x, int y, char direcao) {
    int len = strlen(palavra);
    if (direcao == 'H') {
        if (y + len > DIM) return 0; // Verifica se a palavra não ultrapassa os limites do tabuleiro horizontalmente
        for (int i = 0; i < len; i++) {
            if (tabuleiro[x][y + i] != '.' && tabuleiro[x][y + i] != palavra[i]) {
                return 0; // Verifica conflito com outras palavras no tabuleiro
            }
        }
    } else { // direcao == 'V'
        if (x + len > DIM) return 0; // Verifica se a palavra não ultrapassa os limites do tabuleiro verticalmente
        for (int i = 0; i < len; i++) {
            if (tabuleiro[x + i][y] != '.' && tabuleiro[x + i][y] != palavra[i]) {
                return 0; // Verifica conflito com outras palavras no tabuleiro
            }
        }
    }
    return 1; // Retorna sucesso se não houver conflitos e a palavra couber no tabuleiro
}

void inserir_palavra(char *palavra, int x, int y, char direcao) {
    int len = strlen(palavra);
    if (direcao == 'H') {
        for (int i = 0; i < len; i++) {
            tabuleiro[x][y + i] = palavra[i]; // Insere a palavra horizontalmente
        }
    } else {
        for (int i = 0; i < len; i++) {
            tabuleiro[x + i][y] = palavra[i]; // Insere a palavra verticalmente
        }
    }
}

void jogar(int dificuldade) {
    char palavra_usuario[20];
    int indice_palavra = -1;
    int jogo_ativo = 1;

    inicializar_tabuleiro(tabuleiro);
    while (jogo_ativo) {
        jogo_ativo = 0;
        for (int i = 0; i < NUM_PALAVRAS; i++) {
            if (palavras[i].ativa) {
                printf("%d. Pista: %s\n", i, palavras[i].pista);
                jogo_ativo = 1;
            }
        }

        if (jogo_ativo) {
            printf("Selecione o número da palavra que deseja resolver: ");
            scanf("%d", &indice_palavra);
            while (indice_palavra < 0 || indice_palavra >= NUM_PALAVRAS || !palavras[indice_palavra].ativa) {
                printf("Seleção inválida, tente novamente: ");
                scanf("%d", &indice_palavra);
            }

            printf("Insira sua palavra (use até 19 caracteres): ");
            scanf("%19s", palavra_usuario);

            if (verificar_palavra(palavra_usuario, palavras[indice_palavra].x, palavras[indice_palavra].y, palavras[indice_palavra].direcao)) {
                inserir_palavra(palavra_usuario, palavras[indice_palavra].x, palavras[indice_palavra].y, palavras[indice_palavra].direcao);
                palavras[indice_palavra].ativa = 0;
                printf("Palavra correta! Pontuação: %d\n", pontuacao);
            } else {
                palavras[indice_palavra].tentativas++;
                printf("Palavra incorreta!\n");
                if (palavras[indice_palavra].tentativas % 3 == 0) { // A cada 3 tentativas erradas
                    char resposta;
                    printf("Deseja receber uma dica extra? (s/n): ");
                    scanf(" %c", &resposta);
                    if (resposta == 's' || resposta == 'S') {
                        printf("Dica extra: %s\n", palavras[indice_palavra].pista_extra);
                    }
                }
            }

            exibir_tabuleiro(tabuleiro);
        }
    }

    printf("Parabéns, você resolveu todas as palavras!\n");
}
