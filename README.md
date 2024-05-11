# Jogo de Palavras Cruzadas

## Descrição
Este projeto é um jogo de palavras cruzadas desenvolvido em C, criado como uma forma de aplicar os conceitos aprendidos na disciplina de Algoritmos e Estruturas de Dados.O principal objetivo do jogo é completar a grade corretamente usando as pistas fornecidas, com cada palavra correta ajudando a desvendar palavras adjacentes, assim, ao completar uma grade, o jogador avança para o próximo nível.

## Estruturas de Dados Utilzadas
- Linguagem C: Usada para toda a programação e lógica do jogo.
- Lista Simplesmente Encadeada: Para armazenar palavras, permitindo inserções e remoções eficientes conforme os jogadores interagem com o jogo.
- Matriz: Representa a grade do jogo, facilitando a verificação e inserção de palavras.
  
## Algoritmo de Ordenação Utilizado
- Insertion Sort: Escolhido por sua eficiência em conjuntos de dados pequenos ou quase ordenados, ideal para as palavras no jogo que são frequentemente atualizadas à medida que os jogadores avançam.
  
## Funcionalidades
- Inserção Dinâmica de Palavras: As palavras descobertas são adicionadas à lista mantida pela estrutura de lista simplesmente encadeada.
- Remoção e Atualização de Palavras: Os jogadores podem alterar ou remover respostas anteriores.
- Verificação de Respostas: Confirma se as palavras inseridas estão corretas e se cruzam adequadamente na grade.
- Dicas: Disponíveis para jogadores que precisam de ajuda adicional.
