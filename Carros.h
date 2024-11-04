#ifndef Carros_h
#define Carros_h

#include "Structs.h"
#include "Matriz.h"

// Função para verificar se uma posição específica (x, y) está ocupada por um carro
// Parâmetros:
// - x: posição x do carro atual
// - y: posição y do carro atual
// - Carros: array de carros
// - utlimo_movimento: ultimo movimento do carro atual
bool posicaoOcupada(int x, int y, Carro *carros, int ultimo_movimento)
{
    for (int i = 0; i < QTD_CARROS; i++)
    {
        // Encontrando o carro a frente
        if (carros[i].x == x && carros[i].y == y)
        {
            // Para via dupla, explicada no readme
            if (carros[i].via_dupla)
            {
                if (carros[i].ultimo_movimento == ultimo_movimento && carros[i].parado)
                    return true;
                else
                    return false;
            }
            else if (carros[i].parado)
                return true;
            return false;
        }
    }

    return false;
}
// Função que vai preencher cada pilha de carros.
// Parâmetros:
// - movimentos: pilha de movimentos
// - carro: carro atual.
void preencher(Pilha *movimentos, Carro *carro)
{
    if ((carro->id % 10 == 0 || carro->id % 10 == 1) && carro->id < 10)
    {
        push(movimentos, '>');
        push(movimentos, 'V');
        push(movimentos, '<');
        push(movimentos, '^');
    }
    else if (carro->id % 10 == 2)
    {
        push(movimentos, '<');
        push(movimentos, 'V');
        push(movimentos, '>');
        push(movimentos, '^');
    }
    else if (carro->id % 10 == 3)
    {
        push(movimentos, '>');
        push(movimentos, '^');
        push(movimentos, '<');
        push(movimentos, 'V');
    }
    else if (carro->id % 10 == 4)
    {
        push(movimentos, '<');
        push(movimentos, '^');
        push(movimentos, '>');
        push(movimentos, 'V');
    }
    else if (carro->id % 10 == 5)
    {
        push(movimentos, '^');
        push(movimentos, '>');
        push(movimentos, 'V');
        push(movimentos, '<');
    }
    else if (carro->id % 10 == 6)
    {
        push(movimentos, '^');
        push(movimentos, '<');
        push(movimentos, 'V');
        push(movimentos, '>');
    }
    else if (carro->id % 10 == 7)
    {
        push(movimentos, 'V');
        push(movimentos, '>');
        push(movimentos, '^');
        push(movimentos, '<');
    }
    else if (carro->id % 10 == 8)
    {
        push(movimentos, '^');
        push(movimentos, '<');
        push(movimentos, 'V');
        push(movimentos, '>');
    }
    else if (carro->id % 10 == 9)
    {
        push(movimentos, '>');
        push(movimentos, 'V');
        push(movimentos, '<');
        push(movimentos, '^');
    }
    else
    {
        push(movimentos, 'V');
        push(movimentos, '<');
        push(movimentos, '^');
        push(movimentos, '>');
    }
}

