#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM_FACIL 15    // Define a dimensão do tabuleiro para o nível fácil
#define DIM_MEDIO 20    // Define a dimensão do tabuleiro para o nível médio
#define NUM_PALAVRAS 10 // Define o número de palavras que serão usadas no jogo

// Estrutura que representa uma palavra do jogo (sendo um nó da lista
// simplesmente encadeada)
typedef struct Palavra {
  char palavra[20];
  char pista[100];
  char pista_extra[100];
  int x, y;     // Coordenadas iniciais da palavra no tabuleiro
  char direcao; // Direção da palavra ('H' para horizontal, 'V' para vertical)
  int ativa;    // Flag para indicar se a palavra ainda está ativa no jogo
  int tentativas; // Número de tentativas do jogador para acertar a palavra
                  // (acionar a dica extra)
  struct Palavra *prox;
} Palavra;

void inicializarTabuleiro(char **tabuleiro, int dim);
void exibirTabuleiro(char **tabuleiro, int dim);
void exibirMenu();
int escolherDificuldade();
Palavra *carregarPalavras(int dificuldade);
void jogar(Palavra *listaPalavras, int dim, int *completou);
void liberarPalavras(Palavra *head);
int verificarPalavra(char *palavra, int x, int y, char direcao,
                     char *correct_word, int dim);
int inserirPalavra(char *palavra, int x, int y, char direcao, char **tabuleiro,
                   int dim);
Palavra *inserirOrdenado(Palavra *head, Palavra *novaPalavra);

char **tabuleiro;     // Ponteiro para o tabuleiro do jogo
int pontuacao = 1000; // Pontuação inicial do jogador

int main() {
  int dificuldade;
  int dim;
  int completou = 0; // Flag para verificar se o jogador completou o jogo

  while (1) {
    exibirMenu();
    dificuldade = escolherDificuldade();

    if (dificuldade == 0) { // Se o jogador escolheu sair
      printf("Saindo do jogo...\n");
      break;
    }

    if (dificuldade == 1) { // Se o jogador escolheu a dificuldade fácil
      dim = DIM_FACIL;
    } else if (dificuldade == 2) { // Se o jogador escolheu a dificuldade média
      dim = DIM_MEDIO;
    } else {    // Se o jogador escolheu uma opção inválida
      continue; // Voltará ao início do loop
    }

    // Alocar tabuleiro dinamicamente
    tabuleiro = (char **)malloc(dim * sizeof(char *));
    for (int i = 0; i < dim; i++) {
      tabuleiro[i] = (char *)malloc(dim * sizeof(char));
    }

    inicializarTabuleiro(tabuleiro,
                         dim); // Inicializa o tabuleiro com pontos ('.')
    Palavra *listaPalavras = carregarPalavras(
        dificuldade); // Carrega as palavras de acordo com a dificuldade
    jogar(listaPalavras, dim, &completou); // Inicia o jogo
    liberarPalavras(listaPalavras); // Libera a memória alocada para as palavras

    // Liberar memória do tabuleiro
    for (int i = 0; i < dim; i++) {
      free(tabuleiro[i]);
    }
    free(tabuleiro);

    if (completou) { // Se o jogador completou todas as palavras
      printf("Parabéns, você resolveu todas as palavras!\n");
      break;
    }
  }

  // Imprimir a pontuação final
  printf("Sua pontuação final é: %d\n", pontuacao);

  return 0;
}

// Função para inicializar o tabuleiro com pontos ('.')
void inicializarTabuleiro(char **tabuleiro, int dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      tabuleiro[i][j] = '.';
    }
  }
}

// Função para exibir o tabuleiro no console
void exibirTabuleiro(char **tabuleiro, int dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      printf("%c ", tabuleiro[i][j]);
    }
    printf("\n");
  }
}

// Função para exibir o menu inicial
void exibirMenu() {
  printf("Bem-vindo ao jogo de palavras cruzadas!\n");
  printf("Escolha o nível de dificuldade:\n");
  printf("1. Fácil\n");
  printf("2. Médio\n");
  printf("0. Sair\n");
}

