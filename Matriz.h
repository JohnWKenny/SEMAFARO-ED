#ifndef Matriz_h
#define Matriz_h

#define TAMANHO_CIDADE_LINHA 28  // Define o número de linhas da matriz que representa a cidade
#define TAMANHO_CIDADE_COLUNA 37 // Define o número de colunas da matriz que representa a cidade
#define QTD_ESTRADAS 17          // Define a quantidade de estradas na cidade
#define QTD_CARROS 1           // Define a quantidade de carros
#define QTD_SEMAFOROS 96         // Define a quantidade de semaforos

#include "Structs.h"

extern int EntradaIncidentes_1_x, EntradaIncidentes_1_y;
extern int EntradaIncidentes_2_x, EntradaIncidentes_2_y;


/*{ FUNÇÕES }*/
// Função que inicializa a matriz da cidade, colocando pontos (.) em todas as posições e definindo as estradas com seus respectivos símbolos.
// Parâmetros:
// - matriz: matriz representando a cidade
void inicializarMatriz(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    for (int i = 0; i < TAMANHO_CIDADE_LINHA; i++)
        for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++)
            matriz[i][j] = '.'; // Inicializa todas as posições com um ponto (vazio)

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
    for(int x = EntradaIncidentes_1_x; x <= EntradaIncidentes_2_x; x += 3){
        for(int y = EntradaIncidentes_1_y; y <= EntradaIncidentes_2_y; y += 4){
            if(x == EntradaIncidentes_1_x){
                if(x-2>=0)
                matriz[x - 2][y] = 'A'; 
            }
            if(x == EntradaIncidentes_2_x){
                if(x+2 <= TAMANHO_CIDADE_LINHA)
                matriz[x + 2][y] = 'A'; 
            }
            if(y == EntradaIncidentes_1_y){
                if(y-3>=0)
                matriz[x][y - 3] = 'A';
            }
            if(y == EntradaIncidentes_2_y){
                if(y+3<=TAMANHO_CIDADE_COLUNA)
                matriz[x][y + 3] = 'A';
            }
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
            if (matriz[i][j] != '|' && matriz[i][j] != '-' && matriz[i][j] != 'V' && matriz[i][j] != '>' &&
            matriz[i][j] != '<' && matriz[i][j] != 'v' && matriz[i][j] != '^'&& matriz[i][j]!= 'A')
                matriz[i][j] = '.';

    // Atualiza as posições dos semáforos com base no estado (verde ou vermelho)
    for (int i = 0; i < QTD_SEMAFOROS; i++){
        matriz[semaforos[i].x][semaforos[i].y] = semaforos[i].estado_verde ? 'G' : 'R' ;
        if(semaforos[i].alerta) matriz[semaforos[i].x][semaforos[i].y] = 'Q';
    }
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
            else if (celula == 'A')
                printf("🚧 ");
            else if (celula == 'G')
                printf("🟢 "); // Representa um semáforo verde
            else if(celula == 'Q'){
                printf("⚪ ");
            }
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
#endif