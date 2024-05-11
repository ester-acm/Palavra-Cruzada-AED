#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_GRADE 9
#define TAMANHO_MAX_PALAVRA 15

typedef struct Palavra {
    char palavra[TAMANHO_MAX_PALAVRA];
    int linha, coluna;
    char direcao;
    struct Palavra *proxima;
} Palavra;

typedef struct {
    Palavra *cabeca;
} ListaPalavras;

Palavra palavras_corretas[] = {{"CACHORRO", 0, 0, 'H', NULL},
                               {"AZUL", 0, 1, 'V', NULL},
                               {"SOL", 2, 0, 'H', NULL},
                               {"CANETA", 1, 4, 'V', NULL},
                               {"TERRA", 4, 0, 'H', NULL}};

const char *pistas[] = {"1. (H) Animal doméstico que late (Linha 0, Coluna 0)",
                        "2. (V) Cor do céu em um dia claro (Linha 0, Coluna 1)",
                        "3. (H) Luz do dia (Linha 2, Coluna 0)",
                        "4. (V) Objeto usado para escrever (Linha 1, Coluna 4)",
                        "5. (H) Planeta em que vivemos (Linha 4, Coluna 0)"};

ListaPalavras *criar_lista_palavras() {
    ListaPalavras *lista = (ListaPalavras *)malloc(sizeof(ListaPalavras));
    lista->cabeca = NULL;
    return lista;
}

void adicionar_palavra(ListaPalavras *lista, const char *palavra, int linha,
                       int coluna, char direcao) {
    Palavra *nova_palavra = (Palavra *)malloc(sizeof(Palavra));
    strcpy(nova_palavra->palavra, palavra);
    nova_palavra->linha = linha;
    nova_palavra->coluna = coluna;
    nova_palavra->direcao = direcao;
    nova_palavra->proxima = lista->cabeca;
    lista->cabeca = nova_palavra;
}

void remover_palavra(ListaPalavras *lista, const char *palavra) {
    Palavra *atual = lista->cabeca, *anterior = NULL;
    while (atual != NULL && strcmp(atual->palavra, palavra) != 0) {
        anterior = atual;
        atual = atual->proxima;
    }
    if (atual == NULL) {
        printf("\nPalavra não encontrada na lista.\n");
        return;
    }
    if (anterior == NULL)
        lista->cabeca = atual->proxima;
    else
        anterior->proxima = atual->proxima;
    free(atual);
}

void exibir_lista_palavras(ListaPalavras *lista) {
    Palavra *atual = lista->cabeca;
    printf("\nPalavras descobertas:\n");
    if (atual == NULL)
        printf("Nenhuma palavra descoberta ainda.\n");
    while (atual != NULL) {
        printf("%s - Linha %d, Coluna %d, Direção %c\n", atual->palavra,
               atual->linha, atual->coluna, atual->direcao);
        atual = atual->proxima;
    }
}

void inicializar_grade(char grade[TAMANHO_GRADE][TAMANHO_GRADE]) {
    for (int i = 0; i < TAMANHO_GRADE; i++)
        for (int j = 0; j < TAMANHO_GRADE; j++)
            grade[i][j] = '.';
}

void preencher_grade(char grade[TAMANHO_GRADE][TAMANHO_GRADE],
                     ListaPalavras *lista) {
    inicializar_grade(grade);
    Palavra *atual = lista->cabeca;
    while (atual != NULL) {
        int linha = atual->linha, coluna = atual->coluna;
        if (atual->direcao == 'H')
            for (int i = 0; i < strlen(atual->palavra); i++)
                grade[linha][coluna + i] = atual->palavra[i];
        else
            for (int i = 0; i < strlen(atual->palavra); i++)
                grade[linha + i][coluna] = atual->palavra[i];
        atual = atual->proxima;
    }
}

void exibir_grade(char grade[TAMANHO_GRADE][TAMANHO_GRADE]) {
    printf("\n  0 1 2 3 4\n");
    for (int i = 0; i < TAMANHO_GRADE; i++) {
        printf("%d ", i);
        for (int j = 0; j < TAMANHO_GRADE; j++)
            printf("%c ", grade[i][j]);
        printf("\n");
    }
}