// Função para ler a escolha do jogador para a dificuldade
int escolherDificuldade() {
  int escolha;
  char buffer[10];

  printf("Digite sua escolha (0 para Sair, 1 para Fácil, 2 para Médio): ");
  fgets(buffer, 10, stdin);

  escolha = (int)strtol(buffer, NULL, 10); // Converte a escolha para um inteiro

  while (escolha < 0 || escolha > 2) {
    printf("Escolha inválida, tente novamente: ");
    // Solicita novamente
    fgets(buffer, 10, stdin);
    escolha = (int)strtol(buffer, NULL, 10);
  }
  return escolha;
}

// Função para criar uma nova palavra
Palavra *criarPalavra(const char *palavra, const char *pista,
                      const char *pista_extra, int x, int y, char direcao) {

  Palavra *novaPalavra = (Palavra *)malloc(sizeof(Palavra));

  if (novaPalavra == NULL) {
    printf("Erro de alocação de memória\n");
    exit(EXIT_FAILURE);
  }
  // Copulando os campos da estrutura palavra
  // strcpy por ser string
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

// Função para inserir uma palavra no tabuleiro
int inserirPalavra(char *palavra, int x, int y, char direcao, char **tabuleiro,
                   int dim) {

  int len = strlen(palavra); // Tamnaho da palavra a ser inserida no tabuleiro

  if (direcao == 'H') { // Inserção horizontal
    if (y + len > dim)
      return 0; // Palavra excede o limite do tabuleiro
    for (int i = 0; i < len; i++) {
      if (tabuleiro[x][y + i] != '.' && tabuleiro[x][y + i] != palavra[i]) {
        return 0; // Conflito encontrado (posição ocupada por outra palavra)
      }
    }

    for (int i = 0; i < len; i++) { // Se não der conflito, intera sobre a
                                    // palavra e insere no tabuleiro
      tabuleiro[x][y + i] = palavra[i];
    }

  } else if (direcao == 'V') { // Inserção vertical
    if (x + len > dim)
      return 0; // Palavra excede o limite do tabuleiro
    for (int i = 0; i < len; i++) {
      if (tabuleiro[x + i][y] != '.' && tabuleiro[x + i][y] != palavra[i]) {
        return 0; // Conflito encontrado (posição ocupada por outra palavra)
      }
    }

    for (int i = 0; i < len; i++) { // Se não der conflito, intera sobre a
                                    // palavra e insere no tabuleiro
      tabuleiro[x + i][y] = palavra[i];
    }
  }
  return 1; // Palavra inserida com sucesso
}

// Função para inserir uma nova palavra na lista de palavras em ordem alfabética
// (Insertion Sort)
Palavra *inserirOrdenado(Palavra *head, Palavra *novaPalavra) {

  if (head == NULL || strcmp(novaPalavra->palavra, head->palavra) < 0) {
    // Verifica se a lista está vazia ou se a nova palavra deve ser inserida
    // antes da primeira palavra da lista (alfabeticamente).
    novaPalavra->prox = head;
    return novaPalavra;
  }

  Palavra *atual = head;

  while (atual->prox != NULL &&
         strcmp(novaPalavra->palavra, atual->prox->palavra) >
             0) { // Percorre a lista até encontrar a posição correta onde a
                  // nova palavra deve ser inserida
    atual = atual->prox;
  }

  novaPalavra->prox = atual->prox;
  atual->prox = novaPalavra;

  return head;
}

// Função para carregar as palavras de acordo com a dificuldade escolhida
Palavra *carregarPalavras(int dificuldade) {
  Palavra *head = NULL;

  if (dificuldade == 1) { // Fácil
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

    int posicoes_x[NUM_PALAVRAS] = {2, 1, 0, 3, 2, 7, 6, 2, 6, 5};
    int posicoes_y[NUM_PALAVRAS] = {0, 1, 2, 8, 3, 5, 6, 8, 8, 1};
    char direcoes[NUM_PALAVRAS] = {'H', 'V', 'H', 'H', 'V',
                                   'H', 'V', 'H', 'H', 'H'};

    for (int i = 0; i < NUM_PALAVRAS; i++) {
      Palavra *novaPalavra = criarPalavra(palavras_facil[i], pistas_facil[i],
                                          pistas_extras_facil[i], posicoes_x[i],
                                          posicoes_y[i], direcoes[i]);
      head = inserirOrdenado(head, novaPalavra);
    }
  } else { // Médio
    char *palavras_medio[NUM_PALAVRAS] = {
        "biblioteca", "universidade", "tecnologia", "montanha", "elefante",
        "computador", "orquidea",     "revolucao",  "galaxia",  "nebulosa"};
    char *pistas_medio[NUM_PALAVRAS] = {
        "Lugar cheio de livros",
        "Centro de ensino superior",
        "Campo ligado a inovações",
        "Grande elevação natural da terra",
        "Grande mamífero com uma longa tromba",
        "Máquina para processar informações",
        "Tipo de flor que pode viver em árvores",
        "Mudança brusca e radical",
        "Conjunto enorme de estrelas e planetas",
        "Nuvem de gás no espaço"};
    char *pistas_extras_medio[NUM_PALAVRAS] = {
        "Tem muitos livros e silêncio",
        "Geralmente tem vários cursos",
        "Essencial no mundo moderno",
        "Pode ter neve no topo",
        "Tem presas grandes e pesadas",
        "Essencial para trabalhos modernos",
        "Delicada e muito apreciada por jardineiros",
        "Pode ser social, cultural ou política",
        "O sistema solar faz parte de uma",
        "Visível no céu como uma mancha luminosa"};

    int posicoes_x[NUM_PALAVRAS] = {0, 2, 4, 6, 8, 10, 12, 3, 7, 17};
    int posicoes_y[NUM_PALAVRAS] = {0, 2, 4, 6, 8, 10, 12, 1, 3, 2};
    char direcoes[NUM_PALAVRAS] = {'V', 'H', 'V', 'H', 'V',
                                   'H', 'V', 'H', 'V', 'H'};

    for (int i = 0; i < NUM_PALAVRAS;
         i++) { // Percorre todos os índices de 0 a NUM_PALAVRAS - 1
      // Para cada iteração: cria uma nova palavra com os dados fornecidos pelos
      // arrays
      Palavra *novaPalavra = criarPalavra(palavras_medio[i], pistas_medio[i],
                                          pistas_extras_medio[i], posicoes_x[i],
                                          posicoes_y[i], direcoes[i]);

      head = inserirOrdenado(
          head, novaPalavra); // Insere na lista em ordem alfabética
    }
  }
  return head;
}

// Função para verificar se a palavra inserida pelo usuário está correta
int verificarPalavra(char *palavra, int x, int y, char direcao,
                     char *correct_word, int dim) {

  int len = strlen(palavra);

  // Verificar se a palavra corresponde à palavra correta
  if (strcmp(palavra, correct_word) != 0) {
    printf("Palavra não corresponde à palavra correta.\n");
    return 0; // Palavra incorreta
  }

  // Verificações adicionais para posicionamento e conflitos
  if (direcao == 'H') {
    if (y + len > dim) {
      printf("Palavra excede os limites horizontais do tabuleiro. x=%d, y=%d, "
             "len=%d\n",
             x, y, len);
      return 0;
    }

    for (int i = 0; i < len; i++) {
      if (tabuleiro[x][y + i] != '.' && tabuleiro[x][y + i] != palavra[i]) {
        printf("Conflito encontrado na posição (%d, %d). x=%d, y=%d\n", x,
               y + i, x, y); // Se já tiver uma palavra nessa posição
        return 0;
      }
    }
  } else if (direcao == 'V') {
    if (x + len > dim) {
      printf("Palavra excede os limites verticais do tabuleiro. x=%d, y=%d, "
             "len=%d\n",
             x, y, len);
      return 0;
    }

    for (int i = 0; i < len; i++) {
      if (tabuleiro[x + i][y] != '.' && tabuleiro[x + i][y] != palavra[i]) {
        printf("Conflito encontrado na posição (%d, %d). x=%d, y=%d\n", x + i,
               y, x, y); // Se já tiver uma palavra nessa posição
        return 0;
      }
    }
  }

  return 1;
}

// Função principal do jogo onde o jogador interage para resolver as palavras
// cruzadas
void jogar(Palavra *listaPalavras, int dim, int *completou) {
  inicializarTabuleiro(tabuleiro, dim);
  int jogo_ativo = 1;       // Flag para manter o jogo ativo
  char palavra_usuario[20]; // Armazena a palavra inserida pelo usuário
  int indice_palavra,
      encontrou; // Variáveis para armazenar a seleção do usuário e se encontrou
                 // a palavra correspondente
  Palavra *palavra_atual; // Ponteiro para a palavra atual na lista
  char buffer[20];        // Buffer para leitura de entrada do usuário

  while (jogo_ativo) {
    jogo_ativo = 0;
    int contagem = 0;
    palavra_atual = listaPalavras;

    while (palavra_atual != NULL) { // Enquanto houver palavras na lista.

      if (palavra_atual->ativa) { // Se a palavra atual ainda não foi resolvida
        printf("%d. Pista: %s\n", contagem,
               palavra_atual->pista); // Exibe a pista
        jogo_ativo = 1;
      }

      palavra_atual = palavra_atual->prox;
      contagem++;
    }

    if (jogo_ativo) {
      printf("Selecione o número da palavra que deseja resolver (ou digite -1 "
             "para voltar ao menu principal): ");
      fgets(buffer, sizeof(buffer), stdin);
      sscanf(buffer, "%d", &indice_palavra); // Converte a entrada do jogador
                                             // para um número inteiro

      if (indice_palavra == -1) {
        printf("Saindo do jogo...\n");
        return;
      }

      contagem = 0;
      palavra_atual = listaPalavras;
      encontrou = 0;

      while (palavra_atual != NULL &&
             !encontrou) { // Percorre a lista até encontrar a palavra ou chegar
                           // ao final
        if (contagem == indice_palavra && palavra_atual->ativa) {
          // Verifica se a contagem atual corresponde a escolha do jogador e se
          // a palavra está ativa
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

      // Jogador inserindo a palavra
      printf("Insira sua palavra (use até 19 caracteres): ");
      fgets(buffer, sizeof(buffer), stdin);
      sscanf(buffer, "%19s",
             palavra_usuario); // Converte a entrada do jogador para uma string

      printf("Verificando palavra: %s\n", palavra_usuario); // Para depuração

      if (verificarPalavra(palavra_usuario, palavra_atual->x, palavra_atual->y,
                           palavra_atual->direcao, palavra_atual->palavra,
                           dim)) { // Verifica se a palvra é correta
        inserirPalavra(palavra_usuario, palavra_atual->x, palavra_atual->y,
                       palavra_atual->direcao, tabuleiro,
                       dim);      // Se é correta, insere no tabuleiro
        palavra_atual->ativa = 0; // Marca como palavra resolvida (inativa)
        printf("Palavra correta! Pontuação: %d\n", pontuacao);

      } else {
        palavra_atual->tentativas++; // Se errou a palavra, incremente o
                                     // contador de tentativas
        pontuacao -= 10;             // Decrementa pontos por palavra incorreta
        printf("Palavra incorreta!\n");

        if (palavra_atual->tentativas % 3 ==
            0) { // Se o número de tentativas for múltiplo de 3 oferece uma dica
                 // extra ( se errar 3 vezes, se errar 6...)
          char resposta;

          printf("Deseja receber uma dica extra? (s/n): ");
          fgets(buffer, sizeof(buffer), stdin);
          sscanf(
              buffer, " %c",
              &resposta); //  Converte a resposta do jogador para um caractere

          if (resposta == 's' || resposta == 'S') {
            printf("Dica extra: %s\n", palavra_atual->pista_extra);
          }
        }
      }

      exibirTabuleiro(tabuleiro, dim);
    }
  }

  *completou = 1; // Marca o jogo como completo
}

// Função para liberar a memória alocada para a lista de palavras
void liberarPalavras(Palavra *head) {
  Palavra *tmp;

  while (head != NULL) {
    tmp = head;
    head = head->prox;
    free(tmp);
  }
}
