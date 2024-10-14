/*{ BIBLIOTECAS }*/
#include <stdio.h>    // Biblioteca para funções de entrada e saída, como printf e scanf
#include <stdlib.h>   // Biblioteca para funções como malloc, free, rand e system
#include <stdbool.h>  // Biblioteca para usar o tipo booleano em C
#include <time.h>     // Biblioteca para manipulação de tempo, como gerar números aleatórios com base no tempo atual
#include <unistd.h>   // Biblioteca para a função sleep, usada para pausas na execução do programa
#include "Structs.h"  // Arquivo de cabeçalho para definição de structs usadas no código
#include "Matriz.h"   // Arquivo de cabeçalho para funções relacionadas à matriz da cidade
#include "Carros.h"   // Arquivo de cabeçalho para funções relacionadas aos carros
#include "Semaforos.h"// Arquivo de cabeçalho para funções relacionadas aos semáforos

/*{ CONSTANTES }*/
#define TAMANHO_CIDADE_LINHA 28  // Define o número de linhas da matriz que representa a cidade
#define TAMANHO_CIDADE_COLUNA 37 // Define o número de colunas da matriz que representa a cidade
#define QTD_ESTRADAS 17          // Define a quantidade de estradas na cidade
#define QTD_CARROS 30             // Define a quantidade de carros que estarão na simulação
#define QTD_SEMAFOROS 96         // Define a quantidade de semáforos na cidade

// Variáveis globais para as entradas dos incidentes, usadas para determinar pontos específicos na matriz
int EntradaIncidentes_1_x, EntradaIncidentes_1_y;
int EntradaIncidentes_2_x, EntradaIncidentes_2_y;
int ativarfluxo, semaforo_x, semaforo_y, direcao;


// Array de structs do tipo Estrada, que contém informações sobre as estradas da cidade
Estrada estradas[QTD_ESTRADAS] = {
    {1, 0, 0, 0}, {1, 1, 0, 27}, {0, 1, 0, 36}, {0, 0, 0, 0},
    {0, 0, 1, 4}, {0, 0, 1, 8}, {0, 0, 1, 20}, {0, 0, 1, 24}, {0, 0, 1, 28}, {0, 0, 1, 32},
    {1, 0, 0, 3}, {1, 0, 0, 6}, {1, 0, 1, 12}, {1, 0, 0, 15}, {1, 1, 0, 18}, {1, 1, 0, 21}, {1, 1, 0, 24}
};

// Função que simula a movimentação dos carros e o estado dos semáforos por um determinado tempo
// Parâmetros:
// - carros: array de carros
// - semaforos: array de semáforos
// - tempo_simulacao: tempo em segundos da simulação
void simularCarros(Carro *carros, Semaforo *semaforos, int tempo_simulacao)
{
    while (true) // Loop infinito para simulação contínua
    {
        atualizarSemaforos(semaforos);                            // Atualiza os semáforos com novos estados
        char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA]; // Cria a matriz da cidade
        inicializarMatriz(matriz);                               // Inicializa a matriz com pontos e vértices
        atualizarMatriz(matriz, carros, semaforos);              // Atualiza a matriz com a posição dos carros e semáforos
        imprimirMatriz(matriz, semaforos);                       // Imprime a matriz no terminal
        sleep(1);                                                // Aguarda 1 segundo antes de atualizar novamente
        
        // Move os carros, partindo do último até o primeiro para evitar problemas de sobreposição
        for (int i = QTD_CARROS; i >= 0; i--) {
            MoverCarro(&carros[i], carros, semaforos, matriz); // Função que move cada carro
        }
        system("clear"); // Limpa a tela do terminal para mostrar a próxima atualização da matriz
    }
}