void converter_para_maiusculas(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int verificar_palavra(char grade[TAMANHO_GRADE][TAMANHO_GRADE],
                      ListaPalavras *lista, const char *palavra, int linha,
                      int coluna, char direcao) {
    for (int i = 0; i < sizeof(palavras_corretas) / sizeof(palavras_corretas[0]);
         i++) {
        if (strcmp(palavras_corretas[i].palavra, palavra) == 0 &&
            palavras_corretas[i].linha == linha &&
            palavras_corretas[i].coluna == coluna &&
            palavras_corretas[i].direcao == direcao) {
            adicionar_palavra(lista, palavra, linha, coluna, direcao);
            preencher_grade(grade, lista);
            return 1;
        }
    }
    return 0;
}

void dica(char grade[TAMANHO_GRADE][TAMANHO_GRADE], ListaPalavras *lista) {
    for (int i = 0; i < sizeof(palavras_corretas) / sizeof(palavras_corretas[0]);
         i++) {
        int found = 0;
        for (Palavra *atual = lista->cabeca; atual != NULL;
             atual = atual->proxima) {
            if (strcmp(atual->palavra, palavras_corretas[i].palavra) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("\nDica: %s\n", pistas[i]);
            return;
        }
    }
    printf("\nTodas as palavras já foram descobertas.\n");
}

void insertionSort(ListaPalavras *lista) {
    if (lista->cabeca == NULL || lista->cabeca->proxima == NULL)
        return;
    Palavra *sorted = NULL;
    Palavra *current = lista->cabeca;
    while (current != NULL) {
        Palavra *next = current->proxima;
        if (sorted == NULL || strcmp(sorted->palavra, current->palavra) > 0) {
            current->proxima = sorted;
            sorted = current;
        } else {
            Palavra *s = sorted;
            while (s->proxima != NULL &&
                   strcmp(s->proxima->palavra, current->palavra) < 0)
                s = s->proxima;
            current->proxima = s->proxima;
            s->proxima = current;
        }
        current = next;
    }
    lista->cabeca = sorted;
}

int verificar_todas_palavras_encontradas(ListaPalavras *lista) {
    int count = 0;
    for (Palavra *current = lista->cabeca; current != NULL;
         current = current->proxima)
        count++;
    return count == (sizeof(palavras_corretas) / sizeof(palavras_corretas[0]));
}

void exibir_jogo(char grade[TAMANHO_GRADE][TAMANHO_GRADE], int nivel) {
    printf("\n--- Palavras Cruzadas - Nível %d ---\n", nivel);
    exibir_grade(grade);
    printf("\nPistas:\n");
    for (int i = 0; i < sizeof(pistas) / sizeof(pistas[0]); i++)
        printf("%s\n", pistas[i]);
    printf("\nOpções:\n1. Inserir palavra\n2. Remover palavra\n3. Ver lista de "
           "palavras descobertas\n4. Solicitar dica\n5. Sair do jogo\n");
}

int main() {
    char grade[TAMANHO_GRADE][TAMANHO_GRADE];
    inicializar_grade(grade);
    ListaPalavras *discovered_words = criar_lista_palavras();
    preencher_grade(grade, discovered_words);

    int level = 1;
    char word[TAMANHO_MAX_PALAVRA];
    int line, column;
    char direction;
    int choice;

    while (1) {
        exibir_jogo(grade, level);
        printf("Escolha uma opção: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\nDigite a palavra: ");
                scanf("%s", word);
                converter_para_maiusculas(word);
                printf("Digite a posição (linha coluna): ");
                scanf("%d %d", &line, &column);
                printf("Direção (H para Horizontal, V para Vertical): ");
                getchar();
                scanf("%c", &direction);
                direction = toupper(direction);
                if (verificar_palavra(grade, discovered_words, word, line, column,
                                      direction))
                    printf("\nPalavra correta!\n");
                else
                    printf("\nPalavra incorreta ou posição inválida!\n");
                preencher_grade(grade, discovered_words);
                break;
            case 2:
                printf("\nDigite a palavra a ser removida: ");
                scanf("%s", word);
                converter_para_maiusculas(word);
                remover_palavra(discovered_words, word);
                preencher_grade(grade, discovered_words);
                break;
            case 3:
                exibir_lista_palavras(discovered_words);
                break;
            case 4:
                dica(grade, discovered_words);
                break;
            case 5:
                printf("\nSaindo do jogo...\n");
                return 0;
            default:
                printf("\nOpção inválida!\n");
                break;
        }

        insertionSort(discovered_words);

        if (verificar_todas_palavras_encontradas(discovered_words)) {
            exibir_jogo(grade, level);
            printf("\n--- Parabéns! Você completou o nível %d ---\n", level);
            break;
        }
    }
    return 0;
}