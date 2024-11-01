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
- `A`: Acidente
- `Q`: Semaforo com alerta

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
- **moverCarro**: Movimenta um carro na cidade, seguindo as regras das estradas e semáforos. Se o carro encontrar um semáforo vermelho ou outro carro, ele pode parar ou tentar desviar.
- **atualizarSemaforos**: Controla o tempo de troca dos semáforos entre verde e vermelho.
- **Pilha (Stack)**: Uma pilha é usada para armazenar os possíveis movimentos dos carros (`^` para cima, `V` para baixo, `>` para direita, `<` para esquerda). A pilha ajuda o carro a decidir para onde ir quando se move pelas estradas.

## Como o Código Funciona

1. O código primeiro cria uma matriz representando a cidade com estradas e semáforos.
2. Em seguida, atualiza a posição dos carros e verifica se eles devem parar nos semáforos ou desviar de obstáculos.
3. A cada ciclo de tempo, os semáforos mudam de cor, e os carros se movem ou ficam parados, dependendo das regras de trânsito.

## Como a Lógica Funciona
1. Cada carro se move conforme a pilha vai desimpilhando.
2. Caso o item que foi desempilhado não seja igual ao sentido da estrada ele deve ser ignorado.
3. Quando a pilha ficar vazia ela é preenchida com os movimentos, isso até achar qual o movimento certo a se fazer.

## Funções Principais
### posicaoOcupada
A função posicaoOcupada verifica se a próxima posição do carro está ocupada, considerando as seguintes regras:

Checagem de Carro Parado: Se a próxima posição for ocupada por um carro que está parado, a função retorna que a posição está ocupada, impedindo que o carro atual continue.

Tratamento para Vias Duplas:

Em vias duplas, o carro pode se mover caso os últimos movimentos dos carros sejam diferentes, ou seja, estão em direções opostas.

Exemplo:

`> 🚗 🟢` 
`    🚗 `
`    |`  
No exemplo acima, para o carro que está na via unica, quando ele vai para o semaforo é indica que a posição de baixo esta ocupada, por o carro esta parado, contudo, isso é uma via dupla, então o carro da via unica poderia passar, é por isso que existe o if de via dupla nessa função, se seus "ultimos_movimentos" forem diferentes então o carro pode entrar na via dupla.

### moverCarro
Esta função trata do deslocamento do carro, considerando a velocidade e a presença de semáforos.

Tratamento da Velocidade
Limite de Velocidade para Direções Diferentes:

Em vias verticais, o semáforo está posicionado a cada 3 unidades de deslocamento, enquanto nas horizontais está a cada 4 unidades.
Para manter o controle e evitar que o carro ultrapasse semáforos, a velocidade máxima é definida como:
2 unidades para vias verticais
3 unidades para vias horizontais

Restrição de Velocidade Próximo ao Semáforo:

O primeiro if na função verifica se o carro está próximo a um semáforo e, se estiver, a velocidade é reduzida para evitar que o carro passe o sinal.

o segundo if serve para verificar se ele ta passando por um carro, e se esse carro estiver num semaforo ele tem que parar antes daquela localização, olha o exemplo:

Exemplo de situação:
1º

`    | `
`> 🚗 🟢  >`
`    🚗`
`     |`
2º
`    | `
`> > 🚗  > `
`    🚗 `
`    |`
O carro de baixo percebe o carro à frente em um semáforo e reduz sua velocidade, respeitando o limite do semáforo.

Essas verificações garantem que a movimentação dos carros seja feita de maneira realista, respeitando as sinalizações e as condições de cada via.
