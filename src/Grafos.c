#include "../resources/Grafos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Vertice* obterOuCriarVertice(Grafo* grafo, Antena* antena) {
    if (!grafo || !antena) {
        printf("Erro: Grafo ou antena é nulo.\n");
        return NULL;
    }

    Vertice* atual = grafo->vertices;
    while (atual) {
        if (atual->antena == antena)
            return atual;
        atual = atual->next;
    }

    // Cria um novo vértice se não encontrado
    Vertice* novo = malloc(sizeof(Vertice));
    if (!novo) {
        printf("Erro: Falha ao alocar memória para o vértice.\n");
        return NULL;
    }

    novo->antena = antena;
    novo->adjacentes = NULL;
    novo->next = grafo->vertices;
    grafo->vertices = novo;

    return novo;
}

int adicionarAresta(Grafo* grafo, Antena* a, Antena* b) {
    if (!grafo || !a || !b) return 1;

    Vertice* va = obterOuCriarVertice(grafo, a);
    Vertice* vb = obterOuCriarVertice(grafo, b);
    if (!va || !vb) return 1;

    Aresta* ab = malloc(sizeof(Aresta));
    if (!ab) return 1;
    ab->destino = b;
    ab->next = va->adjacentes;
    va->adjacentes = ab;

    Aresta* ba = malloc(sizeof(Aresta));
    if (!ba) return 1;
    ba->destino = a;
    ba->next = vb->adjacentes;
    vb->adjacentes = ba;

    return 0;
}

Grafo* construirGrafo(Antena* lista) {
    Grafo* grafo = malloc(sizeof(Grafo));
    if (!grafo) return NULL;
    grafo->vertices = NULL;

    for (Antena* a = lista; a != NULL; a = a->next) {
        for (Antena* b = a->next; b != NULL; b = b->next) {
            if (a != b && a->frequencia == b->frequencia) {
                if (a->x == b->x || a->y == b->y) {
                    if (adicionarAresta(grafo, a, b) != 0) {
                        printf("Erro ao adicionar aresta entre (%d,%d) e (%d,%d)\n", a->x, a->y, b->x, b->y);
                    }
                }
            }
        }
    }

    return grafo;
}

static int mesmaAntena(Antena* a, Antena* b) {
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
int dfs_visitado(Vertice* vertice, Antena* alvo, Antena* visitados, int* visitadosCount, int max) {
    if (!vertice || !vertice->antena || *visitadosCount >= max) return 0;

    // Verifica se a antena já foi visitada
    for (int i = 0; i < *visitadosCount; i++) {
        if (mesmaAntena(&visitados[i], vertice->antena)) return 0;
    }

    // Marca a antena como visitada
    visitados[(*visitadosCount)++] = *(vertice->antena);

    // Verifica se o alvo foi encontrado
    if (alvo && mesmaAntena(vertice->antena, alvo)) {
        printf("Encontrado: (%d,%d)\n", alvo->x, alvo->y);
        return 1;
    }

    // Explora os vértices adjacentes
    Aresta* adj = vertice->adjacentes;
    while (adj) {
        if (dfs_visitado(obterOuCriarVertice(NULL, adj->destino), alvo, visitados, visitadosCount, max)) {
            return 1;
        }
        adj = adj->next;
    }

    return 0;
}


int dfs(Grafo* grafo, Antena* origem) {
    if (!grafo || !origem) {
        printf("Erro: Grafo ou antena de origem é nulo.\n");
        return 0;
    }

    // Verifica se o grafo possui vértices
    if (!grafo->vertices) {
        printf("Erro: O grafo está vazio.\n");
        return 0;
    }

    Antena visitados[100]; // Array para armazenar as antenas visitadas
    int count = 0;

    // Obtém o vértice inicial correspondente à antena de origem
    Vertice* start = obterOuCriarVertice(grafo, origem);
    if (!start) {
        printf("Erro: Não foi possível encontrar ou criar o vértice inicial.\n");
        return 0;
    }

    // Marca o vértice inicial como visitado
    visitados[count++] = *(start->antena);
    printf("[DFS] Visitando antena: %c (%d, %d)\n", start->antena->frequencia, start->antena->x, start->antena->y);

    // Pilha para a DFS
    Vertice* stack[100];
    int top = -1;

    stack[++top] = start;

    while (top >= 0) {
        Vertice* atual = stack[top--];

        Aresta* adj = atual->adjacentes;
        while (adj) {
            Vertice* vizinho = obterOuCriarVertice(grafo, adj->destino);

            // Verifica se o vizinho já foi visitado
            int jaVisitado = 0;
            for (int i = 0; i < count; i++) {
                if (visitados[i].x == vizinho->antena->x && visitados[i].y == vizinho->antena->y) {
                    jaVisitado = 1;
                    break;
                }
            }

            if (!jaVisitado) {
                visitados[count++] = *(vizinho->antena);
                printf("[DFS] Visitando antena: %c (%d, %d)\n", vizinho->antena->frequencia, vizinho->antena->x, vizinho->antena->y);
                stack[++top] = vizinho;
            }

            adj = adj->next;
        }
    }

    return count; // Retorna o número de antenas visitadas
}

int bfs(Grafo* grafo, Antena* origem) {
    printf("[BFS] (placeholder)\n");
    return 0;
}

void mostrarCaminhosRec(Grafo* grafo, Vertice* atual, int* visitado) {
    if (!grafo || !atual || visitado[atual->antena->x * 100 + atual->antena->y]) return;

    visitado[atual->antena->x * 100 + atual->antena->y] = 1;
    printf("(%d, %d)\n", atual->antena->x, atual->antena->y);

    Aresta* adj = atual->adjacentes;
    while (adj) {
        Vertice* destino = obterOuCriarVertice(grafo, adj->destino);
        if (destino && !visitado[destino->antena->x * 100 + destino->antena->y]) {
            mostrarCaminhosRec(grafo, destino, visitado);
        }
        adj = adj->next;
    }
}

void mostrarCaminhos(Grafo* g, Antena* origem) {
    if (!g || !origem) {
        printf("Erro: Grafo ou antena de origem é nulo.\n");
        return;
    }

    int visitado[10000] = {0}; // Exemplo: assumindo coordenadas entre 0 e 99
    Vertice* v = obterOuCriarVertice(g, origem);
    if (!v) {
        printf("Erro: Não foi possível encontrar ou criar o vértice inicial.\n");
        return;
    }

    mostrarCaminhosRec(g, v, visitado);
}