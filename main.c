/*{ BIBLIOTECAS }*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "stackbib.h"

/*{ CONSTANTES }*/
#define TAMANHO_CIDADE_LINHA 28  // Define o número de linhas da matriz que representa a cidade
#define TAMANHO_CIDADE_COLUNA 37 // Define o número de colunas da matriz que representa a cidade
#define QTD_ESTRADAS 17          // Define a quantidade de estradas na cidade
#define QTD_CARROS 1             // Define a quantidade de carros
#define QTD_SEMAFAROS 96         // Define a quantidade de semafaros

/*{ ESTRUTURAS }*/
// Definição da estrutura que representa um carro.
typedef struct
{
    int id;       // Identificador do carro
    int x, y;     // Coordenadas X e Y na cidade (matriz)
    int viaDupla; // Indica se o carro está em uma pista dupla e guarda qual o sentido da via
    Pilha movimentos;  // Pilha que guarda os movimentos do carro
} Carro;

// Definição da estrutura para um semáforo.
typedef struct
{
    int x, y;           // Coordenadas X e Y do semáforo na matriz
    int tempo_verde;    // Duração do sinal verde
    int tempo_vermelho; // Duração do sinal vermelho
    int contador;       // Contador que rastreia o tempo que o semáforo está em cada estado
    bool estado_verde;  // Estado do semáforo: true para verde, false para vermelho
} Semaforo;

// Definição da estrutura para estradas.
typedef struct
{
    bool direcao; // 0 para vertical, 1 para horizontal
    bool sentido; // 0 para direita/cima, 1 para esquerda/baixo
    bool ehDupla; // Indica se é uma pista dupla
    int ini;      // Ponto inicial da estrada (linha ou coluna, dependendo da direção)
} Estrada;

/*{ FUNÇÕES }*/
// Função que inicializa a matriz da cidade, colocando pontos (.) em todas as posições e definindo as estradas com seus respectivos símbolos.
// Parâmetros:
// - matriz: matriz representando a cidade
void inicializarMatriz(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    for (int i = 0; i < TAMANHO_CIDADE_LINHA; i++)
        for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++) 
            matriz[i][j] = '.'; // Inicializa todas as posições com um ponto (vazio)

    // Configuração das estradas
    Estrada estradas[QTD_ESTRADAS] = {
        {1, 0, 0, 0}, {1, 1, 0, 27}, {0, 1, 0, 36}, {0, 0, 0, 0}, // Estradas nas extremidades
        {0, 0, 1, 4}, {0, 0, 1, 8}, {0, 0, 1, 20}, {0, 0, 1, 24}, {0, 0, 1, 28}, {0, 0, 1, 32}, // Estradas verticais
        {1, 0, 0, 3}, {1, 0, 0, 6}, {1, 0, 1, 12}, {1, 0, 0, 15}, {1, 1, 0, 18}, {1, 1, 0, 21}, {1, 1, 0, 24}  // Estradas horizontais
    };

    // Desenho das estradas na matriz, com símbolos diferentes dependendo da direção e sentido
    for (int i = 0; i < QTD_ESTRADAS; i++) 
    {
        if (estradas[i].direcao) 
        { // Se a estrada é horizontal
            for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++) 
                matriz[estradas[i].ini][j] = estradas[i].ehDupla ? '-' : (estradas[i].sentido ? '<' : '>');
        } 
        else 
        { // Se a estrada é vertical
            for (int j = 0; j < TAMANHO_CIDADE_LINHA; j++) 
                matriz[j][estradas[i].ini] = estradas[i].ehDupla ? '|' : (estradas[i].sentido ? 'V' : '^');
        }
    }

    // Marca os vértices (pontos de conexão das estradas)
    matriz[0][0] = 'v';
    matriz[0][TAMANHO_CIDADE_COLUNA - 1] = 'v';
    matriz[TAMANHO_CIDADE_LINHA - 1][0] = 'v';
    matriz[TAMANHO_CIDADE_LINHA - 1][TAMANHO_CIDADE_COLUNA - 1] = 'v';
}

