#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

typedef struct Antena {
    char frequencia; // Frequência da antena
    int x, y; // Coordenadas da antena
    struct Antena* next; // Ponteiro para a próxima antena
} Antena;

typedef struct Nefasto {
    int x1, y1; // Coordenadas do primeiro ponto do efeito nefasto
    int x2, y2; // Coordenadas do segundo ponto do efeito nefasto
    struct Antena* antena1; // Ponteiro para a primeira antena
    struct Antena* antena2; // Ponteiro para a segunda antena
    struct Nefasto* next; // Ponteiro para o próximo efeito nefasto
} Nefasto;

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

#endif // ESTRUTURAS_H