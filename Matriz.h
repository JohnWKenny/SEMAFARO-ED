#ifndef Matriz_h
#define Matriz_h

#include "Structs.h" // Inclui a definição de estruturas necessárias

FILE *logFile; // Ponteiro para o arquivo onde os registros serão escritos
cJSON *json; // Ponteiro para o objeto JSON
cJSON *simulacao;
        
// Declaração de variáveis externas para coordenadas de incidentes
extern int EntradaIncidentes_1_x, EntradaIncidentes_1_y;
extern int EntradaIncidentes_2_x, EntradaIncidentes_2_y;
extern int ativarfluxo, exportarFluxo, semaforo_x, semaforo_y, tempo_simulacao;
extern char face;
extern Semaforo semaforos[QTD_SEMAFOROS];

/*{ FUNÇÕES }*/

int fluxo(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], char face) {
    int resultadofluxo = 0; // Inicializar o resultadofluxo
    switch (face) {
        case 'v': // Vertical
            for (int i = -2; i <= 2; i++) {
                if (matriz[semaforo_x + i][semaforo_y] == 'C') resultadofluxo++;
            }
            break; 
            
        case 'h': // Horizontal
            for (int i = -3; i <= 3; i++) {
                if (matriz[semaforo_x][semaforo_y + i] == 'C') resultadofluxo++;
            }
            break; 
        
        case 't': // Tudo
        {
            bool zero = false;
            for (int i = -2; i <= 2; i++) {
                if (i == 0 && matriz[semaforo_x + i][semaforo_y] == 'C') zero = true;
                if (matriz[semaforo_x + i][semaforo_y] == 'C') resultadofluxo++;
            }
            for (int i = -3; i <= 3; i++)
            {
                if (i == 0 && zero) continue;
                if (matriz[semaforo_x][semaforo_y + i] == 'C') resultadofluxo++;
            }
            break;
        }
            
    }

    return resultadofluxo;
}

void gerarLog(char comand, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], int tempo) 
{
    if (comand == 's')
    {
        json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "Projeto", "Sistema de controle de tráfego de uma cidade com evento indesejado");
        cJSON *developers = cJSON_CreateArray();
        cJSON_AddItemToArray(developers, cJSON_CreateString("Davi William de Almeida Santos"));
        cJSON_AddItemToArray(developers, cJSON_CreateString("John Wallex Kennedy Moreira Silva"));
        cJSON_AddItemToArray(developers, cJSON_CreateString("João Gabriel Seixas Santos"));
        cJSON_AddItemToArray(developers, cJSON_CreateString("José Cícero de Oliveira Rodrigues"));
        cJSON_AddItemToObject(json, "Desenvolvedores", developers);
        cJSON_AddNumberToObject(json, "Versão", 1);
        simulacao = cJSON_CreateArray();
    }
    else if (comand == 'c')
    {
        cJSON *objeto = cJSON_CreateObject();
        cJSON_AddNumberToObject(objeto, "tempo", tempo);
        cJSON *sinais = cJSON_CreateArray();
        
        for (int i = 0; i < QTD_SEMAFOROS; i++)
        {
            cJSON *sinal = cJSON_CreateObject();
            cJSON_AddNumberToObject(sinal, "id", i);
            cJSON_AddNumberToObject(sinal, "x", semaforos[i].x);
            cJSON_AddNumberToObject(sinal, "y", semaforos[i].y);
            cJSON_AddNumberToObject(sinal, "fluxo_vertical", fluxo(matriz, 'v'));
            cJSON_AddNumberToObject(sinal, "fluxo_horizontal", fluxo(matriz, 'h'));
            cJSON_AddNumberToObject(sinal, "fluxo_total", fluxo(matriz, 't'));
            cJSON_AddItemToArray(sinais, sinal);
        }
        
        cJSON_AddItemToObject(objeto, "Semáforos", sinais);
        cJSON_AddItemToArray(simulacao, objeto);
    }
    else if (comand == 'e')
    {
        cJSON_AddItemToObject(json, "Simulação", simulacao);
        char *string;
        string = cJSON_Print(json);
        logFile = fopen("log.json", "w");
        fprintf(logFile, "%s", string);
        fclose(logFile);
        free(string);
        cJSON_Delete(json);
    }
}


