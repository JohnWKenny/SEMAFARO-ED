#ifndef STACKBIB_H
#define STACKBIB_H

//Bibliotecas
#include <stdio.h>

#define LEN_MAX 100

//Definição para pilha
typedef struct {
    int items[LEN_MAX];
    int top;
} Pilha;

//Tamanho da pilha
int TAM = 0;

//Funçao para inicializar a pilha
void initPilha(Pilha* p) { p->top = -1; }

//Funçao para verificar se a pilha esta cheia
int isFull(Pilha* p) { return p->top == LEN_MAX - 1; }

//Funçao para verificar se a pilha esta vazia
int isEmpty(Pilha* p) { return p->top == -1; }

//Funçao para empilhar (push) um elemento
void push(Pilha* p, int valor) 
{
    if (isFull(p)) 
    {
        printf("Erro: Pilha cheia!\n");
        return;
    }
    
    p->items[++(p->top)] = valor;
}

//Funçao para desempilhar (pop) um elemento
int pop(Pilha* p) 
{
    if (isEmpty(p)) 
    {
        printf("Erro: Pilha vazia!\n");
        return -1;  // Valor de erro, já que a pilha esta vazia
    }
    
    return p->items[(p->top)--];
}

//Funçao para visualizar o elemento no topo da pilha
int topPilha(Pilha* p) 
{
    if (isEmpty(p)) 
    {
        printf("Erro: Pilha vazia!\n");
        return -1;
    }
    
    return p->items[p->top];
}


#endif