// Move um carro de acordo com sua velocidade e o estado dos semáforos
// Parâmetros:
// - carro: ponteiro para o carro que tentará desviar
// - carros: array de carros
// - semaforos: array de semáforos
// - matriz: matriz representando a cidade
// - deslocamento: quantas unidades o carro se move
void DefinirMovimentoNoSemaforo(Carro *carro, Carro *carros, Semaforo *semaforos, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], int deslocamento)
{
    // Caso a pilha esteja vazia
    if (isEmpty(&carro->movimentos))
        preencher(&carro->movimentos, carro);

    int tentantivas_falhas = 0; // Tentantivas de procurar uma direção que nao achou
    while (1)
    {
        if (tentantivas_falhas > 1)
        {
            carro->parado = true;

            // Voltar para a posição que estava antes do semaforo
            switch (carro->ultimo_movimento)
            {
            case 94:
                carro->x += deslocamento;
                break;
            case 86:
                carro->x -= deslocamento;
                break;
            case 62:
                carro->y -= deslocamento;
                break;
            case 60:
                carro->y += deslocamento;
                break;

            default:
                break;
            }

            break;
        }
        switch (pop(&carro->movimentos))
        {
        case 94: //^
            if (isEmpty(&carro->movimentos))
            {
                tentantivas_falhas += 1;
                preencher(&carro->movimentos, carro);
            }

            // Para o carro nao da um 360 no semaforo
            if (carro->ultimo_movimento == 'V' && matriz[carro->x + deslocamento][carro->y] == '|')
                continue;

            if (!posicaoOcupada(carro->x - deslocamento, carro->y, carros, carro->ultimo_movimento) && (matriz[carro->x - deslocamento][carro->y] == '^' ||
                                                                                               matriz[carro->x - deslocamento][carro->y] == '|'))
            {
                carro->ultimo_movimento = '^';
                if (matriz[carro->x - deslocamento][carro->y] == '|')
                    carro->via_dupla = true;
                break;
            }

            continue;
        case 86: // v
            if (isEmpty(&carro->movimentos))
            {
                tentantivas_falhas += 1;
                preencher(&carro->movimentos, carro);
            }

            if (carro->ultimo_movimento == '^' && matriz[carro->x - deslocamento][carro->y] == '|')
                continue;

            if (!posicaoOcupada(carro->x + deslocamento, carro->y, carros, carro->ultimo_movimento) && (matriz[carro->x + deslocamento][carro->y] == 'V' ||
                                                                                               matriz[carro->x + deslocamento][carro->y] == '|'))
            {
                carro->ultimo_movimento = 'V';
                if (matriz[carro->x + deslocamento][carro->y] == '|')
                    carro->via_dupla = true;
                break;
            }

            continue;
        case 62: //>
            if (isEmpty(&carro->movimentos))
            {
                tentantivas_falhas += 1;
                preencher(&carro->movimentos, carro);
            }

            if (carro->ultimo_movimento == '<' && matriz[carro->x][carro->y + deslocamento] == '-')
                continue;

            if (!posicaoOcupada(carro->x, carro->y + deslocamento, carros, carro->ultimo_movimento) && (matriz[carro->x][carro->y + deslocamento] == '>' ||
                                                                                               matriz[carro->x][carro->y + deslocamento] == '-'))
            {
                if (matriz[carro->x][carro->y + deslocamento] == '-')
                    carro->via_dupla = true;
                carro->ultimo_movimento = '>';
                break;
            }

            continue;
        case 60: //<
            if (isEmpty(&carro->movimentos))
            {
                tentantivas_falhas += 1;
                preencher(&carro->movimentos, carro);
            }

            if (carro->ultimo_movimento == '>' && matriz[carro->x][carro->y - deslocamento] == '-')
                continue;

            if (!posicaoOcupada(carro->x, carro->y - deslocamento, carros, carro->ultimo_movimento) && (matriz[carro->x][carro->y - deslocamento] == '<' ||
                                                                                               matriz[carro->x][carro->y - deslocamento] == '-'))
            {
                if (matriz[carro->x][carro->y - deslocamento] == '-')
                    carro->via_dupla = true;
                carro->ultimo_movimento = '<';
                break;
            }

            continue;
        }

        break;
    }
}

