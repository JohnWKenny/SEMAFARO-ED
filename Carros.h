#ifndef Carros_h
#define Carros_h

#include "Structs.h"
#include "Matriz.h"
#define QTD_CARROS 100           // Define a quantidade de carros

// Função para verificar se uma posição específica (x, y) está ocupada por um carro
bool posicaoOcupada(int x, int y, Carro *carros)
{
    int count = 0; // Inicializa um contador para carros na posição

    // Loop através dos carros
    for (int i = 0; i < QTD_CARROS; i++)
    {
        // Verifica se a posição é ocupada por um carro parado
        if (carros[i].x == x && carros[i].y == y && carros[i].parado) 
            count++; // Incrementa o contador se encontrado um carro parado

        // Se mais de um carro estiver na mesma posição, retorna verdadeiro
        if (count > 1)
            return true;
    }

    return false; // Retorna falso se a posição não estiver ocupada
}

// Função para preencher a pilha de movimentos do carro com base no seu ID
void preencher(Pilha *movimentos, Carro *carro)
{
    // Adiciona movimentos diferentes à pilha dependendo do ID do carro
    if((carro->id % 10 == 0 || carro->id % 10 == 1) && carro->id < 10)
    {
        push(movimentos, '>'); // Adiciona movimento para direita
        push(movimentos, 'V'); // Adiciona movimento para baixo
        push(movimentos, '<'); // Adiciona movimento para esquerda
        push(movimentos, '^'); // Adiciona movimento para cima
    }
    else if(carro->id % 10 == 2)
    {
        push(movimentos, '<'); // Adiciona movimento para esquerda
        push(movimentos, 'V'); // Adiciona movimento para baixo
        push(movimentos, '>'); // Adiciona movimento para direita
        push(movimentos, '^'); // Adiciona movimento para cima
    }
    // Outras condições de ID seguem o mesmo padrão...
    else
    {
        push(movimentos, 'V'); // Se não se enquadrar em nenhum caso anterior, adiciona movimento para baixo
        push(movimentos, '<'); // Adiciona movimento para esquerda
        push(movimentos, '^'); // Adiciona movimento para cima
        push(movimentos, '>'); // Adiciona movimento para direita
    }
}

// Função para definir o movimento do carro com base nos semáforos e na matriz da cidade
void DefinirMovimentoNoSemaforo(Carro *carro, Carro *carros, Semaforo *semaforos, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    // Se a pilha de movimentos do carro estiver vazia, preenche-a
    if (isEmpty(&carro->movimentos))
    {
        preencher(&carro->movimentos, carro);
    }

    char movimentodocarro = pop(&carro->movimentos); // Pega o próximo movimento da pilha

    // Verifica se o último movimento foi para cima ou para baixo e se está em uma interseção
    if((carro->lastmove == 'V' || carro->lastmove == '^') && (matriz[carro->x][carro->y] == 'R' || matriz[carro->x ][carro->y]== 'v')){
        while(1){
            switch(movimentodocarro){ // Verifica o movimento a ser realizado
                case 94: // ^ (cima)
                    // Verifica se a nova posição está ocupada ou não pode ser acessada
                    if(posicaoOcupada(carro->x - carro->velocidade, carro->y, carros) ||
                    matriz[carro->x - carro->velocidade][carro->y] != '^' &&
                    matriz[carro->x - carro->velocidade][carro->y]!='|'){
                        // Se a pilha de movimentos estiver vazia, preenche-a novamente
                        if (isEmpty(&carro->movimentos))
                        {
                            preencher(&carro->movimentos, carro);
                        }
                        movimentodocarro = pop(&carro->movimentos); // Pega o próximo movimento
                        continue; // Continua o loop
                    }
                    else{
                        carro->lastmove = '^'; // Atualiza a última direção do carro
                        carro->parado = false; // O carro não está mais parado
                        return; // Sai da função
                    }    
                // Condições semelhantes para os outros movimentos (baixo, direita, esquerda)
                case 86: // v (baixo)
                    if(posicaoOcupada(carro->x + carro->velocidade, carro->y, carros) ||
                    matriz[carro->x + carro->velocidade][carro->y] != 'V' &&
                    matriz[carro->x + carro->velocidade][carro->y]!='|'){
                        if (isEmpty(&carro->movimentos))
                        {
                            preencher(&carro->movimentos, carro);
                        }
                        movimentodocarro = pop(&carro->movimentos);
                        continue;
                    }
                    else{
                        carro->lastmove = 'V'; 
                        carro->parado = false; 
                        return;
                    }
                case 62: // > (direita)
                    if(posicaoOcupada(carro->x, carro->y + carro->velocidade, carros) ||
                    matriz[carro->x][carro->y + carro->velocidade] != '>' &&
                    matriz[carro->x][carro->y + carro->velocidade]!='-'){
                        if (isEmpty(&carro->movimentos))
                        {
                            preencher(&carro->movimentos, carro);
                        }
                        movimentodocarro = pop(&carro->movimentos);
                        continue;
                    }
                    else{
                        carro->lastmove = '>';
                        carro->parado = false; 
                        return;
                    }
                case 60: // < (esquerda)
                    if(posicaoOcupada(carro->x, carro->y - carro->velocidade, carros) ||
                    matriz[carro->x][carro->y - carro->velocidade] != '<' && 
                    matriz[carro->x][carro->y - carro->velocidade]!='-'){
                        if (isEmpty(&carro->movimentos))
                        {
                            preencher(&carro->movimentos, carro);
                        }
                        movimentodocarro = pop(&carro->movimentos);
                        continue;
                    }
                    else{
                        carro->lastmove = '<';
                        carro->parado = false; 
                        return;
                    }
            }
        }
    }
    // O mesmo padrão se repete para verificar movimentos à direita e esquerda
    else{
        carro->parado = true; // Se não puder se mover, o carro fica parado
        return; // Sai da função
    }
}

