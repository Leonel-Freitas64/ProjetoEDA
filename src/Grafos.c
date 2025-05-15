#include "Estruturas.h"
#include "Funções.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Vertice *obterOuCriarVertice(Grafo *grafo, Antena *antena)
{
    if (!antena)
    {
        printf("Antena é nulo.\n");
        return NULL;
    }

    Vertice *atual = grafo->vertices;
    while (atual)
    {
        if (atual->antena == antena)
            return atual;
        atual = atual->next;
    }

    // Cria um novo vértice se não encontrado
    Vertice *novo = malloc(sizeof(Vertice));
    if (!novo)
    {
        printf("Erro: Falha ao alocar memória para o vértice.\n");
        return NULL;
    }

    novo->antena = antena;
    novo->adjacentes = NULL;
    novo->next = grafo->vertices;
    grafo->vertices = novo;

    return novo;
}

Vertice* encontrarVerticePorAntena(Vertice* atual, Antena* antena) {
    while (atual) {
        if (atual->antena == antena) {
            return atual;
        }
        atual = atual->next;
    }
    return NULL;
}

int adicionarAresta(Grafo *grafo, Antena *a, Antena *b)
{
    if (!grafo || !a || !b)
        return 1;

    Vertice *va = obterOuCriarVertice(grafo, a);
    Vertice *vb = obterOuCriarVertice(grafo, b);
    if (!va || !vb)
        return 1;

    Aresta *ab = malloc(sizeof(Aresta));
    if (!ab)
        return 1;
    ab->destino = b;
    ab->next = va->adjacentes;
    va->adjacentes = ab;

    Aresta *ba = malloc(sizeof(Aresta));
    if (!ba)
        return 1;
    ba->destino = a;
    ba->next = vb->adjacentes;
    vb->adjacentes = ba;

    return 0;
}

Grafo *construirGrafo(Antena *lista)
{
    Grafo *grafo = malloc(sizeof(Grafo));
    if (!grafo)
        return NULL;
    grafo->vertices = NULL;

    for (Antena *a = lista; a != NULL; a = a->next)
    {
        for (Antena *b = a->next; b != NULL; b = b->next)
        {
            if (a != b && a->frequencia == b->frequencia)
            {
                int dx = abs(a->x - b->x);
                int dy = abs(a->y - b->y);
                if(dx > 3 || dy > 3)
                    continue;
                if (a->x == b->x && dy <= 3)
                {
                    if (adicionarAresta(grafo, a, b) != 0)
                    {
                        printf("Erro ao adicionar aresta entre (%d,%d) e (%d,%d)\n", a->x, a->y, b->x, b->y);
                    }
                }
                else if (a->y == b->y && dx <= 3)
                {
                    if (adicionarAresta(grafo, a, b) != 0)
                    {
                        printf("Erro ao adicionar aresta entre (%d,%d) e (%d,%d)\n", a->x, a->y, b->x, b->y);
                    }
                }
                else if ((dx + dy <= 4))
                {
                    if (adicionarAresta(grafo, a, b) != 0)
                    {
                        printf("Erro ao adicionar aresta entre (%d,%d) e (%d,%d)\n", a->x, a->y, b->x, b->y);
                    }
                }
            }
        }
    }

    return grafo;
}

void imprimirGrafo(Grafo* grafo, Antena* lista, int linha, int coluna) {
    if (!grafo || !lista) {
        printf("Erro: Grafo ou lista de antenas é nulo.\n");
        return;
    }
    printf("Grafo:\n");

    // Itera sobre os vértices do grafo
    Vertice* v = grafo->vertices;
    while (v) {
        printf("Antena de frequencia %c: (%d, %d) --- ",v->antena->frequencia, v->antena->x, v->antena->y);

        // Itera sobre as arestas (adjacências) do vértice
        Aresta* adj = v->adjacentes;
        while (adj) {
            printf("Antena (%d, %d)", adj->destino->x, adj->destino->y);
            adj = adj->next;
            if (adj) {
                printf(", --- "); // Adiciona vírgula entre conexões
            }
        }

        printf("\n"); // Nova linha para o próximo vértice
        v = v->next;
    }
    
}

static int mesmaAntena(Antena *a, Antena *b)
{
    return a->x == b->x && a->y == b->y && a->frequencia == b->frequencia;
}

/**
 * @brief Realiza uma busca em profundidade (DFS) no grafo.
 *
 * @param vertice Ponteiro para o vértice atual.
 * @param alvo Ponteiro para a antena alvo.
 * @param visitados Array de antenas já visitadas.
 * @param visitadosCount Número de antenas já visitadas.
 * @param max Tamanho máximo do array de visitados.
 * @return int Retorna 1 se o alvo foi encontrado, 0 caso contrário.
 */
int dfs_visitado(Vertice *vertice, Antena *alvo, Antena *visitados, int *visitadosCount, int max)
{
    if (!vertice || !vertice->antena || *visitadosCount >= max)
        return 0;

    // Verifica se a antena já foi visitada
    for (int i = 0; i < *visitadosCount; i++)
    {
        if (mesmaAntena(&visitados[i], vertice->antena))
            return 0;
    }

    // Marca a antena como visitada
    visitados[(*visitadosCount)++] = *(vertice->antena);

    // Verifica se o alvo foi encontrado
    if (alvo && mesmaAntena(vertice->antena, alvo))
    {
        printf("Encontrado: (%d,%d)\n", alvo->x, alvo->y);
        return 1;
    }

    // Explora os vértices adjacentes
    Aresta *adj = vertice->adjacentes;
    while (adj)
    {
        if (dfs_visitado(obterOuCriarVertice(NULL, adj->destino), alvo, visitados, visitadosCount, max))
        {
            return 1;
        }
        adj = adj->next;
    }

    return 0;
}

