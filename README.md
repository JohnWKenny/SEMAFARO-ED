# Simulador de Trânsito com Carros e Semáforos

## Resumo do Código

Este programa simula o movimento de carros em uma cidade representada por uma matriz (uma grade de linhas e colunas). Os carros se movem pelas estradas, seguindo as regras de trânsito, como parar nos semáforos vermelhos e desviar de outros carros. Cada estrada tem um sentido, e os semáforos mudam de cor entre verde e vermelho, afetando o movimento dos carros. O programa usa pilhas para armazenar os movimentos possíveis dos carros (direções).

## Explicação dos Principais Conceitos

### Cidade como uma Matriz
A cidade é representada por uma matriz (uma grade) onde cada célula é um ponto (`.`), e as estradas são indicadas por diferentes símbolos:

- `|` ou `-`: Estradas de mão dupla.
- `^`, `V`: Estradas de sentido único (subindo ou descendo).
- `<`, `>`: Estradas de sentido único (esquerda ou direita).
- `v`: Pontos de vértice nas extremidades da cidade.
- `C`: Indica a posição de um carro.
- `G`: Semáforo verde.
- `R`: Semáforo vermelho.

### Carros
Cada carro tem uma posição na matriz (X, Y), e eles se movem pelas estradas conforme a direção da via. Se um carro encontrar um semáforo vermelho ou outro carro no caminho, ele tenta parar ou desviar.

### Semáforos
Os semáforos mudam de cor conforme o tempo, alternando entre verde (`G`) e vermelho (`R`). Se o semáforo estiver vermelho, os carros devem parar; se estiver verde, eles podem seguir em frente.

### Pilha para Movimentos
A pilha é usada para armazenar as direções possíveis para o carro se mover (cima, baixo, esquerda, direita). O carro "desempilha" uma direção da pilha e tenta se mover nessa direção.

## Funções do Programa

- **inicializarMatriz**: Configura a cidade, desenhando as estradas na matriz e marcando os vértices com o símbolo `v`.
- **atualizarMatriz**: Atualiza a matriz com as novas posições dos carros e dos semáforos, limpando as posições antigas.
- **imprimirMatriz**: Mostra a matriz na tela, onde os carros são representados pelo símbolo 🚗, os semáforos por 🟢 (verde) e 🔴 (vermelho).
- **posicaoOcupada**: Verifica se uma posição específica da matriz já está ocupada por outro carro.
- **desviar**: Se um carro encontrar um obstáculo (outro carro), essa função tenta encontrar uma direção alternativa para ele seguir.
- **moverCarro**: Movimenta um carro na cidade, seguindo as regras das estradas e semáforos. Se o carro encontrar um semáforo vermelho ou outro carro, ele pode parar ou tentar desviar.
- **atualizarSemaforos**: Controla o tempo de troca dos semáforos entre verde e vermelho.
- **Pilha (Stack)**: Uma pilha é usada para armazenar os possíveis movimentos dos carros (`^` para cima, `V` para baixo, `>` para direita, `<` para esquerda). A pilha ajuda o carro a decidir para onde ir quando se move pelas estradas.

## Como o Código Funciona

1. O código primeiro cria uma matriz representando a cidade com estradas e semáforos.
2. Em seguida, atualiza a posição dos carros e verifica se eles devem parar nos semáforos ou desviar de obstáculos.
3. A cada ciclo de tempo, os semáforos mudam de cor, e os carros se movem ou ficam parados, dependendo das regras de trânsito.

Dessa forma, o programa simula o comportamento de carros em uma cidade, levando em conta semáforos e possíveis engarrafamentos.
