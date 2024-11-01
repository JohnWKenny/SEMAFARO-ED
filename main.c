/*{ BIBLIOTECAS }*/
#include <stdio.h>   // Biblioteca para funções de entrada e saída, como printf e scanf
#include <stdlib.h>  // Biblioteca para funções como malloc, free, rand e system
#include <stdbool.h> // Biblioteca para usar o tipo booleano em C
#include <time.h>    // Biblioteca para manipulação de tempo, como gerar números aleatórios com base no tempo atual
#include <unistd.h>  // Biblioteca para a função sleep, usada para pausas na execução do programa
#include "cJSON.c"
#include "Structs.h"   // Arquivo de cabeçalho para definição de structs usadas no código
#include "Carros.h"    // Arquivo de cabeçalho para funções relacionadas aos carros
#include "Semaforos.h" // Arquivo de cabeçalho para funções relacionadas aos semáforos
#include "Matriz.h"    // Arquivo de cabeçalho para funções relacionadas à matriz da cidade

// Variáveis globais para as entradas dos incidentes, usadas para determinar pontos específicos na matriz
int entrada_incidentes_1_x, entrada_incidentes_1_y; // Primeiro semaforo do acidente
int entrada_incidentes_2_x, entrada_incidentes_2_y; // Ultimo semaforo do acidente
int ativar_fluxo, exportar_fluxo, semaforo_x, semaforo_y;
int tempo_simulacao; // Tempo de cada simulação
char face;

// Cria um array de carros para a simulação
Carro carros[QTD_CARROS];

// Cria um array de semáforos para a simulação
Semaforo semaforos[QTD_SEMAFOROS];

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
void simularCarros(Carro *carros, Semaforo *semaforos, int tempo_simulacao)
{
    char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA]; // Cria a matriz da cidade
    if (exportar_fluxo) gerarLog('s', matriz, tempo_simulacao); // Inicializa o processo de log
    int start = tempo_simulacao; // Garante que os carros sejam adicionados apenas no início da simulação

    while (tempo_simulacao >= 0) // Loop até que o tempo definido acabe
    {
        system("clear");

        atualizarSemaforos(semaforos);                              // Atualiza os semáforos com novos estados
        inicializarMatriz(matriz);                                  // Inicializa a matriz com pontos e vértices

        if (start == tempo_simulacao) addCar(carros, matriz);       // Adiciona os carros ao array

        atualizarMatriz(matriz, carros, semaforos);                 // Atualiza a matriz com a posição dos carros e semáforos
        printf("Simulacao em andamento: %10d\n\n", tempo_simulacao); // Exibe o tempo restante

        if (exportar_fluxo) gerarLog('c', matriz, tempo_simulacao); // Registra o estado atual da simulação

        imprimirMatriz(matriz, carros, semaforos);                  // Imprime a matriz no terminal
        
        sleep(1);                                               // Aguarda 1 segundo antes de atualizar novamente

        for (int i = QTD_CARROS - 1; i >= 0; i--) MoverCarro(&carros[i], carros, semaforos, matriz); // Função que move cada carro
        
        tempo_simulacao--;
    }

    if (exportar_fluxo) gerarLog('e', matriz, tempo_simulacao); // Finaliza o processo de log
}

