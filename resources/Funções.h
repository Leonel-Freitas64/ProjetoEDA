#ifndef FUNCOES_H
#define FUNCOES_H

#include "Estruturas.h"

// Menus
int mostrarMenuPrincipal(ListaMensagem *listaMensagens, int idioma);
int mostrarMenuSecundario(ListaMensagem *listaMensagens, int idioma);
void limparBufferEntrada();

// Processamento de opções
Antena* processarOpcaoSecundaria(int opcao, Antena* lista, Nefasto* efeitos, int linha, int coluna, ListaMensagem* listaMensagens, int idioma);
void processarOpcao(int opcao, Antena *lista, Nefasto *efeitos, int *linha, int *coluna, ListaMensagem *listaMensagens, int idioma);
int executarMenuSecundario(Antena* lista, Nefasto* efeitos, int* linha, int* coluna, ListaMensagem *listaMensagens, int idioma);
Grafo* executarMenuGrafo(Grafo* grafo, Antena* lista, int linha, int coluna, ListaMensagem* listaMensagens, int idioma);


// Manipulação de antenas
Antena* inserirAntena(Antena* lista, char freq, int x, int y);
Antena* eliminarAntena(Antena* lista, int x, int y, Nefasto* listaNefastos, ListaMensagem* listaMensagens, int idioma);
Antena *lerAntenasDoFicheiro(const char *nomeFicheiro, int *linha, int *coluna);
bool listarAntenas(Antena* lista, ListaMensagem* listaMensagens, int idioma);
Antena* liberarAntenas(Antena *lista);
Antena* encontrarAntenaPorCoordenadas(Antena* lista, int x, int y);
Antena *selecionarAntena(Antena *lista, int linha, int coluna, ListaMensagem *listaMensagens, int idioma);

// Manipulação de efeitos nefastos
Nefasto* calcularEfeitoNefasto(Antena* lista, int linha, int coluna);
Nefasto* verificaEfeitoNefasto(Antena* lista, Nefasto* efeitos, int linha, int coluna);
void listarEfeitosNefastos(Nefasto* listaNefastos, ListaMensagem* listaMensagens, int idioma);
Nefasto* liberarEfeitosNefastos(Nefasto* lista);
Nefasto* removerEfeitosPorAntena(Nefasto* lista, Antena* antena);

// Manipulação de arquivos
bool salvarEmBinario(Antena* lista, int largura, int altura, const char* nomeArquivo);
Antena* lerDeBinario(int *largura, int *altura, const char *nomeArquivo);

//Mensagens
ListaMensagem* carregarMensagens(const char* caminho);
void mostrarMensagem(ListaMensagem* lista, int idioma, int codigo);
void liberarListaMensagem(ListaMensagem* lista);
ListaMensagem* obterIdiomaPorIndice(ListaMensagem* lista, int idioma);
ListaMensagem* criarIdioma(ListaMensagem* lista, const char* idiomaStr, int idioma);
ListaMensagem* procurarIdioma(ListaMensagem* lista, const char* idiomaStr);
int selecionarIdioma(ListaMensagem* lista);
int mostrarSobreposicao(ListaMensagem *listaMensagens, int idioma, int x, int y, char caractere);
Mensagem* obterMensagem(ListaMensagem *lista, int idioma, int codigo);

// Outros
bool recolherDadosAntena(ListaMensagem *listaMensagens, int idioma, char *freq, int *x, int *y, int linha, int coluna);
bool recolherCoordenadasAntena(int *x, int *y, int linha, int coluna);
void imprimirMatrizAntenas(Antena* lista, Nefasto* efeitos, int colunas, int linhas, int modo, ListaMensagem* listaMensagem, int idioma);
Antena* redimensionarMatriz(Antena** lista, Nefasto** efeitos, int* linhas, int* colunas, int novasLinhas, int novasColunas);


Grafo* construirGrafo(Antena *lista, ListaMensagem *listamensagens,int idioma);
int adicionarAresta(Grafo* grafo, Antena* a, Antena* b);
int dfs(Grafo* grafo, Antena* origem, ListaMensagem* listaMensagens, int idioma);
int bfs(Grafo* grafo, Antena* origem, ListaMensagem* listaMensagens, int idioma);
bool mostrarCaminhos(Grafo* grafos, Antena* origem, Antena* destino);
static Vertice* obterOuCriarVertice(Grafo* grafo, Antena* antena);
int dfs_visitado(Vertice* vertice, Antena* alvo, Antena* visitados, int* visitadosCount, int max);
void dfsCaminhosRec(Vertice *atual,Antena* destino, int *visitado, int *caminhoX, int *caminhoY, int profundidade);
void imprimirGrafo(Grafo* grafo, Antena* lista, int linha, int coluna, ListaMensagem* listaMensagens, int idioma);
Grafo* libertarGrafo(Grafo* grafo);
void detetarIntersecoes(Grafo* grafo, ListaMensagem* listamensagens, int idioma);

#endif