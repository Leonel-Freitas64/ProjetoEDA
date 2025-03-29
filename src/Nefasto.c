/**
 * @file Nefasto.c
 * @brief Implementação das funções relacionadas aos efeitos nefastos entre antenas.
 * @version 1.1
 * @date 29-03-2025
 */

#include "../resources/Estruturas.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * @brief Cria um novo efeito nefasto.
 * 
 * @param a1 Ponteiro para a primeira antena.
 * @param a2 Ponteiro para a segunda antena.
 * @param x1 Coordenada X inicial do efeito.
 * @param y1 Coordenada Y inicial do efeito.
 * @param x2 Coordenada X final do efeito.
 * @param y2 Coordenada Y final do efeito.
 * @return Nefasto* Ponteiro para o novo efeito nefasto ou NULL em caso de erro.
 */
Nefasto* criarEfeitoNefasto(Antena* a1, Antena* a2, int x1, int y1, int x2, int y2)
{
    // Aloca memória para o novo efeito nefasto
    Nefasto* novo = (Nefasto*)malloc(sizeof(Nefasto));
    if (!novo)
        return NULL;

    // Inicializa os campos do efeito nefasto
    novo->antena1 = a1;
    novo->antena2 = a2;
    novo->x1 = x1;
    novo->y1 = y1;
    novo->x2 = x2;
    novo->y2 = y2;
    novo->next = NULL;

    return novo;
}

/**
 * @brief Insere um novo efeito nefasto na lista.
 * 
 * @param listaNefastos Ponteiro para o ponteiro da lista de efeitos nefastos.
 * @param antena1 Ponteiro para a primeira antena.
 * @param antena2 Ponteiro para a segunda antena.
 * @param x1 Coordenada X inicial do efeito.
 * @param y1 Coordenada Y inicial do efeito.
 * @param x2 Coordenada X final do efeito.
 * @param y2 Coordenada Y final do efeito.
 */
void inserirNefasto(Nefasto** listaNefastos, Antena* antena1, Antena* antena2, int x1, int y1, int x2, int y2)
{
    // Cria um novo efeito nefasto
    Nefasto* novo = criarEfeitoNefasto(antena1, antena2, x1, y1, x2, y2);
    if (!novo)
    {
        printf("Erro ao criar efeito nefasto.\n");
        return;
    }

    // Insere o novo efeito no início da lista
    novo->next = *listaNefastos;
    *listaNefastos = novo;
}

/**
 * @brief Libera a memória da lista de efeitos nefastos.
 * 
 * @param listaNefastos Ponteiro para o ponteiro da lista de efeitos nefastos.
 */
void liberarEfeitosNefastos(Nefasto** listaNefastos)
{
    Nefasto* atual = *listaNefastos;
    while (atual)
    {
        Nefasto* temp = atual;
        atual = atual->next;
        free(temp); // Libera a memória do efeito atual
    }
    *listaNefastos = NULL; // Define a lista como vazia
}

/**
 * @brief Calcula os efeitos nefastos com base na lista de antenas.
 * 
 * @param lista Ponteiro para a lista de antenas.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @return Nefasto* Ponteiro para a lista de efeitos nefastos calculados.
 */
