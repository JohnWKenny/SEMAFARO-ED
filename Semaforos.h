#ifndef Semaforos_h
#define Semaforos_h

#include "Structs.h"
#include "Matriz.h"
#include <stdlib.h>

// Declaração das variáveis globais para as coordenadas dos incidentes
extern int EntradaIncidentes_1_x, EntradaIncidentes_1_y, EntradaIncidentes_2_x, EntradaIncidentes_2_y;

// Função que atualiza os estados dos semáforos com base em seus tempos de verde e vermelho
void atualizarSemaforos(Semaforo *semaforos)
{
    for (int i = 0; i < QTD_SEMAFOROS; i++)
    {
        semaforos[i].contador++; // Incrementa o contador do semáforo para contar o tempo

        // Verifica se o semáforo está verde e se já passou o tempo verde especificado
        if (semaforos[i].estado_verde && semaforos[i].contador >= semaforos[i].tempo_verde)
        {  
            semaforos[i].estado_verde = false; // Muda para vermelho
            semaforos[i].contador = 0; // Reseta o contador
        }
        // Verifica se o semáforo está vermelho e se já passou o tempo vermelho especificado
        else if (!semaforos[i].estado_verde && semaforos[i].contador >= semaforos[i].tempo_vermelho)
        {
            semaforos[i].estado_verde = true; // Muda para verde
            semaforos[i].contador = 0; // Reseta o contador
        }
    }
}

// Função para adicionar os semáforos e configurar suas propriedades
void addSemaforo(Semaforo semaforos[QTD_SEMAFOROS])
{
    int i = 0; // Índice para acessar os semáforos no array

    // Loop para posicionar os semáforos em coordenadas predefinidas (grade)
    for (int indice_x = 0; indice_x <= 27; indice_x += 3)
    {
        for (int indice_y = 0; indice_y <= 36; indice_y += 4)
        {
            // Ignora as bordas da matriz, onde não haverá semáforos
            if ((!indice_y || indice_y == 36) && (!indice_x || indice_x == 27))
                continue;

            // Define o tempo de verde e vermelho aleatoriamente entre 1 e 4 segundos
            int t_verde = (rand() % 4) + 1;
            int t_vermelho = (rand() % 4) + 1;

            // Atribui as propriedades ao semáforo atual
            semaforos[i].x = indice_x;
            semaforos[i].y = indice_y;
            semaforos[i].tempo_verde = t_verde;
            semaforos[i].tempo_vermelho = t_vermelho;
            semaforos[i].contador = 0; // Inicia o contador em zero
            semaforos[i].estado_verde = true; // Inicia com o estado verde
            semaforos[i].alerta = false; // Inicia sem alerta

            // Verifica se o semáforo está dentro da área de incidente definida pelo usuário
            if (indice_x >= EntradaIncidentes_1_x && indice_x <= EntradaIncidentes_2_x &&
                indice_y >= EntradaIncidentes_1_y && indice_y <= EntradaIncidentes_2_y)
            {
                semaforos[i].alerta = true; // Ativa o alerta se estiver na área de incidente
            }
            i++; // Avança para o próximo semáforo
        }
    }
}

#endif