int main()
{
    // Solicita ao usuário os vértices de entrada do primeiro incidente
    printf("Digite o primeiro vertice(Coluna e Linha):\n");
    scanf("%d %d", &entrada_incidentes_1_y, &entrada_incidentes_1_x);
    system("clear"); // Limpa a tela do terminal após a entrada

    // Ajusta os valores dos vértices para a escala da matriz (multiplicando por 3 e 4)
    entrada_incidentes_1_x *= 3;
    entrada_incidentes_1_y *= 4;

    // Verifica se os vértices de entrada são válidos (se estão dentro dos limites e múltiplos corretos)
    if (entrada_incidentes_1_x % 3 != 0 || entrada_incidentes_1_x < 0 || entrada_incidentes_1_x > TAMANHO_CIDADE_LINHA ||
        entrada_incidentes_1_y % 4 != 0 || entrada_incidentes_1_y < 0 || entrada_incidentes_1_y > TAMANHO_CIDADE_COLUNA)
        goto erro1; // Redireciona para a mensagem de erro se o vértice for inválido

    // Solicita ao usuário os vértices de entrada do segundo incidente
    printf("Digite o segundo vertice:(Coluna e Linha)\n");
    scanf("%d %d", &entrada_incidentes_2_y, &entrada_incidentes_2_x);
    system("clear"); // Limpa a tela do terminal após a entrada

    // Ruas que dão problemas
    // Problema do inicio
    if (entrada_incidentes_1_y == 0 && (entrada_incidentes_1_x / 3 >= 5 || entrada_incidentes_2_x >= 5))
        entrada_incidentes_2_x = 9;

    // Problema do meio
    if (entrada_incidentes_1_y / 4 == 4 || entrada_incidentes_1_y / 4 == 5 && entrada_incidentes_1_x / 3 <= 5)
        entrada_incidentes_1_y = 3 * 4;
    if ((entrada_incidentes_2_x >= 6) && (entrada_incidentes_2_y == 2 || entrada_incidentes_2_y == 3))
        entrada_incidentes_2_y = 4;

    // Problema do fim
    if ((entrada_incidentes_1_y / 4 == 9 || entrada_incidentes_2_y == 9) && (entrada_incidentes_2_x <= 6 || entrada_incidentes_1_x / 3 <= 6))
        entrada_incidentes_1_x = 0;

    // Ajusta os valores dos vértices para a escala da matriz (multiplicando por 3 e 4)
    entrada_incidentes_2_x *= 3;
    entrada_incidentes_2_y *= 4;

    // Verifica se os vértices de entrada são válidos (se estão dentro dos limites e múltiplos corretos)
    if (entrada_incidentes_2_x % 3 != 0 || entrada_incidentes_2_x < 0 || entrada_incidentes_2_x > TAMANHO_CIDADE_LINHA ||
        entrada_incidentes_2_y % 4 != 0 || entrada_incidentes_2_y < 0 || entrada_incidentes_2_y > TAMANHO_CIDADE_COLUNA)
        goto erro2; // Redireciona para a mensagem de erro se o vértice for inválido

    // Verifica se o segundo vértice está em uma posição válida em relação ao primeiro vértice
    if ((entrada_incidentes_1_x > entrada_incidentes_2_x) || (entrada_incidentes_1_y > entrada_incidentes_2_y))
        goto erro3;

    printf("Ativar fluxo? (Sim: 1 Não: 0)\t");
    scanf("%d", &ativar_fluxo);

    // Loop até que fluxo seja 0 ou 1
    while (ativar_fluxo != 0 && ativar_fluxo != 1)
    {
        printf("Entrada inválida. Por favor, insira 1 para Sim ou 0 para Não:\t");
        scanf("%d", &ativar_fluxo);
    }

    // Aqui você pode continuar com o que deseja fazer com o valor de fluxo
    if (ativar_fluxo)
    {
        printf("Fluxo ativado.\n");
        printf("Selecione o Semaforo:(Linha e Coluna)\n");

        scanf("%d %d", &semaforo_x, &semaforo_y);
        while (semaforo_x < 0 || semaforo_y > 9 || semaforo_x > 9 || semaforo_y < 0)
        {
            printf("Posicao Invalida, selecione o Semaforo:(Linha e Coluna)\n");
            scanf("%d %d", &semaforo_x, &semaforo_y);
        }

        semaforo_x *= 3;
        semaforo_y *= 4;

        printf("Selecione uma face:\nv: Vertical\nh: Horizontal\n");
        scanf(" %c", &face);
        while (face != 'v' && face != 'h')
        {
            printf("face invalida\n");
            printf("selecione uma face:\nv: Vertical\nh: Horizontal\n");
            scanf(" %c", &face);
        }
    }
    else printf("Fluxo desativado.\n");

    printf("Deseja exportar todo o fluxo? (Sim: 1 Não: 0)\t");
    scanf("%d", &exportar_fluxo);

    addSemaforo(semaforos); // Inicializa os semáforos

    // Solicita ao usuário por o tempo a simulação irá ocorrer
    printf("Quanto tempo de simulacao deseja? ");
    scanf("%d", &tempo_simulacao);

    simularCarros(carros, semaforos); // Inicia a simulação

    return 0; // Termina a execução do programa com sucesso

    // Mensagens de erro caso os vértices sejam inválidos
    erro1:
        printf("Vertice invalido: x = %d\ty = %d\tSemaforo Inexistente 1", entrada_incidentes_1_x, entrada_incidentes_1_y);
        return 1;

    erro2:
        printf("Vertice invalido: x = %d\ty = %d\tSemaforo Inexistente 2", entrada_incidentes_2_x, entrada_incidentes_2_y);
        return 2;

    erro3:
        printf("Vertices invalidos: x1 = %d\ty1 = %d\t && x2 = %d\ty2 = %d\t Area Invalida", entrada_incidentes_1_x, entrada_incidentes_1_y, entrada_incidentes_2_x, entrada_incidentes_2_y);
        return 3;
}