Nefasto* calcularEfeitoNefasto(Antena* lista, int linha, int coluna)
{
    Nefasto* listaNefastos = NULL;
    Antena* a1 = lista;

    // Percorre todas as combinações de antenas
    while (a1)
    {
        Antena* a2 = a1->next;
        while (a2)
        {
            // Verifica se as antenas têm a mesma frequência
            if (a1->frequencia == a2->frequencia)
            {
                int tipoEfeito = 0;
                int dx = abs(a2->x - a1->x);
                int dy = abs(a2->y - a1->y);
                int distancia = dx > dy ? dx : dy;

                // Determina o tipo de efeito nefasto com base na distância
                if (distancia <= 3)
                {
                    if (a1->x == a2->x)
                    {
                        tipoEfeito = 1; // Efeito vertical
                    }
                    else if (a1->y == a2->y)
                    {
                        tipoEfeito = 2; // Efeito horizontal
                    }
                    else if (dx + dy <= 4)
                    {
                        tipoEfeito = 3; // Efeito diagonal
                    }
                }

                switch (tipoEfeito)
                {
                case 1:
                    if (a1->x + distancia >= coluna || a2->x + distancia >= coluna)
                    {
                        /* code */
                    }
                    
                    if (a1->y < a2->y)
                    {
                        inserirNefasto(&listaNefastos, a1, a2, a1->x, a1->y - distancia, a2->x, a2->y + distancia);
                    }
                    else
                    {
                        inserirNefasto(&listaNefastos, a1, a2, a2->x, a2->y - distancia, a1->x, a1->y + distancia);
                    }
                    break;
                case 2:
                    if (a1->x < a2->x)
                    {
                        inserirNefasto(&listaNefastos, a1, a2, a1->x - distancia, a1->y, a2->x + distancia, a2->y);
                    }
                    else
                    {
                        inserirNefasto(&listaNefastos, a1, a2, a2->x - distancia, a2->y, a1->x + distancia, a1->y);
                    }
                    break;
                case 3:
                    // Top-left to bottom-right
                    if (a1->x < a2->x && a1->y < a2->y)
                    {
                        inserirNefasto(&listaNefastos, a1, a2, a1->x - dx, a1->y - dy, a2->x + dx, a2->y + dy);
                    }
                    // Bottom-left to top-right
                    else if (a1->x < a2->x && a1->y > a2->y)
                    {
                        inserirNefasto(&listaNefastos, a1, a2, a1->x - dx, a1->y + dy, a2->x + dx, a2->y - dy);
                    }
                    // Top-right to bottom-left
                    else if (a1->x > a2->x && a1->y < a2->y)
                    {
                        inserirNefasto(&listaNefastos, a1, a2, a1->x + dx, a1->y - dy, a2->x - dx, a2->y + dy);
                    }
                    // Bottom-right to top-left
                    else if (a1->x > a2->x && a1->y > a2->y)
                    {
                        inserirNefasto(&listaNefastos, a1, a2, a1->x + dx, a1->y + dy, a2->x - dx, a2->y - dy);
                    }
                    break;
                default:
                    break;
                }
            }
            a2 = a2->next; // Move to the next antenna in the inner loop
        }
        a1 = a1->next; // Move to the next antenna in the outer loop
    }

    return listaNefastos;
}

/**
 * @brief Verifica e recalcula os efeitos nefastos com base na lista de antenas.
 * 
 * @param listaAntenas Ponteiro para a lista de antenas.
 * @param listaNefastos Ponteiro para o ponteiro da lista de efeitos nefastos.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @return Nefasto* Ponteiro para a lista de efeitos nefastos recalculados.
 */
Nefasto* verificaEfeitoNefasto(Antena* listaAntenas, Nefasto** listaNefastos, int linha, int coluna)
{
    // Free the existing list of "Nefasto" effects
    if (*listaNefastos)
    {
        liberarEfeitosNefastos(listaNefastos);
    }

    // Recalculate the "Nefasto" effects based on the list of antennas
    *listaNefastos = calcularEfeitoNefasto(listaAntenas, linha, coluna);

    return *listaNefastos;
}

/**
 * @brief Lista todos os efeitos nefastos registrados.
 * 
 * @param listaNefastos Ponteiro para a lista de efeitos nefastos.
 */
void listarEfeitosNefastos(Nefasto* listaNefastos)
{
    if (!listaNefastos)
    {
        printf("Não existem efeitos nefastos registrados.\n");
        return;
    }

    Nefasto* atual = listaNefastos;
    while (atual)
    {
        printf("Efeito Nefasto entre '%c' em (%d, %d) e (%d, %d)\n", atual->antena1->frequencia ,atual->x1, atual->y1, atual->x2, atual->y2);
        atual = atual->next;
    }
}