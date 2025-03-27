#ifndef FUNCOES_H
#define FUNCOES_H

#include "Estruturas.h"
#include <stdbool.h>

// Geral
void mostrarmenu();
void processarOpcao(int opcao, Antena** lista, Nefasto** efeitos, int* linha, int* coluna);

// Antenas
bool inserirAntena(Antena** head, char freq, int x, int y);
Antena* lerAntenasDoFicheiro(const char* nomeFicheiro, int* linha, int* coluna);
void listarAntenas(Antena* lista);
void imprimirMatrizAntenas(Antena* lista, Nefasto* efeitos, int colunas, int linhas, int modo);
bool eliminarAntena(Antena** head, int x, int y);
//void recolherDadosAntena(char* freq, int* x, int* y, int linha, int coluna);

// Arquivos
void salvarEmBinario(Antena *lista, Nefasto *efeitos, int largura, int altura, const char *nomeArquivo);
void lerDeBinario(Antena **lista, Nefasto **efeitos, int *largura, int *altura, const char *nomeArquivo);

// Efeitos Nefastos
Nefasto* calcularEfeitoNefasto(Antena* lista, int linha, int coluna);
Nefasto* criarEfeitoNefasto(Antena* antena1, Antena* antena2 ,int x1, int y1, int x2, int y2);
bool inserirNefasto(Nefasto **head, Antena *antena, Antena *antena2, int x1, int y1, int x2, int y2);

// Função para recolher dados da antena
//void recolherDadosAntena(char* freq, int* x, int* y, int linha, int coluna);

#endif // FUNCOES_H