int dfs(Grafo *grafo, Antena *origem)
{
    if (!grafo || !origem)
    {
        printf("Erro: Grafo ou antena de origem é nulo.\n");
        return 0;
    }

    Vertice *inicio = obterOuCriarVertice(grafo, origem);
    if (!inicio)
    {
        printf("Erro: Vértice de origem não encontrado.\n");
        return 0;
    }

    int visitado[10000] = {0};  // assume max 100x100
    Vertice *stack[100];
    int topo = -1;
    stack[++topo] = inicio;

    while (topo >= 0)
    {
        Vertice *v = stack[topo--];
        int idx = v->antena->x * 100 + v->antena->y;

        if (visitado[idx])
            continue;

        visitado[idx] = 1;
        printf("[DFS] Visitado: %c (%d, %d)\n", v->antena->frequencia, v->antena->x, v->antena->y);

        // Adiciona vizinhos à stack
        Aresta *adj = v->adjacentes;
        while (adj)
        {
            Vertice *vizinho = obterOuCriarVertice(grafo, adj->destino);
            int id_vizinho = vizinho->antena->x * 100 + vizinho->antena->y;
            if (!visitado[id_vizinho])
            {
                stack[++topo] = vizinho;
            }
            adj = adj->next;
        }
    }

    return 1;
}


int bfs(Grafo *grafo, Antena *origem)
{
    if (!grafo || !origem)
    {
        printf("Erro: Grafo ou antena de origem é nulo.\n");
        return 0;
    }

    Vertice *inicio = obterOuCriarVertice(grafo, origem);
    if (!inicio)
    {
        printf("Erro: Vértice de origem não encontrado.\n");
        return 0;
    }

    int visitado[10000] = {0}; // assume matriz até 100x100
    Vertice *fila[100];        // fila para BFS
    int frente = 0, tras = 0;

    fila[tras++] = inicio;

    while (frente < tras)
    {
        Vertice *atual = fila[frente++];
        int idx = atual->antena->x * 100 + atual->antena->y;

        if (visitado[idx])
            continue;

        visitado[idx] = 1;
        printf("[BFS] Visitado: %c (%d, %d)\n", atual->antena->frequencia, atual->antena->x, atual->antena->y);

        Aresta *adj = atual->adjacentes;
        while (adj)
        {
            Vertice *vizinho = obterOuCriarVertice(grafo, adj->destino);
            int id_vizinho = vizinho->antena->x * 100 + vizinho->antena->y;

            if (!visitado[id_vizinho])
            {
                fila[tras++] = vizinho;
            }
            adj = adj->next;
        }
    }

    return 1;
}

void dfsCaminhosRec(Vertice* atual, Antena* destino, int* visitado, int* caminhoX, int* caminhoY, int profundidade) {
    if (!atual || !destino) {
        return;
    }

    int index = atual->antena->x * 100 + atual->antena->y;
    visitado[index] = 1; // Marca o vértice como visitado

    // Adiciona a antena atual ao caminho
    caminhoX[profundidade] = atual->antena->x;
    caminhoY[profundidade] = atual->antena->y;

    // Verifica se o destino foi alcançado
    if (atual->antena->x == destino->x && atual->antena->y == destino->y) {
        printf("Caminho encontrado: ");
        for (int i = 0; i <= profundidade; i++) {
            printf("(%d, %d) ", caminhoX[i], caminhoY[i]);
        }
        printf("\n");
        visitado[index] = 0; // Desmarca o vértice antes de retornar
        return;
    }

    // Explora os vértices adjacentes
    Aresta* adj = atual->adjacentes;
    while (adj) {
        Antena* destinoAntena = adj->destino;
        int destionIndex = destinoAntena->x * 100 + destinoAntena->y;
        if (!visitado[destionIndex]) {
            Vertice* proximo = encontrarVerticePorAntena(atual, destinoAntena);
            dfsCaminhosRec(proximo, destino, visitado, caminhoX, caminhoY, profundidade + 1);
        }
        adj = adj->next;
    }

    visitado[index] = 0; // Desmarca o vértice
}

void mostrarCaminhos(Grafo* grafo, Antena* origem, Antena* destino) {
    if (!grafo || !origem || !destino) {
        printf("Erro: Grafo ou antena de origem/destino é nulo.\n");
        return;
    }

    Vertice* verticeOrigem = obterOuCriarVertice(grafo, origem);
    if (!verticeOrigem) {
        printf("Erro: Não foi possível encontrar ou criar o vértice de origem.\n");
        return;
    }

    int visitado[10000] = {0}; // Array para marcar os vértices visitados
    int caminhoX[100], caminhoY[100]; // Arrays para armazenar o caminho

    printf("Caminhos de (%d, %d) para (%d, %d):\n", origem->x, origem->y, destino->x, destino->y);
    dfsCaminhosRec(verticeOrigem, destino, visitado, caminhoX, caminhoY, 0);
}

Grafo* libertarGrafo(Grafo *grafo)
{
    if (!grafo) return NULL;

    Vertice *vAtual = grafo->vertices;
    while (vAtual)
    {
        // Libertar todas as arestas do vértice atual
        Aresta *aAtual = vAtual->adjacentes;
        while (aAtual)
        {
            Aresta *aRemover = aAtual;
            aAtual = aAtual->next;
            free(aRemover);
        }

        // Libertar o vértice
        Vertice *vRemover = vAtual;
        vAtual = vAtual->next;
        free(vRemover);
    }

    // Libertar o grafo
    free(grafo);
    return NULL;
}