int main()
{
    // Solicita ao usuário os vértices de entrada do primeiro incidente
    printf("Digite o primeiro vertice(Coluna e Linha):\n");
    scanf("%d %d", &EntradaIncidentes_1_y, &EntradaIncidentes_1_x);
    system("clear"); // Limpa a tela do terminal após a entrada
    
    // Ajusta os valores dos vértices para a escala da matriz (multiplicando por 3 e 4)
    EntradaIncidentes_1_x *= 3;
    EntradaIncidentes_1_y *= 4;
    
    // Verifica se os vértices de entrada são válidos (se estão dentro dos limites e múltiplos corretos)
    if (EntradaIncidentes_1_x % 3 != 0 || EntradaIncidentes_1_x < 0 || EntradaIncidentes_1_x > TAMANHO_CIDADE_LINHA ||
        EntradaIncidentes_1_y % 4 != 0 || EntradaIncidentes_1_y < 0 || EntradaIncidentes_1_y > TAMANHO_CIDADE_COLUNA) {
        goto erro1; // Redireciona para a mensagem de erro se o vértice for inválido
    }

    // Solicita ao usuário os vértices de entrada do segundo incidente
    printf("Digite o segundo vertice:(Coluna e Linha)\n");
    scanf("%d %d", &EntradaIncidentes_2_y, &EntradaIncidentes_2_x);
    system("clear"); // Limpa a tela do terminal após a entrada
    
    // Ajusta os valores dos vértices para a escala da matriz (multiplicando por 3 e 4)
    EntradaIncidentes_2_x *= 3;
    EntradaIncidentes_2_y *= 4;
    
    // Verifica se os vértices de entrada são válidos (se estão dentro dos limites e múltiplos corretos)
    if (EntradaIncidentes_2_x % 3 != 0 || EntradaIncidentes_2_x < 0 || EntradaIncidentes_2_x > TAMANHO_CIDADE_LINHA ||
        EntradaIncidentes_2_y % 4 != 0 || EntradaIncidentes_2_y < 0 || EntradaIncidentes_2_y > TAMANHO_CIDADE_COLUNA) {
        goto erro2; // Redireciona para a mensagem de erro se o vértice for inválido
    }

    // Verifica se o segundo vértice está em uma posição válida em relação ao primeiro vértice
    if ((EntradaIncidentes_1_x > EntradaIncidentes_2_x) || (EntradaIncidentes_1_y > EntradaIncidentes_2_y)) goto erro3;
    
    printf("Ativar fluxo? (Sim: 1 Não: 0)\t");
    scanf("%d", &ativarfluxo);

    // Loop até que fluxo seja 0 ou 1
    while (ativarfluxo != 0 && ativarfluxo != 1) {
        printf("Entrada inválida. Por favor, insira 1 para Sim ou 0 para Não:\t");
        scanf("%d", &ativarfluxo);
    }

    // Aqui você pode continuar com o que deseja fazer com o valor de fluxo
    if (ativarfluxo) {
        printf("Fluxo ativado.\n");
    } else {
        printf("Fluxo desativado.\n");
    }
    
    if(ativarfluxo){
        printf("Selecione o Semaforo:(Linha e Coluna)\n");
        scanf("%d %d", &semaforo_x, &semaforo_y);
        while(semaforo_x < 0 || semaforo_y > 9 || semaforo_x > 9 || semaforo_y < 0){
            printf("Posicao Invalida, selecione o Semaforo:(Linha e Coluna)\n");
            scanf("%d %d", &semaforo_x, &semaforo_y);  
        }
        semaforo_y *= 3;
        semaforo_y *= 4;
        
        printf("Selecione uma direcao?\n1:Cima\n2:Baixo\n3:Esquerda\n4:Direita\n");
        scanf("%d",&direcao);
        while(direcao < 1 && direcao > 4){
            printf("direcao invalida\n");
            printf("selecione uma direcao?\n1:Cima\n2:Baixo\n3:Esquerda\n4:Direita\n");
            scanf("%d",&direcao);        
        }
    }
    // Cria um array de carros para a simulação
    Carro carros[QTD_CARROS] = {
        // Inicialização dos carros (detalhes omitidos)
    };
    addCar(carros); // Adiciona os carros ao array
    
    // Cria um array de semáforos para a simulação
    Semaforo semaforos[QTD_SEMAFOROS];
    addSemaforo(semaforos); // Inicializa os semáforos
    
    int tempo_simulacao = 1000; // Tempo total de simulação em segundos
    simularCarros(carros, semaforos, tempo_simulacao); // Inicia a simulação

    return 0; // Termina a execução do programa com sucesso


// Mensagens de erro caso os vértices sejam inválidos
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