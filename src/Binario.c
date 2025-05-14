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
bool salvarEmBinario(Antena* lista, int largura, int altura, const char* nomeArquivo)
{
    // Abre o arquivo binário para escrita
    FILE* f = fopen(nomeArquivo, "wb");
    if (!f)
    {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", nomeArquivo);
        return false;
    }

    // Salva o número de linhas e colunas da matriz
    fwrite(&altura, sizeof(int), 1, f);
    fwrite(&largura, sizeof(int), 1, f);

    // Salva as antenas na lista
    Antena* atualAntena = lista;
    while (atualAntena)
    {
        fwrite(&atualAntena->frequencia, sizeof(char), 1, f); // Frequência da antena
        fwrite(&atualAntena->x, sizeof(int), 1, f);          // Coordenada X
        fwrite(&atualAntena->y, sizeof(int), 1, f);          // Coordenada Y
        atualAntena = atualAntena->next;
    }

    // Escreve um marcador de fim de lista de antenas
    char endMarker = '\0';
    fwrite(&endMarker, sizeof(char), 1, f);

    // Fecha o arquivo
    fclose(f);
    return true;
}

/**
 * @brief Lê os dados de antenas e efeitos nefastos de um arquivo binário.
 * 
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param largura Ponteiro para armazenar o número de colunas da matriz.
 * @param altura Ponteiro para armazenar o número de linhas da matriz.
 * @param nomeArquivo Nome do arquivo binário de onde os dados serão lidos.
 * @return Nefasto* Retorna o ponteiro para a lista de efeitos nefastos atualizada.
 */
Antena* lerDeBinario(int* largura, int* altura, const char* nomeArquivo)
{   
    Antena* lista = NULL;
    // Abre o arquivo binário para leitura
    FILE* f = fopen(nomeArquivo, "rb");
    if (!f)
    {
        printf("Erro ao abrir o arquivo '%s' para leitura.\n", nomeArquivo);
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
        if (fread(&freq, sizeof(char), 1, f) != 1 || freq == '\0')
            break;

        // Lê as coordenadas X e Y da antena
        fread(&x, sizeof(int), 1, f);
        fread(&y, sizeof(int), 1, f);

        // Insere a antena na lista
        lista = inserirAntena(lista, freq, x, y);
    }


    // Fecha o arquivo
    fclose(f);
    return lista;
}