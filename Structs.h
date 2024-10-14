#ifndef STRUCTS_H
#define STRUCTS_H

#define QTD_ESTRADAS 17
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

/*{ ESTRUTURAS }*/ 
// Definição da estrutura que representa um carro.
typedef struct {
    int id;       // Identificador do carro
    int x, y;     // Coordenadas X e Y na cidade (matriz)
    int viaDupla; // Indica se o carro está em uma pista dupla e guarda qual o sentido da via
    int direcaoCarro;
    Pilha movimentos;  // Pilha que guarda os movimentos do carro
    int lastmove;
    int velocidade;
    bool parado;
} Carro;

// Definição da estrutura para um semáforo.
typedef struct {
    int x, y;           // Coordenadas X e Y do semáforo na matriz
    int tempo_verde;    // Duração do sinal verde
    int tempo_vermelho; // Duração do sinal vermelho
    int contador;       // Contador que rastreia o tempo que o semáforo está em cada estado
    bool estado_verde;  // Estado do semáforo: true para verde, false para vermelho
    bool alerta;        // Estado do Semáforo: true para estado de alerta, false para estado normal
} Semaforo;

// Definição da estrutura para estradas.
typedef struct {
    bool direcao; // 0 para vertical, 1 para horizontal
    bool sentido; // 0 para direita/cima, 1 para esquerda/baixo
    bool ehDupla; // Indica se é uma pista dupla
    int ini;      // Ponto inicial da estrada (linha ou coluna, dependendo da direção)
} Estrada;

// Configuração das estradas
extern Estrada estradas[QTD_ESTRADAS];

#endif // STRUCTS_H
