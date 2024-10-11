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
#define QTD_CARROS 3            // Define a quantidade de carros
#define QTD_SEMAFOROS 96         // Define a quantidade de semaforos

/*{ ESTRUTURAS }*/
// Definição da estrutura que representa um carro.
typedef struct
{
    int id;       // Identificador do carro
    int x, y;     // Coordenadas X e Y na cidade (matriz)
    int viaDupla; // Indica se o carro está em uma pista dupla e guarda qual o sentido da via
    int direcaoCarro; //******/
    Pilha movimentos;  // Pilha que guarda os movimentos do carro
    int lastmove;
    int velocidade;
    bool parado;
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

// Configuração das estradas
Estrada estradas[QTD_ESTRADAS] = {
    {1, 0, 0, 0}, {1, 1, 0, 27}, {0, 1, 0, 36}, {0, 0, 0, 0}, // Estradas nas extremidades
    {0, 0, 1, 4}, {0, 0, 1, 8}, {0, 0, 1, 20}, {0, 0, 1, 24}, {0, 0, 1, 28}, {0, 0, 1, 32}, // Estradas verticais
    {1, 0, 0, 3}, {1, 0, 0, 6}, {1, 0, 1, 12}, {1, 0, 0, 15}, {1, 1, 0, 18}, {1, 1, 0, 21}, {1, 1, 0, 24}  // Estradas horizontais
};

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
    for (int i = 0; i < QTD_SEMAFOROS; i++)
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
        if (carros[i].x == x && carros[i].y == y && carros[i].parado) 
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

// Função que vai preencher cada pilha de carros.
// Parâmetros:
// - movimentos: pilha de movimentos, carro: carro atual.
void preencher(Pilha *movimentos, Carro *carro)
{
    if((carro->id % 10 == 0 || carro->id % 10 == 1) && carro->id < 10)
    {
        push(movimentos, '>');
        push(movimentos, 'V');
        push(movimentos, '<');
        push(movimentos, '^');
    }
    else if(carro->id % 10 == 2)
    {
        push(movimentos, '<');
        push(movimentos, 'V');
        push(movimentos, '>');
        push(movimentos, '^');
    }
    else if(carro->id % 10 == 3)
    {
        push(movimentos, '>');
        push(movimentos, '^');
        push(movimentos, '<');
        push(movimentos, 'V');
    }
    else if(carro->id % 10 == 4)
    {
        push(movimentos, '<');
        push(movimentos, '^');  
        push(movimentos, '>');
        push(movimentos, 'V');
    }
       else if(carro->id % 10 == 5)
    {
        push(movimentos, '^');
        push(movimentos, '>');
        push(movimentos, 'V');
        push(movimentos, '<');
    }
       else if(carro->id % 10 == 6)
    {
        push(movimentos, '^');
        push(movimentos, '<');
        push(movimentos, 'V');
        push(movimentos, '>');
    }
       else if(carro->id % 10 == 7)
    {
        push(movimentos, 'V');
        push(movimentos, '>');
        push(movimentos, '^');
        push(movimentos, '<');
    }
       else if(carro->id % 10 == 8)
    {
        push(movimentos, '^');
        push(movimentos, '<'); 
        push(movimentos, 'V');
        push(movimentos, '>');
    }
       else if(carro->id % 10 == 9)
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

// Seta o primeiro movimento do carro
// Parâmetros:
// - carro: ponteiro para o carro que tentará desviar
// - matriz: matriz representando a cidade
void firstmovecar(Carro *carro, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    if(carro->x - 1 >= 0 && carro->x + 1 < TAMANHO_CIDADE_LINHA && matriz[carro->x - 1][carro->y] != '.' && matriz[carro->x + 1][carro->y] != '.')
    {
        for(int ind_estrada = 0; ind_estrada < QTD_ESTRADAS; ind_estrada++)
        {
            if(estradas[ind_estrada].direcao) continue;
            
            if(estradas[ind_estrada].ini == carro->y)
            {

                if(estradas[ind_estrada].sentido) carro->lastmove = 'V';
                else if(!estradas[ind_estrada].sentido) carro->lastmove = '^';
                else 
                {
                    int coninueWhile = 1;
                    while(coninueWhile)
                    {
                        switch (pop(&carro->movimentos))
                        {
                            case 94: // '^' - Mover para cima
                                carro->lastmove = '^';
                                coninueWhile != coninueWhile;
                                break;
                            case 86: // 'V' - Mover para baixo
                                carro->lastmove = 'V';
                                coninueWhile != coninueWhile;
                                break;
                            default:
                                break;
                        }   
                    }
                }
            }
            
        }
        
    } else
    {
        
        for(int ind_estrada = 0; ind_estrada < QTD_ESTRADAS; ind_estrada++)
        {
            if(!estradas[ind_estrada].direcao) continue;
            
            if(estradas[ind_estrada].ini == carro->x)
            {
                
                if(estradas[ind_estrada].sentido) carro->lastmove = '<';
                else if(!estradas[ind_estrada].sentido) carro->lastmove = '>';
                else 
                {
                    int coninueWhile = 1;
                    while(coninueWhile)
                    {
                        switch (pop(&carro->movimentos))
                        {
                            case 62: // '>' - Mover para direita
                                carro->lastmove = '>';
                                coninueWhile = 0;
                                break;
                            case 60: // '<' - Mover para esquerda
                                carro->lastmove = '<';
                                coninueWhile = 0;
                                break;
                            default:
                                break;
                        }   
                    }
                }
            }
            
        }
    }
    
    
}

// Move um carro de acordo com sua velocidade e o estado dos semáforos
// Parâmetros:
// - carro: ponteiro para o carro que tentará desviar
// - carros: array de carros
// - semaforos: array de semáforos
// - matriz: matriz representando a cidade
void DefinirMovimentoNoSemaforo(Carro *carro, Carro *carros, Semaforo *semaforos, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
        if (isEmpty(&carro->movimentos))
        {
            preencher(&carro->movimentos, carro);
        }
        char movimentodocarro = pop(&carro->movimentos);
        if((carro->lastmove == 'V' || carro->lastmove == '^') && (matriz[carro->x][carro->y] == 'R' || matriz[carro->x ][carro->y]== 'v')){
            while(1){
                switch(movimentodocarro){
                    case 94: //^
                        if(posicaoOcupada(carro->x -carro->velocidade, carro->y, carros) || matriz[carro->x-carro->velocidade][carro->y] != '^' && matriz[carro->x-carro->velocidade][carro->y]!='|'){
                            if (isEmpty(&carro->movimentos))
                            {
                            preencher(&carro->movimentos, carro);
                            }
                            movimentodocarro = pop(&carro->movimentos);
                        }
                        else{
                            carro->lastmove = '^';
                            carro->parado = false; 
                            return;
                        }    
                    case 86: //v
                        if(posicaoOcupada(carro->x +carro->velocidade, carro->y, carros)|| matriz[carro->x+carro->velocidade][carro->y] != 'V' && matriz[carro->x+carro->velocidade][carro->y]!='|'){
                                if (isEmpty(&carro->movimentos))
                                {
                                preencher(&carro->movimentos, carro);
                                }
                                movimentodocarro = pop(&carro->movimentos);
                                
                            }
                        else{
                                carro->lastmove = 'V';
                                carro->parado = false; 
                                return;
                        }
                    case 62: //>
                        if(posicaoOcupada(carro->x+carro->velocidade, carro->y, carros)|| matriz[carro->x+carro->velocidade][carro->y] != '>'|| matriz[carro->x+carro->velocidade][carro->y]!='-'){
                                    if (isEmpty(&carro->movimentos))
                                    {
                                    preencher(&carro->movimentos, carro);
                                    }
                                    movimentodocarro = pop(&carro->movimentos);
                                    
                                }
                        else{
                            carro->lastmove = '>';
                            carro->parado = false; 
                            return;
                        }
                    case 60: //<
                        if(posicaoOcupada(carro->x, carro->y-carro->velocidade, carros)|| matriz[carro->x][carro->y-carro->velocidade] != '<'&& matriz[carro->x][carro->y-carro->velocidade]!='-'){
                            if (isEmpty(&carro->movimentos))
                                {
                                preencher(&carro->movimentos, carro);
                                }
                                movimentodocarro = pop(&carro->movimentos);
                                
                        }
                        else{
                            carro->lastmove = '<';
                            carro->parado = false; 
                            return;
                        }
                }
 
            }
        }
        else if((carro->lastmove == '>'|| carro->lastmove =='<') && (matriz[carro->x][carro->y]== 'G'|| matriz[carro->x][carro->y]== 'v')){
            while(1){
                    switch(movimentodocarro){
                        case 94: //^
                            if(posicaoOcupada(carro->x-carro->velocidade, carro->y, carros)|| matriz[carro->x - carro->velocidade][carro->y] != '^'&& matriz[carro->x - carro->velocidade][carro->y]!='|'){
                                if (isEmpty(&carro->movimentos))
                                {
                                    preencher(&carro->movimentos, carro);
                                }
                                movimentodocarro = pop(&carro->movimentos);
                            }
                            else{
                                carro->lastmove = '^';
                                carro->parado = false; 
                                return;
                            }    
                        case 86: //v
                            if(posicaoOcupada(carro->x +carro->velocidade, carro->y, carros) || matriz[carro->x +carro->velocidade][carro->y] != 'V'&& matriz[carro->x +carro->velocidade][carro->y]!='|'){
                                if (isEmpty(&carro->movimentos))
                                    {
                                    preencher(&carro->movimentos, carro);
                                    }
                                    movimentodocarro = pop(&carro->movimentos);
                                    
                                }
                            else{
                                carro->lastmove = 'V';
                                carro->parado = false; 
                                return;
                            }
                        case 62: //>
                            if(posicaoOcupada(carro->x, carro->y +carro->velocidade, carros)|| matriz[carro->x][carro->y +carro->velocidade] != '>'&& matriz[carro->x][carro->y +carro->velocidade]!='-'){
                                        if (isEmpty(&carro->movimentos))
                                        {
                                        preencher(&carro->movimentos, carro);
                                        }
                                        movimentodocarro = pop(&carro->movimentos);
                                        
                                    }
                            else{
                                carro->lastmove = '>';
                                carro->parado = false; 
                                return;
                            }
                        case 60: //<
                            if(posicaoOcupada(carro->x, carro->y -carro->velocidade, carros)|| matriz[carro->x][carro->y -carro->velocidade] != '<' && matriz[carro->x][carro->y -carro->velocidade]!='-'){
                                        if (isEmpty(&carro->movimentos))
                                        {
                                        preencher(&carro->movimentos, carro);
                                        }
                                        movimentodocarro = pop(&carro->movimentos);
                                        
                                    }
                            else{
                                carro->lastmove = '<';
                                carro->parado = false; 
                                return;
                            }
            }
        }
    }
    else{
        carro->parado = true;
        return;
    }
}

void MoverCarro(Carro *carro, Carro *carros, Semaforo *semaforos, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA]) {
    // Se o carro não está parado, realiza o próximo movimento
    if(carro->parado== true) carro-> parado = false;
    if (carro->parado == false) {
        switch (carro->lastmove) {
            case '^': // Mover para cima
                if (carro->x - carro->velocidade >= 0) {
                    if (matriz[carro->x - carro->velocidade][carro->y] == 'G') {
                        carro->parado = true;      
                        return; // Carro encontra semáforo verde, parar
                    } else if (matriz[carro->x - carro->velocidade][carro->y] == 'R') {
                        carro->x -= carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
                    } else {
                        carro->x -= carro->velocidade;
                        return;
                    }
                }

            case 'V': // Mover para baixo
                if (carro->x + carro->velocidade < TAMANHO_CIDADE_LINHA) {
                    if (matriz[carro->x + carro->velocidade][carro->y] == 'G') {
                        carro->parado = true;      
                        return; // Carro encontra semáforo verde, parar
                    } else if (matriz[carro->x + carro->velocidade][carro->y] == 'R') {
                        carro->x += carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
                    } else {
                        carro->x += carro->velocidade;
                        return;
                    }
                }

            case '>': // Mover para direita
                if (carro->y + carro->velocidade < TAMANHO_CIDADE_COLUNA) {
                    if (matriz[carro->x][carro->y + carro->velocidade] == 'R') {
                        carro->parado = true;      
                        return; // Carro encontra semáforo verde, parar
                    } else if (matriz[carro->x][carro->y + carro->velocidade] == 'G') {
                        carro->y += carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
                    } else {
                        imprimirMatriz(matriz, semaforos); // Imprime a matriz
                        carro->y += carro->velocidade;
                        system("clear");
                        imprimirMatriz(matriz, semaforos); // Imprime a matriz
                        return;
                    }
                }

            case '<': // Mover para esquerda
                if (carro->y - carro->velocidade >= 0) {
                    if (matriz[carro->x][carro->y - carro->velocidade] == 'R'/*lembra do carro*/) {
                        carro->parado = true;      
                        return; // Carro encontra semáforo verde, parar
                    } else if (matriz[carro->x][carro->y - carro->velocidade] == 'G') {
                        carro->y -= carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
                    } else {
                        carro->y -= carro->velocidade;
                        return;
                    }
                }
        }
    }
   
}

void addCar(Carro *carros)
{
    for(int i = 0; i < QTD_CARROS; i++)
    {
        carros[i].id = i;
        int px = i / 10, py = i % 10;
        if(px == 0){
            carros[i].x = 0;
            carros[i].lastmove = '>';
        } 
        else if(px == 1){
            carros[i].x = 3;
            carros[i].lastmove = '>';
        }
        else if(px == 2){
            carros[i].x = 6;
            carros[i].lastmove = '>';
        }
        else if(px == 3){
            carros[i].x = 12;
            carros[i].lastmove = '>';
        }
        else if(px == 4){
            carros[i].x = 15;
            carros[i].lastmove = '>';
        }
        else if(px == 5){
            carros[i].x = 18;
            carros[i].lastmove = '<';
        }
        else if(px == 6){
            carros[i].x = 21;
            carros[i].lastmove = '<';
        }
        else if(px == 7){
            carros[i].x = 24;
            carros[i].lastmove = '<';
        }
        else if(px == 8)
        {
            carros[i].x = 27;
            carros[i].lastmove = '>';
        } 
        else if(px == 9){
            carros[i].x = 27;
            carros[i].lastmove = '>';
        }

        if(py == 0) carros[i].y = 2;
        else if(py == 1) carros[i].y = 6;
        else if(py == 2) carros[i].y = 14;
        else if(py == 3) carros[i].y = 21;
        else if(py == 4) carros[i].y = 26;
        else if(py == 5) carros[i].y = 30;
        else if(py == 6) carros[i].y = 34;
        else if(py == 7) carros[i].y = 18;
        else if(py == 9) carros[i].y = 18;
        carros[i].velocidade = 1;
        carros[i].parado = false;
        initPilha(&carros[i].movimentos);
        preencher(&carros[i].movimentos, &carros[i]);
    }
    //printcar(carros);
}    

// Simula a movimentação dos carros e o estado dos semáforos por um determinado tempo
// Parâmetros:
// - carros: array de carros
// - semaforos: array de semáforos
// - tempo_simulacao: tempo em segundos da simulação
void simularCarros(Carro *carros, Semaforo *semaforos, int tempo_simulacao)
{
    srand(time(0)); // Inicializa o gerador de números aleatórios
    int firstmovecars = 1; // Para setar o primeiro movimento dos carros somente uma vez

    while (true)
    {
        atualizarSemaforos(semaforos);                              // Atualiza os semáforos
        char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA];
        inicializarMatriz(matriz);                                  // Inicializa a matriz com pontos e vértices
        atualizarMatriz(matriz, carros, semaforos);                 // Atualiza a matriz com carros e semáforos
        imprimirMatriz(matriz, semaforos);
        
        //if(firstmovecars) 
        //{
            //firstmovecar(&carros[0], matriz);
            //firstmovecars = 0;
        //}

        for (int i = 0; i < QTD_CARROS; i++){
        
        MoverCarro(&carros[i], carros, semaforos, matriz);
        
               // Limpa o terminal

         // Imprime a matriz
        sleep(1);               // Aguarda 1 segundo antes de atualizar novamente
        system("clear"); 
        }
    }
}

int main()
{
    Carro carros[QTD_CARROS] = {
        
    };
    addCar(carros);
    Semaforo semaforos[QTD_SEMAFOROS];

    // Inicializa os semáforos
    // OBS: Os eixos x e y parecem estar invertidos, não encontrei a origem do problema, por isso optei por mantê-los assim
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
            semaforos[i].tempo_verde = t_verde;
            semaforos[i].tempo_vermelho = t_vermelho;
            semaforos[i].contador = 0;
            semaforos[i].estado_verde = true;
            i++;
        }   
    }
    
    int tempo_simulacao = 30;                          // Tempo total de simulação em segundos
    simularCarros(carros, semaforos, tempo_simulacao); // Inicia a simulação

    return 0;
}