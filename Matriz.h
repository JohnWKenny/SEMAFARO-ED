#ifndef Matriz_h
#define Matriz_h

#include "Structs.h" // Inclui a definição de estruturas necessárias

FILE *logFile; // Ponteiro para o arquivo onde os registros serão escritos
cJSON *json;   // Ponteiro para o objeto JSON
cJSON *simulacao;

// Declaração de variáveis externas para coordenadas de incidentes
extern int entrada_incidentes_1_x, entrada_incidentes_1_y;
extern int entrada_incidentes_2_x, entrada_incidentes_2_y;
extern int ativar_fluxo, exportar_fluxo, semaforo_x, semaforo_y, tempo_simulacao;
extern char face;

// Cria um array de semáforos para a simulação
extern Semaforo semaforos[QTD_SEMAFOROS];

/*{ FUNÇÕES }*/

// Função que retorna o fluxo (quantidade de carros) nos semáforos
// Parâmetros:
// - matriz: matriz representando a cidade
// - x: cordenada x do semáforo 
// - y: cordenada y do semáforo 
// - face: a face do semáforo ao qual se deseja obter o fluxo ("v": vertical, "h": horizontal, "t": ambas as faces)
int fluxo(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], int x, int y, char face) {
    int resultadofluxo = 0; // Inicializar o resultadofluxo
    switch (face) {
        case 'v': // Vertical
            for (int i = -2; i <= 2; i++) {
                // Percorre de duas casas a cima até duas abaixo do semáforo verificando se existe algum carro
                if (matriz[x + i][y] == 'C') resultadofluxo++;
            }
            break; 
            
        case 'h': // Horizontal
            for (int i = -3; i <= 3; i++) {
                // Percorre de três casas à esquerda até três à direita do semáforo verificando se existe algum carro
                if (matriz[x][y + i] == 'C') resultadofluxo++;
            }
            break; 
        
        case 't': // Ambas as faces
        {
            bool zero = false; // Serve para indicar se existe um carro no semáforo, evitando redundância na contagem

            for (int i = -2; i <= 2; i++) {
                // Percorre de duas casas a cima até duas abaixo do semáforo verificando se existe algum carro
                if (i == 0 && matriz[x + i][y] == 'C') zero = true;
                if (matriz[x + i][y] == 'C') resultadofluxo++;
            }
            for (int i = -3; i <= 3; i++)
            {
                // Percorre de três casas à esquerda até três à direita do semáforo verificando se existe algum carro
                if (i == 0 && zero) continue;
                if (matriz[x][y + i] == 'C') resultadofluxo++;
            }
            break;
        }
            
    }

    return resultadofluxo; // Retorna a quantidade de carros encontrada em relação ao que foi solicitado
}

