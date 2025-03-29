/**
 * @file Binario.c
 * @author Leonel Freitas (a31520@alunos.ipca.pt)
 * @brief Implementação das funções para manipulação de arquivos binários.
 * @version 1.1
 * @date 29-03-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

/**
 * @brief Salva os dados de antenas e efeitos nefastos em um arquivo binário.
 * 
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param largura Número de colunas da matriz.
 * @param altura Número de linhas da matriz.
 * @param nomeArquivo Nome do arquivo binário onde os dados serão salvos.
 */
void salvarEmBinario(Antena *lista, Nefasto *efeitos, int largura, int altura, const char *nomeArquivo)
{
    // Abre o arquivo binário para escrita
    FILE *f = fopen(nomeArquivo, "wb");
    if (!f)
    {
        printf("Erro ao abrir o ficheiro '%s'\n", nomeArquivo);
        return;
    }

    // Salva o número de linhas e colunas da matriz
    fwrite(&altura, sizeof(int), 1, f);
    fwrite(&largura, sizeof(int), 1, f);

    // Salva as antenas na lista
    Antena *currentAntena = lista;
    while (currentAntena)
    {
        fwrite(&currentAntena->frequencia, sizeof(char), 1, f); // Frequência da antena
        fwrite(&currentAntena->x, sizeof(int), 1, f);          // Coordenada X
        fwrite(&currentAntena->y, sizeof(int), 1, f);          // Coordenada Y
        currentAntena = currentAntena->next;
    }

    // Escreve um marcador de fim de lista de antenas
    char endMarker = '\0';
    fwrite(&endMarker, sizeof(char), 1, f);

    // Fecha o arquivo
    fclose(f);
}

/**
 * @brief Lê os dados de antenas e efeitos nefastos de um arquivo binário.
 * 
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param largura Ponteiro para armazenar o número de colunas da matriz.
 * @param altura Ponteiro para armazenar o número de linhas da matriz.
 * @param nomeArquivo Nome do arquivo binário de onde os dados serão lidos.
 */
void lerDeBinario(Antena **lista, Nefasto **efeitos, int *largura, int *altura, const char *nomeArquivo)
{
    // Abre o arquivo binário para leitura
    FILE *f = fopen(nomeArquivo, "rb");
    if (!f)
    {
        printf("Erro ao abrir o ficheiro '%s'\n", nomeArquivo);
        return;
    }

    // Libera a memória da lista de antenas existente, se necessário
    if (*lista)
    {
        liberarAntenas(*lista);
        *lista = NULL;
    }

    // Libera a memória da lista de efeitos nefastos existente, se necessário
    if (*efeitos)
    {
        liberarEfeitosNefastos(efeitos);
    }

    // Lê o número de linhas e colunas da matriz
    fread(altura, sizeof(int), 1, f);
    fread(largura, sizeof(int), 1, f);

    // Lê as antenas do arquivo
    while (1)
    {
        char freq;
        int x, y;

        // Lê a frequência da antena
        if (fread(&freq, sizeof(char), 1, f) != 1)
            break;

        // Verifica o marcador de fim de lista de antenas
        if (freq == '\0')
            break;

        // Lê as coordenadas X e Y da antena
        if (fread(&x, sizeof(int), 1, f) != 1)
            break;
        if (fread(&y, sizeof(int), 1, f) != 1)
            break;

        // Insere a antena na lista
        inserirAntena(lista, freq, x, y);
    }

    // Fecha o arquivo
    fclose(f);
}