// Função que inicializa a matriz da cidade, colocando pontos (.) em todas as posições e definindo as estradas com seus respectivos símbolos.
// Parâmetros:
// - matriz: matriz representando a cidade
void inicializarMatriz(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA])
{
    // Inicializa todas as posições da matriz com um ponto (representando vazio)
    for (int i = 0; i < TAMANHO_CIDADE_LINHA; i++)
        for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++)
            matriz[i][j] = '.'; // Todas as posições são inicialmente vazias

    // Desenho das estradas na matriz, com símbolos diferentes dependendo da direção e sentido
    for (int i = 0; i < QTD_ESTRADAS; i++) 
    {
        if (estradas[i].direcao) 
        { // Se a estrada é horizontal
            for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++) 
                // Define o símbolo da estrada com base em se é dupla ou seu sentido
                matriz[estradas[i].ini][j] = estradas[i].ehDupla ? '-' : (estradas[i].sentido ? '<' : '>');
        } 
        else 
        { // Se a estrada é vertical
            for (int j = 0; j < TAMANHO_CIDADE_LINHA; j++) 
                // Define o símbolo da estrada vertical com base em se é dupla ou seu sentido
                matriz[j][estradas[i].ini] = estradas[i].ehDupla ? '|' : (estradas[i].sentido ? 'V' : '^');
        }
    }

    // Marca as posições de incidentes com o símbolo 'A'
    for(int x = EntradaIncidentes_1_x; x <= EntradaIncidentes_2_x; x += 3){
        for(int y = EntradaIncidentes_1_y; y <= EntradaIncidentes_2_y; y += 4){
            if(x == EntradaIncidentes_1_x){
                if(x-2 >= 0)
                    matriz[x - 2][y] = 'A'; // Marca posição acima do primeiro ponto de incidente
            }
            if(x == EntradaIncidentes_2_x){
                if(x + 2 <= TAMANHO_CIDADE_LINHA)
                    matriz[x + 2][y] = 'A'; // Marca posição abaixo do segundo ponto de incidente
            }
            if(y == EntradaIncidentes_1_y){
                if(y - 3 >= 0)
                    matriz[x][y - 3] = 'A'; // Marca posição à esquerda do primeiro ponto de incidente
            }
            if(y == EntradaIncidentes_2_y){
                if(y + 3 <= TAMANHO_CIDADE_COLUNA)
                    matriz[x][y + 3] = 'A'; // Marca posição à direita do segundo ponto de incidente
            }
        }
    }

    // Marca os vértices (pontos de conexão das estradas) nas extremidades da matriz
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
            if (matriz[i][j] != '|' && matriz[i][j] != '-' && matriz[i][j] != 'V' && matriz[i][j] != '>' &&
                matriz[i][j] != '<' && matriz[i][j] != 'v' && matriz[i][j] != '^' && matriz[i][j] != 'A')
                matriz[i][j] = '.'; // Restaura a célula para vazio se não é uma estrada ou incidente

    // Atualiza as posições dos semáforos com base no estado (verde ou vermelho)
    for (int i = 0; i < QTD_SEMAFOROS; i++){
        matriz[semaforos[i].x][semaforos[i].y] = semaforos[i].estado_verde ? 'G' : 'R'; // 'G' para verde e 'R' para vermelho
        if(semaforos[i].alerta) 
            matriz[semaforos[i].x][semaforos[i].y] = 'Q'; // 'Q' para semáforo com alerta
    }

    // Atualiza as posições dos carros na matriz
    for (int i = 0; i < QTD_CARROS; i++)
        if(!carros[i].desativado) matriz[carros[i].x][carros[i].y] = 'C'; // 'C' representa um carro
}

// Imprime a matriz da cidade no terminal com carros, semáforos e ruas
// Parâmetros:
// - matriz: matriz representando a cidade
void imprimirMatriz(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], Semaforo *semaforos)
{
    if(ativarfluxo){
        printf("Fluxo do semaforo linha %d coluna %d:\t %d\n\n",semaforo_x, semaforo_y, fluxo(matriz, face));
    }

    for (int i = 0; i < TAMANHO_CIDADE_LINHA; i++)
    {
        for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++)
        {
            char celula = matriz[i][j]; // Obtém o valor da célula atual
            if (celula == 'C')
                printf("🚗 "); // Representa um carro
            else if (celula == 'v')
                printf("● "); // Representa um vértice
            else if (celula == 'A')
                printf("🚧 "); // Representa um incidente
            else if (celula == 'G')
                printf("🟢 "); // Representa um semáforo verde
            else if(celula == 'Q'){
                printf("⚪ "); // Representa um semáforo em alerta
            }
            else if (celula == 'R'){ // Verifica se a célula é um semáforo vermelho
                for(int indice = 0;indice < QTD_SEMAFOROS;indice++)
                {
                    if(semaforos[indice].x == i && semaforos[indice].y == j)
                        if (semaforos[indice].contador < 1)
                            printf("🟡 "); // Representa um semáforo amarelo
                        else
                            printf("🔴 "); // Representa um semáforo vermelho
                }       
            }         
            else
                printf("%c ", celula); // Qualquer outra célula é impressa como está
        }

        printf("\n"); // Nova linha após imprimir uma linha da matriz
    }

    printf("\n"); // Linha em branco após a matriz
}
#endif