void MoverCarro(Carro *carro, Carro *carros, Semaforo *semaforos, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    int deslocamento = carro->velocidade;
    switch (carro->ultimo_movimento)
    {
    case '^': // Mover para cima
        //Tratamento da velocidade, explicada melhor no readme
        deslocamento = carro->velocidade;
        for (int i = deslocamento; i >= 1; i--)
        {
            if (((matriz[carro->x - i][carro->y] != '^' && matriz[carro->x - i][carro->y] != '|' && matriz[carro->x - i][carro->y] != 'C') || matriz[carro->x - i][carro->y] == 'A'))
            {
                deslocamento = 1;
                break;
            }
            else if (matriz[carro->x - i][carro->y] == 'C')
            {
                if((carro->x - i) % 3 == 0 && (carro->y) % 4 == 0)
                {
                    deslocamento = i - 1;
                    continue;
                }
                int aux = i - 1;
                i = 1;
                deslocamento = aux;
                continue;
            }
        }
        if (deslocamento == 0)
            deslocamento = 1;

        if (carro->x - deslocamento >= 0)
        {
            // Se for um semaforo
            if ((carro->x - deslocamento) % 3 == 0 && (carro->y) % 4 == 0 && matriz[carro->x - deslocamento][carro->y] != 'v')
            {
                Semaforo semaforo;
                for (int indice = 0; indice < QTD_SEMAFOROS; indice++)
                {
                    if (semaforos[indice].x == carro->x - deslocamento && semaforos[indice].y == carro->y)
                    {
                        semaforo = semaforos[indice];
                        break;
                    }
                }

                // Caso possa se mover
                if (!semaforo.estado_verde)
                {
                    carro->parado = false;
                    carro->x -= deslocamento;
                    DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento); // Chama a função para lidar com o semáforo
                }
                else
                    carro->parado = true;
            }
            else if (matriz[carro->x - deslocamento][carro->y] == 'v' && !posicaoOcupada(carro->x - deslocamento, carro->y, carros, carro->ultimo_movimento))
            {
                carro->x -= deslocamento;
                DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento); // Chama a função para lidar com o semáforo
            }
            else if (!posicaoOcupada(carro->x - deslocamento, carro->y, carros, carro->ultimo_movimento) && matriz[carro->x - deslocamento][carro->y] != 'A')
                carro->x -= deslocamento;
            else
                carro->parado = true;
        }
        else
            DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento);

        break;

    case 'V': // Mover para baixo
        deslocamento = carro->velocidade;
        for (int i = deslocamento; i >= 1; i--)
        {
            if (((matriz[carro->x + i][carro->y] != '|' && matriz[carro->x + i][carro->y] != 'V' && matriz[carro->x + i][carro->y] != 'C') || matriz[carro->x + i][carro->y] == 'A'))
            {
                deslocamento = 1;
                break;
            }
            else if (matriz[carro->x + i][carro->y] == 'C')
            {
                if((carro->x + i) % 3 == 0 && (carro->y) % 4 == 0)
                {
                    deslocamento = i - 1;
                    continue;
                }
                int aux = i - 1;
                i = 1;
                deslocamento = aux;
                continue;
            }
        }
        if (deslocamento == 0)
            deslocamento = 1;

        if (carro->x + deslocamento < TAMANHO_CIDADE_LINHA)
        {
            if ((carro->x + deslocamento) % 3 == 0 && (carro->y) % 4 == 0 && matriz[carro->x + deslocamento][carro->y] != 'v')
            {
                Semaforo semaforo;
                for (int indice = 0; indice < QTD_SEMAFOROS; indice++)
                {
                    if (semaforos[indice].x == carro->x + deslocamento && semaforos[indice].y == carro->y)
                    {
                        semaforo = semaforos[indice];
                        break;
                    }
                }

                if (!semaforo.estado_verde)
                {
                    carro->parado = false;
                    carro->x += deslocamento;
                    DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento); // Chama a função para lidar com o semáforo
                }
                else
                    carro->parado = true;
            }
            else if (matriz[carro->x + deslocamento][carro->y] == 'v' && !posicaoOcupada(carro->x + deslocamento, carro->y, carros, carro->ultimo_movimento))
            {
                carro->x += deslocamento;
                DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento); // Chama a função para lidar com o semáforo
            }
            else if (!posicaoOcupada(carro->x + deslocamento, carro->y, carros, carro->ultimo_movimento) && matriz[carro->x + deslocamento][carro->y] != 'A')
                carro->x += deslocamento;
            else
                carro->parado = true;
        }
        else
            DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento);

        break;

    case '>': // Mover para direita
        deslocamento = carro->velocidade;
        for (int i = deslocamento; i >= 1; i--)
        {
            if (((matriz[carro->x][carro->y + i] != '>' && matriz[carro->x][carro->y + i] != '-' && matriz[carro->x][carro->y + i] != 'C') || matriz[carro->x][carro->y + i] == 'A'))
            {
                deslocamento = 1;
                break;
            }
            else if (matriz[carro->x][carro->y + i] == 'C')
            {
                if((carro->x) % 3 == 0 && (carro->y + i) % 4 == 0)
                {
                    deslocamento = i - 1;
                    continue;
                }
                deslocamento -= 1;
                continue;
            }
        }
        if (deslocamento == 0)
            deslocamento = 1;

        if (carro->y + deslocamento < TAMANHO_CIDADE_COLUNA)
        {
            if ((carro->x) % 3 == 0 && (carro->y + deslocamento) % 4 == 0 && matriz[carro->x][carro->y + deslocamento] != 'v')
            {
                Semaforo semaforo;
                for (int indice = 0; indice < QTD_SEMAFOROS; indice++)
                {
                    if (semaforos[indice].x == carro->x && semaforos[indice].y == carro->y + deslocamento)
                    {
                        semaforo = semaforos[indice];
                        break;
                    }
                }

                if (semaforo.estado_verde)
                {
                    carro->parado = false;
                    carro->y += deslocamento;
                    DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento); // Chama a função para lidar com o semáforo
                }
                else
                    carro->parado = true;
            }
            else if (matriz[carro->x][carro->y + deslocamento] == 'v' && !posicaoOcupada(carro->x, carro->y + deslocamento, carros, carro->ultimo_movimento))
            {
                carro->y += deslocamento;
                DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento); // Chama a função para lidar com o semáforo
            }
            else if (!posicaoOcupada(carro->x, carro->y + deslocamento, carros, carro->ultimo_movimento) && matriz[carro->x][carro->y + deslocamento] != 'A')
                carro->y += deslocamento;
            else
                carro->parado = true;
        }
        else
            DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento);

        break;

    case '<': // Mover para esquerda
        deslocamento = carro->velocidade;
        for (int i = deslocamento; i >= 1; i--)
        {
            if (((matriz[carro->x][carro->y - i] != '<' && matriz[carro->x][carro->y - i] != '-' && matriz[carro->x][carro->y - i] != 'C') || matriz[carro->x][carro->y - i] == 'A'))
            {
                deslocamento = 1;
                break;
            }
            else if (matriz[carro->x][carro->y - i] == 'C')
            {
                if((carro->x) % 3 == 0 && (carro->y-i) % 4 == 0)
                {
                    deslocamento = i - 1;
                    continue;
                }
                deslocamento -= 1;
                continue;
            }
        }
        if (deslocamento == 0)
            deslocamento = 1;
        if (carro->y - deslocamento >= 0)
        {
            if ((carro->x) % 3 == 0 && (carro->y - deslocamento) % 4 == 0 && matriz[carro->x][carro->y - deslocamento] != 'v')
            {
                Semaforo semaforo;
                for (int indice = 0; indice < QTD_SEMAFOROS; indice++)
                {
                    if (semaforos[indice].x == carro->x && semaforos[indice].y == carro->y - deslocamento)
                    {
                        semaforo = semaforos[indice];
                        break;
                    }
                }

                if (semaforo.estado_verde)
                {
                    carro->parado = false;
                    carro->y -= deslocamento;
                    DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento); // Chama a função para lidar com o semáforo
                }
                else
                    carro->parado = true;
            }
            else if (matriz[carro->x][carro->y - deslocamento] == 'v' && !posicaoOcupada(carro->x, carro->y - deslocamento, carros, carro->ultimo_movimento))
            {
                carro->y -= deslocamento;
                DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento); // Chama a função para lidar com o semáforo
            }
            else if (!posicaoOcupada(carro->x, carro->y - deslocamento, carros, carro->ultimo_movimento) && matriz[carro->x][carro->y - deslocamento] != 'A')
                carro->y -= deslocamento;
            else
                carro->parado = true;
        }
        else
            DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz, deslocamento);

        break;
    }
}

