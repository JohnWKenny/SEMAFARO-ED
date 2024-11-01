#ifndef Semaforos_h
#define Semaforos_h

#include "Structs.h"
#include "Matriz.h"
#include <stdlib.h>

// Declaração das variáveis globais para as coordenadas dos incidentes
extern int entrada_incidentes_1_x, entrada_incidentes_1_y, entrada_incidentes_2_x, entrada_incidentes_2_y;

// Função que atualiza os estados dos semáforos com base em seus tempos de verde e vermelho
void atualizarSemaforos(Semaforo *semaforos)
{
    for (int indice_semaforo = 0; indice_semaforo < QTD_SEMAFOROS; indice_semaforo++)
    {
        semaforos[indice_semaforo].contador++; // Incrementa o contador do semáforo para contar o tempo

        // Verifica se o semáforo está verde e se já passou o tempo verde especificado
        if (semaforos[indice_semaforo].estado_verde && semaforos[indice_semaforo].contador >= semaforos[indice_semaforo].tempo_verde)
        {
            semaforos[indice_semaforo].estado_verde = false; // Muda para vermelho
            semaforos[indice_semaforo].contador = 0;         // Reseta o contador
        }
        // Verifica se o semáforo está vermelho e se já passou o tempo vermelho especificado
        else if (!semaforos[indice_semaforo].estado_verde && semaforos[indice_semaforo].contador >= semaforos[indice_semaforo].tempo_vermelho + 1) // como o semaforo amarelo esta no vermelho é necessario somar 1
        {
            semaforos[indice_semaforo].estado_verde = true; // Muda para verde
            semaforos[indice_semaforo].contador = 0;        // Reseta o contador
        }
    }
}

// Função para adicionar os semáforos e configurar suas propriedades
void addSemaforo(Semaforo semaforos[QTD_SEMAFOROS])
{
    int indice_semaforo = 0; // Índice para acessar os semáforos no array

    // Loop para posicionar os semáforos em coordenadas predefinidas (grade)
    for (int indice_x = 0; indice_x < TAMANHO_CIDADE_LINHA; indice_x += 3)
    {
        for (int indice_y = 0; indice_y < TAMANHO_CIDADE_COLUNA; indice_y += 4)
        {
            // Ignora as bordas da matriz, onde não haverá semáforos
            if ((!indice_y || indice_y == 36) && (!indice_x || indice_x == 27))
                continue;

            // Define o tempo de verde e vermelho aleatoriamente entre 1 e 4 segundos
            int t_verde = (rand() % 4) + 1;
            int t_vermelho = (rand() % 4) + 1;

            // Atribui as propriedades ao semáforo atual
            semaforos[indice_semaforo].x = indice_x;
            semaforos[indice_semaforo].y = indice_y;
            semaforos[indice_semaforo].tempo_verde = t_verde;
            semaforos[indice_semaforo].tempo_vermelho = t_vermelho;
            semaforos[indice_semaforo].contador = -1;       // Começando com -1 pq o atualizarSemaforos acontece no primeiro print da matriz, ent ela tava começando no tempo 1
            semaforos[indice_semaforo].estado_verde = true; // Inicia com o estado verde
            semaforos[indice_semaforo].alerta = false;      // Inicia sem alerta

            // Verifica se o semáforo está dentro da área de incidente definida pelo usuário
            if (indice_x >= entrada_incidentes_1_x && indice_x <= entrada_incidentes_2_x &&
                indice_y >= entrada_incidentes_1_y && indice_y <= entrada_incidentes_2_y)
                semaforos[indice_semaforo].alerta = true; // Ativa o alerta se estiver na área de incidente

            indice_semaforo++; // Avança para o próximo semáforo
        }
    }
}

#endif