// Atualiza a matriz da cidade com as posições atuais dos carros e semáforos
// Parâmetros:
// - matriz: matriz representando a cidade
// - carros: array de carros contendo as suas respectivas posições
// - semaforos: array de semáforos com suas posições e estados
void atualizarMatriz(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], Carro *carros, Semaforo *semaforos)
{
    // Limpa a matriz das posições anteriores, mantendo apenas as ruas
    for (int i = 0; i < TAMANHO_CIDADE_LINHA; i++)
        for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++)
            if (matriz[i][j] != '|' && matriz[i][j] != '-' && matriz[i][j] != 'V' && matriz[i][j] != '>' && matriz[i][j] != '<' && matriz[i][j] != 'v' && matriz[i][j] != '^')
                matriz[i][j] = '.';

    // Atualiza as posições dos semáforos com base no estado (verde ou vermelho)
    for (int i = 0; i < QTD_SEMAFAROS; i++)
        matriz[semaforos[i].x][semaforos[i].y] = semaforos[i].estado_verde ? 'G' : 'R';

    // Atualiza as posições dos carros na matriz
    for (int i = 0; i < QTD_CARROS; i++)
        matriz[carros[i].x][carros[i].y] = 'C';
}

// Imprime a matriz da cidade no terminal com carros, semáforos e ruas
// Parâmetros:
// - matriz: matriz representando a cidade
void imprimirMatriz(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], Semaforo *semaforos)
{
    for (int i = 0; i < TAMANHO_CIDADE_LINHA; i++)
    {
        for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++)
        {
            char celula = matriz[i][j];
            if (celula == 'C')
                printf("🚗 "); // Representa um carro
            else if (celula == 'v')
                printf("● "); // Representa um vertice
            else if (celula == 'G')
                printf("🟢 "); // Representa um semáforo verde
            else if (celula == 'R')
                if (!semaforos[i].estado_verde && semaforos[i].contador < (semaforos[i].tempo_vermelho)/2)
                    printf("🟡 "); // Representa um semáforo amarelo
                else
                    printf("🔴 "); // Representa um semáforo vermelho
            else
                printf("%c ", celula); // Qualquer outra célula
        }

        printf("\n");
    }

    printf("\n");
}

// Verifica se uma posição específica está ocupada por um carro (FUNÇÃO NAO TESTADA)
// Parâmetros:
// - x, y: coordenadas da posição
// - carros: array de carros
// Retorno:
// - true se a posição estiver ocupada por mais de um carro, false caso contrário
bool posicaoOcupada(int x, int y, Carro *carros)
{
    int count = 0;

    for (int i = 0; i < QTD_CARROS; i++)
    {
        if (carros[i].x == x && carros[i].y == y) 
            count++;

        if (count > 1)
            return true;
    }

    return false;
}

// Tenta desviar um carro para uma nova posição se a posição atual estiver ocupada (FUNÇÃO NAO TESTADA)
// Parâmetros:
// - carro: ponteiro para o carro que tentará desviar
// - carros: array de carros
// Retorno:
// - true se o carro conseguir desviar, false caso contrário
bool desviar(Carro *carro, Carro *carros)
{
    int possiveis_desvios[4][2] = {
        {carro->x + 1, carro->y},
        {carro->x - 1, carro->y},
        {carro->x, carro->y + 1},
        {carro->x, carro->y - 1}
    };

    for (int i = 0; i < 4; i++)
    {
        int novo_x = possiveis_desvios[i][0];
        int novo_y = possiveis_desvios[i][1];

        if (novo_x >= 0 && novo_x < TAMANHO_CIDADE_LINHA && novo_y >= 0 && novo_y < TAMANHO_CIDADE_COLUNA && !posicaoOcupada(novo_x, novo_y, carros))
        {
            carro->x = novo_x;
            carro->y = novo_y;
            return true;
        }
    }
    return false;
}

