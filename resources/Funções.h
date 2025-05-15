#ifndef FUNCOES_H
#define FUNCOES_H

#include "Estruturas.h"

// Menus
void mostrarMenuPrincipal();
void mostrarMenuSecundario();

// Processamento de opções
Antena* processarOpcaoSecundaria(int opcao, Antena* lista, Nefasto* efeitos, int linha, int coluna);
void processarOpcao(int opcao, Antena* lista, Nefasto* efeitos, int* linha, int* coluna);
int obterOpcaoMenu();
int obterOpcaoMenuSecundario();
int executarMenuSecundario(Antena* lista, Nefasto* efeitos, int* linha, int* coluna);
Grafo* executarMenuGrafo(Grafo* grafo, Antena* lista, int linha, int coluna);
int mostrarMenuGrafo();

// Manipulação de antenas
Antena* inserirAntena(Antena* lista, char freq, int x, int y);
Antena* eliminarAntena(Antena* lista, int x, int y, Nefasto* listaNefastos);
Antena *lerAntenasDoFicheiro(const char *nomeFicheiro, int *linha, int *coluna);
bool listarAntenas(Antena* lista);
Antena* liberarAntenas(Antena *lista);
Antena* encontrarAntenaPorCoordenadas(Antena* lista, int x, int y);
Antena* selecionarAntena(Antena* lista, int linha, int coluna);

// Manipulação de efeitos nefastos
Nefasto* calcularEfeitoNefasto(Antena* lista, int linha, int coluna);
Nefasto* verificaEfeitoNefasto(Antena* lista, Nefasto* efeitos, int linha, int coluna);
void listarEfeitosNefastos(Nefasto* listaNefastos);
Nefasto* liberarEfeitosNefastos(Nefasto* lista);
Nefasto* removerEfeitosPorAntena(Nefasto* lista, Antena* antena);

// Manipulação de arquivos
bool salvarEmBinario(Antena* lista, int largura, int altura, const char* nomeArquivo);
Antena* lerDeBinario(int *largura, int *altura, const char *nomeArquivo);

// Outros
bool recolherDadosAntena(char *freq, int *x, int *y, int linha, int coluna);
bool recolherCoordenadasAntena(int *x, int *y, int linha, int coluna);
void imprimirMatrizAntenas(Antena* lista, Nefasto* efeitos, int colunas, int linhas, int modo);
Antena* redimensionarMatriz(Antena** lista, Nefasto** efeitos, int* linhas, int* colunas, int novasLinhas, int novasColunas);


Grafo* construirGrafo(Antena* lista);
int adicionarAresta(Grafo* grafo, Antena* a, Antena* b);
int dfs(Grafo* grafo, Antena* origem);
int bfs(Grafo* grafo, Antena* origem);
void mostrarCaminhos(Grafo* grafos, Antena* origem, Antena* destino);
static Vertice* obterOuCriarVertice(Grafo* grafo, Antena* antena);
int dfs_visitado(Vertice* vertice, Antena* alvo, Antena* visitados, int* visitadosCount, int max);
void dfsCaminhosRec(Vertice *atual,Antena* destino, int *visitado, int *caminhoX, int *caminhoY, int profundidade);
void imprimirGrafo(Grafo* grafo, Antena* lista, int linha, int coluna);
Grafo* libertarGrafo(Grafo* grafo);

#endif