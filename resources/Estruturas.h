#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#define MAX_IDIOMA 3 // Tamanho máximo do código do idioma (ex.: "PT", "EN", "ZH")
#define MAX_TEXTO 200

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

typedef struct Mensagem {
    int codigo;
    char texto[MAX_TEXTO];
    struct Mensagem* next;
} Mensagem;

typedef struct ListaMensagem {
    char idiomaStr[MAX_IDIOMA]; // Código do idioma como string (ex.: "PT", "EN", "ZH")
    int idioma;        // Índice do idioma (ex.: 1, 2, 3)
    Mensagem* mensagens;
    struct ListaMensagem* next;
} ListaMensagem;
#endif // ESTRUTURAS_H