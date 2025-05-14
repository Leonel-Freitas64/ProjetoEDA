#ifndef FUNCOES_H
#define FUNCOES_H

#include "Estruturas.h"

// Menus
void mostrarMenuPrincipal();
void mostrarMenuSecundario();

// Processamento de opções
void processarOpcaoSecundaria(int opcao, Antena* lista, Nefasto* efeitos, int linha, int coluna, bool primeiraVez);
void processarOpcao(int opcao, Antena* lista, Nefasto* efeitos, int* linha, int* coluna, bool* primeiraVez);


// Manipulação de antenas
Antena* inserirAntena(Antena* lista, char freq, int x, int y);
Antena* eliminarAntena(Antena* lista, int x, int y);
Antena *lerAntenasDoFicheiro(const char *nomeFicheiro, int *linha, int *coluna);
bool listarAntenas(Antena* lista);
bool liberarAntenas(Antena *lista);
Antena* encontrarAntenaPorCoordenadas(Antena* lista, int x, int y);
Antena* selecionarAntena(Antena* lista, int linha, int coluna);

// Manipulação de efeitos nefastos
Nefasto* calcularEfeitoNefasto(Antena* lista, int linha, int coluna);
Nefasto* verificaEfeitoNefasto(Antena* lista, Nefasto* efeitos, int linha, int coluna);
void listarEfeitosNefastos(Nefasto* listaNefastos);
void liberarEfeitosNefastos(Nefasto* lista);

// Manipulação de arquivos
bool salvarEmBinario(Antena* lista, int largura, int altura, const char* nomeArquivo);
Antena* lerDeBinario(int *largura, int *altura, const char *nomeArquivo);

// Outros
bool recolherDadosAntena(char *freq, int *x, int *y, int linha, int coluna);
bool recolherCoordenadasAntena(int *x, int *y, int linha, int coluna);
void imprimirMatrizAntenas(Antena* lista, Nefasto* efeitos, int colunas, int linhas, int modo);
Antena* redimensionarMatriz(Antena** lista, Nefasto** efeitos, int* linhas, int* colunas, int novasLinhas, int novasColunas);

#endif