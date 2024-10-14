/*{ BIBLIOTECAS }*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "Structs.h"
#include "Matriz.h"
#include "Carros.h"
//#include "Semaforo.h"


/*{ CONSTANTES }*/
#define TAMANHO_CIDADE_LINHA 28  // Define o número de linhas da matriz que representa a cidade
#define TAMANHO_CIDADE_COLUNA 37 // Define o número de colunas da matriz que representa a cidade
#define QTD_ESTRADAS 17          // Define a quantidade de estradas na cidade
#define QTD_CARROS 1           // Define a quantidade de carros
#define QTD_SEMAFOROS 96         // Define a quantidade de semaforos

// Variáveis globais para as entradas dos incidentes
int EntradaIncidentes_1_x, EntradaIncidentes_1_y;
int EntradaIncidentes_2_x, EntradaIncidentes_2_y;

Estrada estradas[QTD_ESTRADAS] = {
    {1, 0, 0, 0}, {1, 1, 0, 27}, {0, 1, 0, 36}, {0, 0, 0, 0},
    {0, 0, 1, 4}, {0, 0, 1, 8}, {0, 0, 1, 20}, {0, 0, 1, 24}, {0, 0, 1, 28}, {0, 0, 1, 32},
    {1, 0, 0, 3}, {1, 0, 0, 6}, {1, 0, 1, 12}, {1, 0, 0, 15}, {1, 1, 0, 18}, {1, 1, 0, 21}, {1, 1, 0, 24}
};

void atualizarSemaforos(Semaforo *semaforos)
{
    for (int i = 0; i < QTD_SEMAFOROS; i++)
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
void addSemaforo(Semaforo semaforos[QTD_SEMAFOROS]){
    int i = 0;
        for(int indice_x = 0; indice_x <= 27; indice_x += 3)
        {
            for(int indice_y = 0; indice_y <= 36; indice_y += 4)
            {
                if((!indice_y || indice_y == 36) && (!indice_x || indice_x == 27)) continue;
    
                int t_verde = (rand() % 4) + 1;
                int t_vermelho = (rand() % 4) +     1;
                
        
                semaforos[i].x = indice_x;
                semaforos[i].y = indice_y;
                semaforos[i].tempo_verde = t_verde; //rever
                semaforos[i].tempo_vermelho = t_vermelho; //rever
                semaforos[i].contador = 0;
                semaforos[i].estado_verde = true;
                semaforos[i].alerta = false;
                
                if(indice_x >= EntradaIncidentes_1_x && indice_x <= EntradaIncidentes_2_x &&
                indice_y >= EntradaIncidentes_1_y && indice_y <= EntradaIncidentes_2_y){
                    semaforos[i].alerta = true;
                } 
                i++;
            }   
        }
}

// Simula a movimentação dos carros e o estado dos semáforos por um determinado tempo
// Parâmetros:
// - carros: array de carros
// - semaforos: array de semáforos
// - tempo_simulacao: tempo em segundos da simulação
void simularCarros(Carro *carros, Semaforo *semaforos, int tempo_simulacao)
{
    while (true)
    {
        atualizarSemaforos(semaforos);                              // Atualiza os semáforos
        char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA];   // Inicializa a Matriz
        inicializarMatriz(matriz);                                  // Inicializa a matriz com pontos e vértices
        atualizarMatriz(matriz, carros, semaforos);                 // Atualiza a matriz com carros e semáforos
        imprimirMatriz(matriz, semaforos);                          // Imprime a matriz 
        sleep(1);                                                   // Aguarda 1 segundo antes de atualizar novamente
        
        for (int i = QTD_CARROS; i >= 0; i--){
            MoverCarro(&carros[i], carros, semaforos, matriz);
        }
        system("clear"); 

    }
}

int main()
{
    printf("Digite o primeiro vertice(Coluna e Linha):\n");
    scanf("%d %d", &EntradaIncidentes_1_y, &EntradaIncidentes_1_x);
    system("clear"); 
    
    EntradaIncidentes_1_x *= 3;
    EntradaIncidentes_1_y *= 4;
    
    if(EntradaIncidentes_1_x % 3 != 0 || EntradaIncidentes_1_x < 0 || EntradaIncidentes_1_x > TAMANHO_CIDADE_LINHA||
    EntradaIncidentes_1_y % 4 != 0 || EntradaIncidentes_1_y < 0 || EntradaIncidentes_1_y > TAMANHO_CIDADE_COLUNA){
        goto erro1;
    }
    printf("Digite o segundo vertice:\n");
    scanf("%d %d", &EntradaIncidentes_2_y, &EntradaIncidentes_2_x);
    system("clear"); 
    
    EntradaIncidentes_2_x *= 3;
    EntradaIncidentes_2_y *= 4;
    
    if(EntradaIncidentes_2_x % 3 != 0 || EntradaIncidentes_2_x < 0 || EntradaIncidentes_2_x > TAMANHO_CIDADE_LINHA||
    EntradaIncidentes_2_y % 4 != 0 || EntradaIncidentes_2_y < 0 || EntradaIncidentes_2_y > TAMANHO_CIDADE_COLUNA){
        goto erro2;
    }
    if((EntradaIncidentes_1_x > EntradaIncidentes_2_x) || (EntradaIncidentes_1_y > EntradaIncidentes_2_y)) goto erro3;
    
    Carro carros[QTD_CARROS] = {
        
    };
    addCar(carros);
    Semaforo semaforos[QTD_SEMAFOROS];

    addSemaforo(semaforos);// Inicializa os semáforos
    
    int tempo_simulacao = 30;                          // Tempo total de simulação em segundos
    simularCarros(carros, semaforos, tempo_simulacao); // Inicia a simulação

    return 0;


erro1:
    printf("Vertice invalido: x = %d\ty = %d\tSemaforo Inexistente 1", EntradaIncidentes_1_x, EntradaIncidentes_1_y);
    return 1;


erro2:
    printf("Vertice invalido: x = %d\ty = %d\tSemaforo Inexistente2", EntradaIncidentes_2_x, EntradaIncidentes_2_y);
    return 2;

erro3:
    printf("Vertices invalidos: x1 = %d\ty1 = %d\t && x2 = %d\ty2 = %d\t Area Invalida", EntradaIncidentes_1_x, EntradaIncidentes_1_y, EntradaIncidentes_2_x, EntradaIncidentes_2_y);
    return 3;
}