// Função para mover um carro baseado na sua última direção e semáforos
void MoverCarro(Carro *carro, Carro *carros, Semaforo *semaforos, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA]) 
{
    // Se o carro não está parado, realiza o próximo movimento
    if(carro->parado== true) carro->parado = false; // Libera o carro que estava parado

    if (carro->parado == false) {
        switch (carro->lastmove) {
            case '^': // Mover para cima
                if (carro->x - carro->velocidade >= 0) { // Verifica se o movimento está dentro dos limites
                    if (matriz[carro->x - carro->velocidade][carro->y] == 'G') {
                        carro->parado = true; // Para se encontra um semáforo verde
                        return; // Sai da função
                    } else if (matriz[carro->x - carro->velocidade][carro->y] == 'R' ||
                    matriz[carro->x - carro->velocidade][carro->y] == 'v') {
                        carro->x -= carro->velocidade; // Move o carro para cima
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
                    } else {
                        if(matriz[carro->x - carro->velocidade][carro->y] != 'C'){ // Verifica se não é um carro
                            carro->x -= carro->velocidade; // Move o carro
                        }
                    
                        carro->parado= true; // Para o carro se não puder se mover
                        return;
                    }
                }
            // Condições semelhantes para os outros movimentos (baixo, direita, esquerda)
            case 'V': // Mover para baixo
                if (carro->x + carro->velocidade < TAMANHO_CIDADE_LINHA) {
                    if (matriz[carro->x + carro->velocidade][carro->y] == 'G') {
                        carro->parado = true;
                        return;
                    } else if (matriz[carro->x + carro->velocidade][carro->y] == 'R' ||
                    matriz[carro->x + carro->velocidade][carro->y] == 'v') {
                        carro->x += carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz);
                        return;
                    } else {
                        if(matriz[carro->x + carro->velocidade][carro->y] != 'C'){
                            carro->x += carro->velocidade;
                        }
                        carro->parado = true;
                        return;
                    }
                }
            case '>': // Mover para direita
                if (carro->y + carro->velocidade < TAMANHO_CIDADE_COLUNA) {
                    if (matriz[carro->x][carro->y + carro->velocidade] == 'G') {
                        carro->parado = true;
                        return;
                    } else if (matriz[carro->x][carro->y + carro->velocidade] == 'R' ||
                    matriz[carro->x][carro->y + carro->velocidade] == 'v') {
                        carro->y += carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz);
                        return;
                    } else {
                        if(matriz[carro->x][carro->y + carro->velocidade] != 'C'){
                            carro->y += carro->velocidade;
                        }
                        carro->parado = true;
                        return;
                    }
                }
            case '<': // Mover para esquerda
                if (carro->y - carro->velocidade >= 0) {
                    if (matriz[carro->x][carro->y - carro->velocidade] == 'G') {
                        carro->parado = true;
                        return;
                    } else if (matriz[carro->x][carro->y - carro->velocidade] == 'R' ||
                    matriz[carro->x][carro->y - carro->velocidade] == 'v') {
                        carro->y -= carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz);
                        return;
                    } else {
                        if(matriz[carro->x][carro->y - carro->velocidade] != 'C'){
                            carro->y -= carro->velocidade;
                        }
                        carro->parado = true;
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
        
        if(px == 1){
            carros[i].x = 3;
            carros[i].lastmove = '>';
        }
        else if(px == 2){
            carros[i].x = 6;
            carros[i].lastmove = '>';
        }
        else if(px == 3){
            continue;
        }
        else if(px == 4){
            carros[i].x = 12;
            carros[i].lastmove = '>';
        }
        else if(px == 0 || px == 5){
            carros[i].x = 15;
            carros[i].lastmove = '>';
        }
        else if(px == 6){
            carros[i].x = 18;
            carros[i].lastmove = '<';
        }
        else if(px == 7){
            carros[i].x = 21;
            carros[i].lastmove = '<';
        }
        else if(px == 8)
        {
            carros[i].x = 24;
            carros[i].lastmove = '>';
        } 
        else if(px == 9){
            if (py <=9)
            {
                carros[i].x = 27;
                carros[i].lastmove = '<';
            }
            else{
                continue;
            }
        }
        
        if (px!= 3 && py != 9)
        {
            if(py == 1) carros[i].y = 6;
            else if(py == 2) carros[i].y = 10;
            else if(py == 3) carros[i].y = 14;
            else if(py == 4) carros[i].y = 18;
            else if(py == 0 || py == 5) carros[i].y = 22;
            else if(py == 6) carros[i].y = 26;
            else if(py == 7) carros[i].y = 30;
            else carros[i].y = 34;
        }

        
        carros[i].velocidade = 1;
        carros[i].parado = false;
        initPilha(&carros[i].movimentos);
        preencher(&carros[i].movimentos, &carros[i]);
    }
}
#endif