void addCar(Carro *carros, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    for (int i = 0; i < QTD_CARROS; i++)
    {
        carros[i].id = i;

        int px = i / 10, py = i % 10;

        if (px == 1)
        {
            carros[i].x = 3;
            carros[i].ultimo_movimento = '>';
            carros[i].velocidade = 2;
        }
        else if (px == 2)
        {
            carros[i].x = 6;
            carros[i].ultimo_movimento = '>';
            carros[i].velocidade = 1;
        }
        else if (px == 3)
        {
            carros[i].desativado = true;
            continue;
        }
        else if (px == 4)
        {
            carros[i].x = 12;
            carros[i].ultimo_movimento = '>';
            carros[i].velocidade = 3;
        }
        else if (px == 5 || px == 0)
        {
            carros[i].x = 15;
            carros[i].ultimo_movimento = '>';
            carros[i].velocidade = 2;
        }
        else if (px == 6)
        {
            carros[i].x = 18;
            carros[i].ultimo_movimento = '<';
            carros[i].velocidade = 1;
        }
        else if (px == 7)
        {
            carros[i].x = 21;
            carros[i].ultimo_movimento = '<';
            carros[i].velocidade = 3;
        }
        else if (px == 8)
        {
            carros[i].x = 24;
            carros[i].ultimo_movimento = '<';
            carros[i].velocidade = 3;
        }
        else if (px == 9)
        {
            carros[i].x = 27;
            carros[i].ultimo_movimento = '<';
            carros[i].velocidade = 2;
        }

        if (py == 0)
            py = 5;
        if (py == 9)
            py = 0;

        int isDesativate = 0;

        for (int y = py * 4; y + 1 < (py + 1) * 4; y++)
        {
            int isOcupped = 0;
            isDesativate = 1;

            for (int indice = 0; indice < QTD_CARROS; indice++)
            {
                if (carros[i].x == carros[indice].x && carros[indice].y == y + 1)
                {
                    isOcupped = 1;
                    break;
                }
            }

            if (!isOcupped)
            {
                isDesativate = 0;
                carros[i].y = y + 1;
                break;
            }
        }

        if (matriz[carros[i].x][carros[i].y] == 'A')
            isDesativate = true;

        if (isDesativate)
            carros[i].desativado = true;
        else
            carros[i].desativado = false;

        carros[i].parado = false;
        initPilha(&carros[i].movimentos);
        preencher(&carros[i].movimentos, &carros[i]);
    }
}

int totalCars(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    int total = 0;

    for (int i = 0; i < TAMANHO_CIDADE_LINHA; i++)
    {
        for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++)
        {
            if (matriz[i][j] == 'C')
                total++;
        }
    }

    return total;
}

#endif