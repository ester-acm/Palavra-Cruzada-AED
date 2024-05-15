#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 10
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

void inicializarTabuleiro(char tabuleiro[DIM][DIM]);
void exibirTabuleiro(char tabuleiro[DIM][DIM]);
void exibirMenu();
int escolherDificuldade();
Palavra *carregarPalavras(int dificuldade);
void jogar(Palavra *listaPalavras);
void liberarPalavras(Palavra *head);
int verificarPalavra(char *palavra, int x, int y, char direcao,
                     char *correct_word);
void inserirPalavra(char *palavra, int x, int y, char direcao);
Palavra *inserirOrdenado(Palavra *head, Palavra *novaPalavra);

char tabuleiro[DIM][DIM];
int pontuacao = 1000;

int main() {
  int dificuldade;
  exibirMenu();
  dificuldade = escolherDificuldade();
  Palavra *listaPalavras = carregarPalavras(dificuldade);
  jogar(listaPalavras);
  liberarPalavras(listaPalavras);
  return 0;
}

void inicializarTabuleiro(char tabuleiro[DIM][DIM]) {
  for (int i = 0; i < DIM; i++) {
    for (int j = 0; j < DIM; j++) {
      tabuleiro[i][j] = '.';
    }
  }
}

void exibirTabuleiro(char tabuleiro[DIM][DIM]) {
  for (int i = 0; i < DIM; i++) {
    for (int j = 0; j < DIM; j++) {
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
}

int escolherDificuldade() {
  int escolha;
  char buffer[10];
  printf("Digite sua escolha (1 para Fácil, 2 para Médio): ");
  fgets(buffer, 10, stdin);
  escolha = (int)strtol(buffer, NULL, 10);
  while (escolha < 1 || escolha > 2) {
    printf("Escolha inválida, tente novamente: ");
    fgets(buffer, 10, stdin);
    escolha = (int)strtol(buffer, NULL, 10);
  }
  return escolha;
}

Palavra *criarPalavra(const char *palavra, const char *pista,
                      const char *pista_extra, int x, int y, char direcao) {
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

// Function to insert the word into the board at the specified position and
// direction
void inserirPalavra(char *palavra, int x, int y, char direcao) {
  int len = strlen(palavra);
  if (direcao == 'H') { // Horizontal insertion
    for (int i = 0; i < len; i++) {
      if (y + i < DIM) { // Ensure the word does not exceed board dimensions
        tabuleiro[x][y + i] = palavra[i];
      }
    }
  } else if (direcao == 'V') { // Vertical insertion
    for (int i = 0; i < len; i++) {
      if (x + i < DIM) { // Ensure the word does not exceed board dimensions
        tabuleiro[x + i][y] = palavra[i];
      }
    }
  }
}

Palavra *inserirOrdenado(Palavra *head, Palavra *novaPalavra) {
  if (head == NULL || strcmp(novaPalavra->palavra, head->palavra) < 0) {
    novaPalavra->prox = head;
    return novaPalavra;
  }
  Palavra *atual = head;
  while (atual->prox != NULL &&
         strcmp(novaPalavra->palavra, atual->prox->palavra) > 0) {
    atual = atual->prox;
  }
  novaPalavra->prox = atual->prox;
  atual->prox = novaPalavra;
  return head;
}

Palavra *carregarPalavras(int dificuldade) {
  Palavra *head = NULL;
  char *palavras_facil[NUM_PALAVRAS] = {"casa",  "gato", "sol",  "livro",
                                        "arroz", "mesa", "teto", "chuva",
                                        "noite", "flor"};
  char *pistas_facil[NUM_PALAVRAS] = {"Lugar onde moramos",
                                      "Animal doméstico que mia",
                                      "Astro-rei",
                                      "Objeto usado para ler",
                                      "Comida básica do brasileiro",
                                      "Móvel usado para refeições",
                                      "Parte superior de um cômodo",
                                      "Precipitação comum do tempo",
                                      "Oposto de dia",
                                      "Bela e cheirosa, cresce no jardim"};
  char *pistas_extras_facil[NUM_PALAVRAS] = {
      "Tem portas e janelas",      "Famoso por ter nove vidas",
      "Aquece nosso planeta",      "Pode ser digital ou de papel",
      "Geralmente cozido em água", "Tem quatro pernas e é usada para comer",
      "Fica no alto das casas",    "Associada a um guarda-chuva",
      "Quando as estrelas saem",   "Pode ser de várias cores e aromas"};
  int posicoes_x[NUM_PALAVRAS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int posicoes_y[NUM_PALAVRAS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  char direcoes[NUM_PALAVRAS] = {'V', 'H', 'V', 'H', 'V',
                                 'H', 'V', 'H', 'V', 'H'};

  for (int i = 0; i < NUM_PALAVRAS; i++) {
    Palavra *novaPalavra =
        criarPalavra(palavras_facil[i], pistas_facil[i], pistas_extras_facil[i],
                     posicoes_x[i], posicoes_y[i], direcoes[i]);
    head = inserirOrdenado(head, novaPalavra);
  }

  return head;
}

int verificarPalavra(char *palavra, int x, int y, char direcao,
                     char *correct_word) {
  int len = strlen(palavra);

  // First check if the word matches the expected correct word
  if (strcmp(palavra, correct_word) != 0) {
    return 0; // Incorrect word
  }

  // Additional checks for placement and conflicts
  if (direcao == 'H') {
    if (y + len > DIM)
      return 0;
    for (int i = 0; i < len; i++) {
      if (tabuleiro[x][y + i] != '.' && tabuleiro[x][y + i] != palavra[i]) {
        return 0;
      }
    }
  } else if (direcao == 'V') {
    if (x + len > DIM)
      return 0;
    for (int i = 0; i < len; i++) {
      if (tabuleiro[x + i][y] != '.' && tabuleiro[x + i][y] != palavra[i]) {
        return 0;
      }
    }
  }
  return 1;
}

void jogar(Palavra *listaPalavras) {
  inicializarTabuleiro(tabuleiro);
  int jogo_ativo = 1;
  char palavra_usuario[20];
  int indice_palavra, encontrou;
  Palavra *palavra_atual;

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
      printf("Selecione o número da palavra que deseja resolver: ");
      scanf("%d", &indice_palavra);
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
      scanf("%19s", palavra_usuario);

      if (verificarPalavra(palavra_usuario, palavra_atual->x, palavra_atual->y,
                           palavra_atual->direcao, palavra_atual->palavra)) {
        inserirPalavra(palavra_usuario, palavra_atual->x, palavra_atual->y,
                       palavra_atual->direcao);
        palavra_atual->ativa = 0;
        printf("Palavra correta! Pontuação: %d\n", pontuacao);
      } else {
        palavra_atual->tentativas++;
        printf("Palavra incorreta!\n");
        if (palavra_atual->tentativas % 3 == 0) {
          char resposta;
          printf("Deseja receber uma dica extra? (s/n): ");
          scanf(" %c", &resposta);
          if (resposta == 's' || resposta == 'S') {
            printf("Dica extra: %s\n", palavra_atual->pista_extra);
          }
        }
      }

      exibirTabuleiro(tabuleiro);
    }
  }

  printf("Parabéns, você resolveu todas as palavras!\n");
}

void liberarPalavras(Palavra *head) {
  Palavra *tmp;
  while (head != NULL) {
    tmp = head;
    head = head->prox;
    free(tmp);
  }
}
