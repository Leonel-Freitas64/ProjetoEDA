// Grafo.h
#ifndef GRAFO_H
#define GRAFO_H

#include "Estruturas.h"

typedef struct Aresta {
    Antena* destino;
    struct Aresta* next;
} Aresta;

typedef struct Vertice {
    Antena* antena;
    Aresta* adjacentes;
    struct Vertice* next;
} Vertice;

typedef struct Grafo {
    Vertice* vertices;
} Grafo;

Grafo* construirGrafo(Antena* lista);
int adicionarAresta(Grafo* grafo, Antena* a, Antena* b);
int dfs(Grafo* grafo, Antena* origem);
int bfs(Grafo* grafo, Antena* origem);
void mostrarCaminhos(Grafo* g, Antena* origem);
static Vertice* obterOuCriarVertice(Grafo* grafo, Antena* antena);
int dfs_visitado(Vertice* vertice, Antena* alvo, Antena* visitados, int* visitadosCount, int max);
void bfs_visitado(Vertice* vertice, Antena* alvo, int* visitado);
void mostrarCaminhosRec(Grafo* grafo, Vertice* atual, int* visitado);

#endif // GRAFO_HS