// Atualiza o estado dos semáforos com base no tempo
// Parâmetros:
// - semaforos: array de semáforos
void atualizarSemaforos(Semaforo *semaforos)
{
    for (int i = 0; i < QTD_SEMAFAROS; i++)
    {
        semaforos[i].contador++;
        if (semaforos[i].estado_verde && semaforos[i].contador >= semaforos[i].tempo_verde)
        {
            semaforos[i].estado_verde = false;
            semaforos[i].contador = 0;
        }
        else if (!semaforos[i].estado_verde && semaforos[i].contador >= semaforos[i].tempo_vermelho)
        {
            semaforos[i].estado_verde = true;
            semaforos[i].contador = 0;
        }
    }
}

// Função que vai preencher cada pilha de carros. Não esta completa, a função so coloca 4 movimentos iguais para cada carro, ela foi feita para continuar a logica do programa
// Parâmetros:
// - movimentos: pilha de movimentos
void preencher(Pilha *movimentos)
{
    push(movimentos, '^');
    push(movimentos, 'V');
    push(movimentos, '>');
    push(movimentos, '<');
}

// Move um carro de acordo com sua velocidade e o estado dos semáforos
// Parâmetros:
// - carro: ponteiro para o carro que tentará desviar
// - carros: array de carros
// - semaforos: array de semáforos
// - matriz: matriz representando a cidade
void moverCarro(Carro *carro, Carro *carros, Semaforo *semaforos, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    // Se a nova posição não está ocupada, move o carro; caso contrário, tenta desviar
    if (!posicaoOcupada(carro->x, carro->y, carros))
    {
        // Se a pilha de movimentos estiver vazia, preenche-a com novos movimentos (que esta fixo so para continuar a logica)
        if (isEmpty(&carro->movimentos))
            preencher(&carro->movimentos);

        int breakwhile = 0;
        
        // Loop para determinar o próximo movimento do carro com base na pilha de movimentos
        while (1)
        {
            if (isEmpty(&carro->movimentos))
                preencher(&carro->movimentos);

            // Realiza o movimento com base no valor retirado da pilha
            switch (pop(&carro->movimentos))
            {
                case 94: // '^' - Mover para cima
                    if (matriz[carro->x - 1][carro->y] == 'R' && // Se o proximo movimento for um sinal vermelho E
                       (matriz[carro->x + 1][carro->y] == '^' || // O movimento adjascente for para cima OU
                       (matriz[carro->x + 1][carro->y] == '|' && carro->viaDupla == 94))) // O movimento adjascente for uma via dupla E a via que o carro esta é para cima
                    {
                        breakwhile = 1;
                        break;
                    }

                    if (matriz[carro->x - 1][carro->y] == '|' || // Se o proximo movimento for uma via dupla OU
                    (matriz[carro->x - 1][carro->y] == 'G' && matriz[carro->x + 1][carro->y] == '|')) // O proximo movimento for um sinal verde E o anterior uma via dupla
                    {
                        // Se o carro não estava numa via dupla coloca o sentido dela
                        if (carro->viaDupla == 0)
                            carro->viaDupla = 94;
                        
                        // Caso o sentido da via dupla seja pra cima
                        if (carro->viaDupla == 94)
                        {
                            if(posicaoOcupada((carro->x) - 1,  carro->y, carros)){
                                desviar(carro, carros);// se a posição pra onde o carro está tentando ir tiver outro carro, tento um possível desvio 
                            }
                            else
                            (carro->x)--; // Andando para cima
                            breakwhile = 1;
                        }

                        break;
                    }

                    if ((carro->x != 0 && matriz[carro->x - 1][carro->y] == '^') ||
                        (matriz[carro->x - 1][carro->y] == 'G' && matriz[carro->x + 1][carro->y] == '^') ||
                        (matriz[carro->x - 1][carro->y] == 'v' && matriz[carro->x + 1][carro->y] == '^'))
                    {
                        carro->viaDupla = 0; // O carro não esta numa vida 
                        if(posicaoOcupada((carro->x) - 1, carro->y, carros)){
                                desviar(carro, carros); // se a posição pra onde o carro está tentando ir tiver outro carro, tento um possível desvio 
                            }
                        else
                        (carro->x)--; // Andando para cima
                        breakwhile = 1;
                    }
                    break;
                // A partir daqui a logica se repete, apenas alterando o sinal
                case 86: // 'V' - Mover para baixo
                    if (matriz[carro->x + 1][carro->y] == 'R' &&
                       (matriz[carro->x - 1][carro->y] == 'V' ||
                       (matriz[carro->x - 1][carro->y] == '|' && carro->viaDupla == 86))) 
                    {
                        breakwhile = 1;
                        break;
                    }

                    if (matriz[carro->x + 1][carro->y] == '|' || 
                        (matriz[carro->x + 1][carro->y] == 'G' && matriz[carro->x - 1][carro->y] == '|')) 
                    {
                        if (carro->viaDupla == 0)
                            carro->viaDupla = 86;
                        if (carro->viaDupla == 86)
                        {
                            if(posicaoOcupada((carro->x) + 1, carro->y, carros)){
                                desviar(carro, carros);// se a posição pra onde o carro está tentando ir tiver outro carro, tento um possível desvio 
                            }
                            else
                            (carro->x)++;
                            breakwhile = 1;
                        }
                        break;
                    }

                    if ((carro->x != 36 && matriz[carro->x + 1][carro->y] == 'V') ||
                        (matriz[carro->x + 1][carro->y] == 'G' && matriz[carro->x - 1][carro->y] == 'V') ||
                        (matriz[carro->x + 1][carro->y] == 'v' && matriz[carro->x - 1][carro->y] == 'V'))
                    {
                        carro->viaDupla = 0;
                        if(posicaoOcupada((carro->x) + 1, carro->y, carros)){
                                desviar(carro, carros);// se a posição pra onde o carro está tentando ir tiver outro carro, tento um possível desvio 
                            }
                        else
                        (carro->x)++;
                        breakwhile = 1;
                    }
                    break;
                case 62: // '>' - Mover para direita
                    if (matriz[carro->x][carro->y + 1] == 'R' && 
                       (matriz[carro->x][carro->y - 1] == '>' || 
                       (matriz[carro->x][carro->y - 1] == '-' && carro->viaDupla == 62))) 
                    {
                        breakwhile = 1;
                        break;
                    }

                    if (matriz[carro->x][carro->y + 1] == '-')
                    {
                        if (carro->viaDupla == 0)
                            carro->viaDupla = 62;
                        if (carro->viaDupla == 62)
                        {
                            if(posicaoOcupada((carro->x), carro->y +1, carros)){
                                desviar(carro, carros);// se a posição pra onde o carro está tentando ir tiver outro carro, tento um possível desvio 
                            }
                        else
                            (carro->y)++;
                            breakwhile = 1;
                        }
                        break;
                    }

                    if ((carro->y != 27 && matriz[carro->x][carro->y + 1] == '>') ||
                        (matriz[carro->x][carro->y + 1] == 'G' && matriz[carro->x][carro->y - 1] == '>') ||
                        (matriz[carro->x][carro->y + 1] == 'v' && matriz[carro->x][carro->y - 1] == '>'))
                    {
                        carro->viaDupla = 0;
                        if(posicaoOcupada((carro->x), carro->y + 1, carros)){
                                desviar(carro, carros);// se a posição pra onde o carro está tentando ir tiver outro carro, tento um possível desvio 
                            }
                        else
                        (carro->y)++;
                        breakwhile = 1;
                    }
                    break;
                case 60: // '<' - Mover para esquerda
                    if (matriz[carro->x][carro->y - 1] == 'R' && 
                       (matriz[carro->x][carro->y + 1] == '<' || 
                       (matriz[carro->x][carro->y + 1] == '-' && carro->viaDupla == 60))) 
                    {
                        breakwhile = 1;
                        break;
                    }

                    if (matriz[carro->x][carro->y - 1] == '-')
                    {
                        if (carro->viaDupla == 0)
                            carro->viaDupla = 60;
                        if (carro->viaDupla == 60)
                        {
                            if(posicaoOcupada((carro->x), carro->y - 1, carros)){
                                desviar(carro, carros);// se a posição pra onde o carro está tentando ir tiver outro carro, tento um possível desvio 
                            }
                            else
                            (carro->y)--;
                            breakwhile = 1;
                        }
                        break;
                    }

                    if ((carro->y != 0 && matriz[carro->x][carro->y - 1] == '<') ||
                        (matriz[carro->x][carro->y - 1] == 'G' && matriz[carro->x][carro->y + 1] == '<') ||
                        (matriz[carro->x][carro->y - 1] == 'v' && matriz[carro->x][carro->y + 1] == '<'))
                    {
                        carro->viaDupla = 0;
                        if(posicaoOcupada((carro->x), carro->y - 1, carros)){
                                desviar(carro, carros);// se a posição pra onde o carro está tentando ir tiver outro carro, tento um possível desvio 
                            }
                        else
                        (carro->y)--;
                        breakwhile = 1;
                    }
                    break;
                default:
                    break;
            }

            if (breakwhile)
                break;
        }
    }
    else
    {
        // Lembrando que a logica de desviar nao foi testada
        if (!desviar(carro, carros))
            printf("Carro %d não conseguiu se mover e permanece na posição (%d, %d).\n", carro->id, carro->x, carro->y);
    }
}

