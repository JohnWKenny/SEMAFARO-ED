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

## Como a Lógica Funciona
1. Cada carro se move conforme a pilha vai desimpilhando.
2. Caso o item que foi desempilhado não seja igual ao sentido da estrada ele deve ser ignorado.
3. Quando a pilha ficar vazia ela é preenchida com os movimentos, isso até achar qual o movimento certo a se fazer.

## Problema da Lógica (recomendável olhar o código)
Seguindo a lógica original, o carro ainda pode se mover incorretamente em algumas situações. Vamos considerar o seguinte exemplo para ilustrar o problema. Suponha os seguintes símbolos:

- `🚗`: Carro
- `🟢`: Semáforo verde
- `>`: Sentido direita
- `<`: Sentido esquerda
- Desconsideraremos o sinal amarelo e vermelho.

Agora, imagine a seguinte estrada:
`🚗 > 🟢 > >`

Suponha que a pilha de direções seja `['<', '>']`, com o topo sendo `>`, e uma função arbitrária que desempilha, empilha todos os movimentos caso a pilha esteja vazía e move o carro conforme as seguintes condições:

- Se a posição à direita contiver `>` ou `🟢`, o carro se move para a direita.
- Se a posição à esquerda contiver `<` ou `🟢`, o carro se move para a esquerda.

A sequência de movimentos seria algo assim:

`> 🚗 🟢 > >`

Lembrando que a pilha ficaria `['>']`, então ele desimpilharia ele sem se mover e depois empilha todos os movimentos de volta, voltando ao original, o que implica que isso acontecerá diversas vezes.

O carro se move para a direita, ficando assim:

`> > 🚗 > >`

Ao continuar seguindo as regras, o carro novamente se move para a direita, aproximando-se do semáforo verde:

`> > 🟢 🚗 >`

Neste ponto, se a função detectar que a próxima posição à esquerda (`<`) tem um semáforo verde, o carro tentaria retornar para essa posição, resultando no seguinte:

`> > 🚗 > >`

Isso criaria um loop infinito, já que o carro continuaria a se mover entre as mesmas posições, sem alcançar um destino final.

### Solução Implementada

Para resolver esse problema, foi implementada uma verificação adicional. Agora, a função não apenas verifica a próxima posição à frente, mas também a posição adjacente (a "traseira" do carro). Com essa verificação dupla, o carro evita entrar em loops, pois a lógica impede que ele volte para uma posição já ocupada ou desfavorável. Assim, o movimento fica mais preciso e evita comportamentos erráticos.

Caso dúvidas pode-se fazer um teste, na função mover carro retire de todas as condições que verificam a posição adjacente, por exemplo, se o carro está indo para direita sua posição normal seria y + 1, sua posição adjacente seria y - 1.

### O Que Falta/Problema

- [ ] Isolamento da área desativada (Os carros ainda circulam por lá)

### ÚLTIMAS MODIFICAÇÕES

- [x] Definição de constante necessárias apenas no arquivo "Structs.h;
- [x] Arquivo "stackbib.h" removido, por redundância. (Seu conteúdo já está no arquivo "Structs.h");
- [x] Biblioteca "cJSON" adicionada para auxiliar na geração do arquivo de log;
- [x] Função de log do fluxo da cidade completo.