// Função que gera um arquivo de log com o fluxo de todos os semáforos durante toda a simulação
// Parâmetros:
// - comand: indica se será gerado o início('s'), conteúdo('c') ou final('e) do arquivo
// - matriz: matriz representando a cidade
// - tempo: o tempo atual da simulação
void gerarLog(char comand, char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], int tempo) 
{
    if (comand == 's') // Registra o cabeçalho do arquivo
    {
        json = cJSON_CreateObject(); // Cria um objeto JSON que será o objeto raiz do arquivo

        // São criados os dados que serão incluídos no cabeçalho: Projeto, Desenvolvedores e Versão
        cJSON_AddStringToObject(json, "Projeto", "Sistema de controle de tráfego de uma cidade com evento indesejado");
        cJSON *developers = cJSON_CreateArray();
        cJSON_AddItemToArray(developers, cJSON_CreateString("Davi William de Almeida Santos"));
        cJSON_AddItemToArray(developers, cJSON_CreateString("John Wallex Kennedy Moreira Silva"));
        cJSON_AddItemToArray(developers, cJSON_CreateString("João Gabriel Seixas Santos"));
        cJSON_AddItemToArray(developers, cJSON_CreateString("José Cícero de Oliveira Rodrigues"));
        cJSON_AddItemToObject(json, "Desenvolvedores", developers);
        cJSON_AddNumberToObject(json, "Versão", 1);

        simulacao = cJSON_CreateArray(); // Cria o array onde será registrada toda a simulação
    }
    else if (comand == 'c') // Registra o fluxo de todos os semáforos no tempo atual
    {
        cJSON *objeto = cJSON_CreateObject(); // Esse objeto guarda o estado atual da simulação
        cJSON_AddNumberToObject(objeto, "tempo", tempo); // Registra o tempo
        cJSON *sinais = cJSON_CreateArray(); // Array onde serão registrado o fluxo de todos os semáforos no tempo atual
        
        for (int i = 0; i < QTD_SEMAFOROS; i++) // Percorre todos os semáforos
        {
            cJSON *sinal = cJSON_CreateObject(); //Cria um objeto para registrar os dados do semáforo
            // Registra todos os dados necessários
            cJSON_AddNumberToObject(sinal, "id", i);
            cJSON_AddNumberToObject(sinal, "x", semaforos[i].x);
            cJSON_AddNumberToObject(sinal, "y", semaforos[i].y);
            cJSON_AddNumberToObject(sinal, "fluxo_vertical", fluxo(matriz, semaforos[i].x, semaforos[i].y, 'v'));
            cJSON_AddNumberToObject(sinal, "fluxo_horizontal", fluxo(matriz, semaforos[i].x, semaforos[i].y, 'h'));
            cJSON_AddNumberToObject(sinal, "fluxo_total", fluxo(matriz, semaforos[i].x, semaforos[i].y, 't'));

            cJSON_AddItemToArray(sinais, sinal); // Adiciona o semáforo no array de semáforos do tempo atual
        }
        
        cJSON_AddItemToObject(objeto, "Semáforos", sinais); // Adiciona o array com de semáforos objeto pai
        cJSON_AddItemToArray(simulacao, objeto); // Adiciona o obejto ao array que guarda o esado da simulação
    }
    else if (comand == 'e') // Finaliza o objeto JSON e escreve em um arquivo
    {
        cJSON_AddItemToObject(json, "Simulação", simulacao); // Adiciona o array com todos os dados ao objeto raiz
        
        // Copia o JSON para uma String
        char *string;
        string = cJSON_Print(json);

        logFile = fopen("log.json", "w");  // Abre o arquivo de log em modo escrita (Cria o arquivo caso não exista)
        fprintf(logFile, "%s", string); // Escreve o objeto JSON no arquivo
        fclose(logFile); // Fecha o arquivo
        // Libera a memória que estava sendo usada pelo objeto JSON
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
                matriz[estradas[i].inicio_estrada][j] = estradas[i].via_dupla ? '-' : (estradas[i].sentido ? '<' : '>');
        }
        else
        { // Se a estrada é vertical
            for (int j = 0; j < TAMANHO_CIDADE_LINHA; j++)
                // Define o símbolo da estrada vertical com base em se é dupla ou seu sentido
                matriz[j][estradas[i].inicio_estrada] = estradas[i].via_dupla ? '|' : (estradas[i].sentido ? 'V' : '^');
        }
    }

    // Marca as posições de incidentes com o símbolo 'A'
    for (int i = 0; i < QTD_SEMAFOROS; i++)
    {
        matriz[semaforos[i].x][semaforos[i].y] = semaforos[i].estado_verde ? 'G' : 'R'; // 'G' para verde e 'R' para vermelho
        if (semaforos[i].alerta)
        {
            matriz[semaforos[i].x][semaforos[i].y] = 'Q'; // 'Q' para semáforo com alerta
            for (int x = entrada_incidentes_1_x; x <= entrada_incidentes_2_x; x += 3)
            {
                for (int y = entrada_incidentes_1_y; y <= entrada_incidentes_2_y; y += 4)
                {
                    for (int i = 0; i <= 2; i++)
                    {
                        if (x - i >= 0)
                            matriz[x - i][y] = 'A'; // Marca posição acima do primeiro ponto de incidente

                        if (x + i <= TAMANHO_CIDADE_LINHA)
                            matriz[x + i][y] = 'A'; // Marca posição abaixo do segundo ponto de incidente
                    }
                    for (int i = 0; i <= 3; i++)
                    {
                        if (y - i >= 0)
                            matriz[x][y - i] = 'A'; // Marca posição à esquerda do primeiro ponto de incidente

                        if (y + i <= TAMANHO_CIDADE_COLUNA)
                            matriz[x][y + i] = 'A'; // Marca posição à direita do segundo ponto de incidente
                    }
                }
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
    for (int i = 0; i < QTD_SEMAFOROS; i++)
    {
        matriz[semaforos[i].x][semaforos[i].y] = semaforos[i].estado_verde ? 'G' : 'R'; // 'G' para verde e 'R' para vermelho
        if (semaforos[i].alerta)
            matriz[semaforos[i].x][semaforos[i].y] = 'Q'; // 'Q' para semáforo com alerta
    }

    // Atualiza as posições dos carros na matriz
    for (int i = 0; i < QTD_CARROS; i++)
        if (!carros[i].desativado)
            matriz[carros[i].x][carros[i].y] = 'C'; // 'C' representa um carro
}

// Imprime a matriz da cidade no terminal com carros, semáforos e ruas
// Parâmetros:
// - matriz: matriz representando a cidade
void imprimirMatriz(char matriz[TAMANHO_CIDADE_LINHA][TAMANHO_CIDADE_COLUNA], Carro *carros, Semaforo *semaforos)
{
    if (ativar_fluxo)
    {
        printf("Fluxo %s do semaforo linha %d coluna %d:\t %d\n\n", (face == 'v') ? "vertical" : "horizontal", semaforo_x, semaforo_y, fluxo(matriz, semaforo_x, semaforo_y, face));
    }

    for (int i = 0; i < TAMANHO_CIDADE_LINHA; i++)
    {
        for (int j = 0; j < TAMANHO_CIDADE_COLUNA; j++)
        {
            char celula = matriz[i][j]; // Obtém o valor da célula atual
            if (celula == 'C')
            {
                for (int indice = 0; indice < QTD_CARROS; indice++)
                {
                    if (carros[indice].x == i && carros[indice].y == j)
                    {
                        if (carros[indice].velocidade == 1)
                            printf("🚌 ");
                        else if (carros[indice].velocidade == 2)
                            printf("🚗 ");
                        else
                            printf("🚑 ");
                        break; // Interrompe o loop ao encontrar um carro na posição
                    }
                }
            }
            else if (celula == 'v')
                printf(" ● "); // Representa um vértice
            else if (celula == 'A')
                printf("🚧 "); // Representa um incidente
            else if (celula == 'G')
                printf("🟢 "); // Representa um semáforo verde
            else if (celula == 'Q')
            {
                printf("⚪ "); // Representa um semáforo em alerta
            }
            else if (celula == '>')
            {
                printf(" → ");
            }
            else if (celula == '<')
            {
                printf(" ← ");
            }
            else if (celula == '^')
            {
                printf(" ↑ ");
            }
            else if (celula == 'V')
            {
                printf(" ↓ ");
            }
            else if (celula == '-')
            {
                printf(" ⇄ ");
            }
            else if (celula == '|')
            {
                printf(" ⇅ ");
            }

            else if (celula == 'R')
            { // Verifica se a célula é um semáforo vermelho
                for (int indice = 0; indice < QTD_SEMAFOROS; indice++)
                {
                    if (semaforos[indice].x == i && semaforos[indice].y == j)
                    {
                        if (semaforos[indice].contador < 1)
                            printf("🟡 ");
                        else
                            printf("🔴 ");
                        break; // Interrompe o loop ao encontrar um semáforo na posição
                    }
                }
            }
            else
                printf(" %c ", celula); // Qualquer outra célula é impressa como está
        }

        printf("\n"); // Nova linha após imprimir uma linha da matriz
    }

    printf("\n"); // Linha em branco após a matriz
}
#endif