// Simula a movimentação dos carros e o estado dos semáforos por um determinado tempo
// Parâmetros:
// - carros: array de carros
// - semaforos: array de semáforos
// - tempo_simulacao: tempo em segundos da simulação
void simularCarros(Carro *carros, Semaforo *semaforos, int tempo_simulacao)
{
    srand(time(0)); // Inicializa o gerador de números aleatórios

    while (true)
    {
        atualizarSemaforos(semaforos);                              // Atualiza os semáforos

        char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA];
        inicializarMatriz(matriz);                                  // Inicializa a matriz com pontos e vértices
        atualizarMatriz(matriz, carros, semaforos);                 // Atualiza a matriz com carros e semáforos

        for (int i = 0; i < QTD_CARROS; i++)
            moverCarro(&carros[i], carros, semaforos, matriz);      // Move cada carro

        system("clear");        // Limpa o terminal
        imprimirMatriz(matriz, semaforos); // Imprime a matriz
        sleep(1);               // Aguarda 1 segundo antes de atualizar novamente
    }
}

int main()
{
    Carro carros[QTD_CARROS] = {
        {1, 0, 1, 0}
    };

    initPilha(&carros[0].movimentos);
    preencher(&carros[0].movimentos);

    // Inicializa os semáforos
    Semaforo semaforos[] = {
        {0, 4, 4, 3, 0, true},
        {0, 8, 4, 2, 0, false},
        {0, 12, 4, 2, 0, false},
        {0, 16, 4, 2, 0, false},
        {0, 20, 4, 2, 0, false},
        {0, 24, 4, 2, 0, false},
        {0, 28, 4, 2, 0, false},
        {0, 32, 4, 2, 0, false},

        {3, 0, 4, 3, 0, true},
        {3, 8, 4, 2, 0, false},
        {3, 4, 4, 2, 0, false},
        {3, 12, 4, 2, 0, false},
        {3, 16, 4, 2, 0, false},
        {3, 20, 4, 2, 0, false},
        {3, 24, 4, 2, 0, false},
        {3, 28, 4, 2, 0, false},
        {3, 32, 4, 2, 0, false},
        {3, 36, 4, 2, 0, false},

        {6, 0, 4, 3, 0, true},
        {6, 8, 4, 2, 0, false},
        {6, 4, 4, 2, 0, false},
        {6, 12, 4, 2, 0, false},
        {6, 16, 4, 2, 0, false},
        {6, 20, 4, 2, 0, false},
        {6, 24, 4, 2, 0, false},
        {6, 28, 4, 2, 0, false},
        {6, 32, 4, 2, 0, false},
        {6, 36, 4, 2, 0, false},

        {9, 0, 4, 3, 0, true},
        {9, 8, 4, 2, 0, false},
        {9, 4, 4, 2, 0, false},
        {9, 12, 4, 2, 0, false},
        {9, 16, 4, 2, 0, false},
        {9, 20, 4, 2, 0, false},
        {9, 24, 4, 2, 0, false},
        {9, 28, 4, 2, 0, false},
        {9, 32, 4, 2, 0, false},
        {9, 36, 4, 2, 0, false},

        {12, 0, 4, 3, 0, true},
        {12, 4, 4, 2, 0, false},
        {12, 8, 4, 2, 0, false},
        {12, 12, 4, 2, 0, false},
        {12, 16, 4, 2, 0, false},
        {12, 20, 4, 2, 0, false},
        {12, 24, 4, 2, 0, false},
        {12, 28, 4, 2, 0, false},
        {12, 32, 4, 2, 0, false},
        {12, 36, 4, 2, 0, false},

        {15, 0, 4, 3, 0, true},
        {15, 4, 4, 2, 0, false},
        {15, 8, 4, 2, 0, false},
        {15, 12, 4, 2, 0, false},
        {15, 16, 4, 2, 0, false},
        {15, 20, 4, 2, 0, false},
        {15, 24, 4, 2, 0, false},
        {15, 28, 4, 2, 0, false},
        {15, 32, 4, 2, 0, false},
        {15, 36, 4, 2, 0, false},

        {18, 0, 4, 3, 0, true},
        {18, 4, 4, 2, 0, false},
        {18, 8, 4, 2, 0, false},
        {18, 12, 4, 2, 0, false},
        {18, 16, 4, 2, 0, false},
        {18, 20, 4, 2, 0, false},
        {18, 24, 4, 2, 0, false},
        {18, 28, 4, 2, 0, false},
        {18, 32, 4, 2, 0, false},
        {18, 36, 4, 2, 0, false},

        {21, 0, 4, 3, 0, true},
        {21, 4, 4, 2, 0, false},
        {21, 8, 4, 2, 0, false},
        {21, 12, 4, 2, 0, false},
        {21, 16, 4, 2, 0, false},
        {21, 20, 4, 2, 0, false},
        {21, 24, 4, 2, 0, false},
        {21, 28, 4, 2, 0, false},
        {21, 32, 4, 2, 0, false},
        {21, 36, 4, 2, 0, false},

        {24, 0, 4, 2, 0, false},
        {24, 4, 4, 2, 0, false},
        {24, 8, 4, 2, 0, false},
        {24, 12, 4, 2, 0, false},
        {24, 16, 4, 2, 0, false},
        {24, 20, 4, 2, 0, false},
        {24, 24, 4, 2, 0, false},
        {24, 28, 4, 2, 0, false},
        {24, 32, 4, 2, 0, false},
        {24, 36, 4, 2, 0, false},

        {27, 4, 4, 2, 0, false},
        {27, 8, 4, 2, 0, false},
        {27, 12, 4, 2, 0, false},
        {27, 16, 4, 2, 0, false},
        {27, 20, 4, 2, 0, false},
        {27, 24, 4, 2, 0, false},
        {27, 28, 4, 2, 0, false},
        {27, 32, 4, 2, 0, false},
    };

    int tempo_simulacao = 30;                          // Tempo total de simulação em segundos
    simularCarros(carros, semaforos, tempo_simulacao); // Inicia a simulação

    return 0;
}