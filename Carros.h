#ifndef Carros_h
#define Carros_h

#include "Structs.h"
#include "Matriz.h"

// Função para verificar se uma posição específica (x, y) está ocupada por um carro
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

// Atualiza o estado dos semáforos com base no tempo
// Parâmetros:
// - semaforos: array de semáforos


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
        if((carro->lastmove == 'V' || carro->lastmove == '^') && (matriz[carro->x][carro->y] == 'R' ||
        matriz[carro->x ][carro->y]== 'v')){
            while(1){
                switch(movimentodocarro){
                    case 94: //^
                        if(posicaoOcupada(carro->x -carro->velocidade, carro->y, carros) ||
                        matriz[carro->x-carro->velocidade][carro->y] != '^' &&
                        matriz[carro->x-carro->velocidade][carro->y]!='|'){
                            if (isEmpty(&carro->movimentos))
                            {
                            preencher(&carro->movimentos, carro);
                            }
                            movimentodocarro = pop(&carro->movimentos);
                            continue;
                        }
                        else{
                            carro->lastmove = '^';
                            carro->parado = false; 
                            return;
                        }    
                    case 86: //v
                        if(posicaoOcupada(carro->x +carro->velocidade, carro->y, carros)||
                        matriz[carro->x+carro->velocidade][carro->y] != 'V' &&
                        matriz[carro->x+carro->velocidade][carro->y]!='|'){
                                if (isEmpty(&carro->movimentos))
                                {
                                preencher(&carro->movimentos, carro);
                                }
                                movimentodocarro = pop(&carro->movimentos);
                                continue;                            }
                        else{
                                carro->lastmove = 'V';
                                carro->parado = false; 
                                return;
                        }
                    case 62: //>
                        if(posicaoOcupada(carro->x, carro->y + carro->velocidade, carros)||
                        matriz[carro->x][carro->y+carro->velocidade] != '>' &&
                        matriz[carro->x][carro->y+carro->velocidade]!='-'){
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
                    case 60: //<
                        if(posicaoOcupada(carro->x, carro->y-carro->velocidade, carros)||
                        matriz[carro->x][carro->y-carro->velocidade] != '<'&& 
                        matriz[carro->x][carro->y-carro->velocidade]!='-'){
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
        else if((carro->lastmove == '>'|| carro->lastmove =='<') && (matriz[carro->x][carro->y]== 'G'||
        matriz[carro->x][carro->y]== 'v')){
            while(1){
                    switch(movimentodocarro){
                        case 94: //^
                            if(posicaoOcupada(carro->x-carro->velocidade, carro->y, carros)||
                            matriz[carro->x - carro->velocidade][carro->y] != '^'&&
                            matriz[carro->x - carro->velocidade][carro->y]!='|'){
                                if (isEmpty(&carro->movimentos))
                                {
                                    preencher(&carro->movimentos, carro);
                                }
                                movimentodocarro = pop(&carro->movimentos);
                                continue;
                            }
                            else{
                                carro->lastmove = '^';
                                carro->parado = false; 
                                return;
                            }    
                        case 86: //v
                            if(posicaoOcupada(carro->x +carro->velocidade, carro->y, carros) ||
                            matriz[carro->x +carro->velocidade][carro->y] != 'V'&&
                            matriz[carro->x +carro->velocidade][carro->y]!='|'){
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
                        case 62: //>
                            if(posicaoOcupada(carro->x, carro->y +carro->velocidade, carros)||
                            matriz[carro->x][carro->y +carro->velocidade] != '>'&&
                            matriz[carro->x][carro->y +carro->velocidade]!='-'){
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
                        case 60: //<
                            if(posicaoOcupada(carro->x, carro->y -carro->velocidade, carros)||
                            matriz[carro->x][carro->y -carro->velocidade] != '<' &&
                            matriz[carro->x][carro->y -carro->velocidade]!='-'){
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
    else{
        carro->parado = true;
        return;
    }
}

void MoverCarro(Carro *carro, Carro *carros, Semaforo *semaforos, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA]) 
{
    // Se o carro não está parado, realiza o próximo movimento
    if(carro->parado== true) carro-> parado = false;
    if (carro->parado == false) {
        switch (carro->lastmove) {
            case '^': // Mover para cima
                if (carro->x - carro->velocidade >= 0) {
                    if (matriz[carro->x - carro->velocidade][carro->y] == 'G') {
                        carro->parado = true;      
                        return; // Carro encontra semáforo verde, parar
                    } else if (matriz[carro->x - carro->velocidade][carro->y] == 'R' ||
                    matriz[carro->x - carro->velocidade][carro->y] == 'v') {
                        carro->x -= carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
                    } else {
                        if(matriz[carro->x - carro->velocidade][carro->y] != 'C'){
                            carro->x -= carro->velocidade;
                        }
                    
                        carro->parado= true;
                        return;
                    }
                }

            case 'V': // Mover para baixo
                if (carro->x + carro->velocidade < TAMANHO_CIDADE_LINHA) {
                    if (matriz[carro->x + carro->velocidade][carro->y] == 'G') {
                        carro->parado = true;      
                        return; // Carro encontra semáforo verde, parar
                    } else if (matriz[carro->x + carro->velocidade][carro->y] == 'R'||
                    matriz[carro->x + carro->velocidade][carro->y] == 'v') {
                        carro->x += carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
                    } else {
                        if(matriz[carro->x + carro->velocidade][carro->y] != 'C'){
                            carro->x += carro->velocidade;
                        }
                        
                        carro->parado=true;
                        return;
                    }
                }

            case '>': // Mover para direita
                if (carro->y + carro->velocidade < TAMANHO_CIDADE_COLUNA) {
                    if (matriz[carro->x][carro->y + carro->velocidade] == 'R') {
                        carro->parado = true;      
                        return; // Carro encontra semáforo verde, parar
                    } else if (matriz[carro->x][carro->y + carro->velocidade] == 'G'||
                    matriz[carro->x][carro->y+ carro->velocidade] == 'v') {
                        carro->y += carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
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
                    if (matriz[carro->x][carro->y - carro->velocidade] == 'R'/*lembra do carro*/) {
                        carro->parado = true;      
                        return; // Carro encontra semáforo verde, parar
                    } else if (matriz[carro->x][carro->y - carro->velocidade] == 'G'||
                    matriz[carro->x][carro->y - carro->velocidade] == 'v') {
                        carro->y -= carro->velocidade;
                        DefinirMovimentoNoSemaforo(carro, carros, semaforos, matriz); // Chama a função para lidar com o semáforo
                        return; // Para no semáforo
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
            carros[i].desativado = true;
            continue;
        }
        else if(px == 4){
            carros[i].x = 12;
            carros[i].lastmove = '>';
        }
        else if(px == 5 || px == 0){
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
            carros[i].x = 27;
            carros[i].lastmove = '<';
        }
        
        if(py == 0) py = 5;
        if(py == 9) py = 0;
        
        int isDesativate = 0;
        
        for(int y = py * 4; y + 1 < (py + 1) * 4; y++)
        {
            int isOcupped = 0;
            isDesativate = 1;
            
            for(int teste = 0; teste < QTD_CARROS;teste++)
            {
                if(carros[i].x == carros[teste].x && carros[teste].y == y + 1)
                {
                    isOcupped = 1;
                    break;
                }    
            }
            
            if(!isOcupped) 
            {
                isDesativate = 0;
                carros[i].y = y + 1;
                break;
            }
            
        }
        if(isDesativate) carros[i].desativado = true;
        else carros[i].desativado = false;
        
        carros[i].velocidade = 1;
        carros[i].parado = false;
        initPilha(&carros[i].movimentos);
        preencher(&carros[i].movimentos, &carros[i]);
    }
}   
#endif