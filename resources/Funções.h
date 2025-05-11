#ifndef FUNCOES_H
#define FUNCOES_H

#include "Estruturas.h"
#include "Mensagem.h"

// Menus
void mostrarMenuPrincipal();
void mostrarMenuSecundario();

// Processamento de opções
void processarOpcaoSecundaria(int opcao, Antena** lista, Nefasto** efeitos, int linha, int coluna, bool* primeiraVez);
void processarOpcao(int opcao, Antena** lista, Nefasto** efeitos, int* linha, int* coluna, bool* primeiraVez);

// Manipulação de antenas
bool inserirAntena(Antena** head, char freq, int x, int y);
bool eliminarAntena(Antena** head, int x, int y);
Antena* lerAntenasDoFicheiro(const char* nomeFicheiro,Antena **lista, int* linha, int* coluna);
void listarAntenas(Antena* lista);

// Manipulação de efeitos nefastos
Nefasto* calcularEfeitoNefasto(Antena* lista, int linha, int coluna);
Nefasto* verificaEfeitoNefasto(Antena* lista, Nefasto** efeitos, int linha, int coluna);
void listarEfeitosNefastos(Nefasto* listaNefastos);
void liberarEfeitosNefastos(Nefasto* lista);

// Manipulação de arquivos
void salvarEmBinario(Antena* lista, Nefasto* efeitos, int largura, int altura, const char* nomeArquivo);
void lerDeBinario(Antena** lista, Nefasto** efeitos, int* largura, int* altura, const char* nomeArquivo);

//Mensagens
/* void inicializarMensagemLista(MensagemLista* lista);
MensagemLista* carregarMensagens(const char* caminho);
void mostrarMensagem(MensagemLista* lista, int idioma, int codigo);
void liberarMensagens(MensagemLista* lista); */

// Outros
void recolherDadosAntena(char* freq, int* x, int* y, int linha, int coluna);
void imprimirMatrizAntenas(Antena* lista, Nefasto* efeitos, int colunas, int linhas, int modo);
Antena* redimensionarMatriz(Antena** lista, Nefasto** efeitos, int* linhas, int* colunas, int novasLinhas, int novasColunas);

#endif