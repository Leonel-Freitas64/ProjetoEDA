#include "../resources/Funções.h"
#include "../resources/Estruturas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

Antena *criarAntena(char freq, int x, int y)
{
    Antena *nova = malloc(sizeof(Antena));
    if (!nova)
        return NULL;
    memset(nova, 0, sizeof(Antena));
    nova->frequencia = freq;
    nova->x = x;
    nova->y = y;
    nova->next = NULL;
    return nova;
}

// Insere no início da lista
bool inserirAntena(Antena **head, char freq, int x, int y) {
    // Verificar se já existe uma antena nas mesmas coordenadas
    Antena *current = *head;
    /* while (current) {
        if (current->x == x && current->y == y) {
            printf("Erro: Já existe uma antena nas coordenadas (%d, %d)\n", x, y);
            return false;
        }
        current = current->next;
    } */

    // Inserir nova antena
    Antena *nova = criarAntena(freq, x, y);
    if (!nova) return false;
    nova->next = *head;
    *head = nova;
    return true;
}

// Lê ficheiro e guarda apenas antenas
Antena *lerAntenasDoFicheiro(const char *nomeFicheiro, int *linha, int *coluna)
{
    FILE *f = fopen(nomeFicheiro, "r");
    if (!f)
    {
        printf("Erro ao abrir o ficheiro '%s'\n", nomeFicheiro);
        return NULL;
    }

    Antena *lista = NULL;
    char buffer[256];
    int y = 0;
    int maxColuna = 0;

    while (fgets(buffer, sizeof(buffer), f))
    {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        int linhaComprimento = strlen(buffer);
        if (linhaComprimento > maxColuna)
        {
            maxColuna = linhaComprimento;
        }
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            char c = buffer[i];
            if (isalpha(c) || isdigit(c))
            {
                if (inserirAntena(&lista, c, i, y) == true)
                {
                    // Inserção bem-sucedida, prossegue
                }
                else
                {
                    // Inserção falhou, trata o erro
                    printf("Erro ao inserir antena na posição (%d, %d)\n", i, y);
                }
            }
        }
        y++;
    }

    *linha = y;
    *coluna = maxColuna;

    fclose(f);
    return lista;
}

// Imprime antenas
void listarAntenas(Antena *lista)
{
    Antena *current = lista;
    while (current)
    {
        printf("Frequencia: %c, Coordenadas: (%d, %d)\n", current->frequencia, current->x, current->y);
        current = current->next;
    }
}

// Função para imprimir a matriz com as antenas e/ou efeitos nefastos
void imprimirMatrizAntenas(Antena *lista, Nefasto *efeitos, int colunas, int linhas, int modo)
{
    // Cria a matriz e inicializa com '.'
    char matriz[linhas][colunas];
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            matriz[i][j] = '.';
        }
    }

    // Preenche a matriz com as antenas
    if (modo == 1 || modo == 3)
    {
        Antena *current = lista;
        while (current)
        {
            if (current->x < colunas && current->y < linhas)
            {
                matriz[current->y][current->x] = current->frequencia;
            }
            current = current->next;
        }
    }

    // Preenche a matriz com os efeitos nefastos
    if (modo == 2 || modo == 3)
    {
        Nefasto *efeitoAtual = efeitos;
        while (efeitoAtual)
        {
            if (efeitoAtual->x1 < colunas && efeitoAtual->y1 < linhas)
            {
                matriz[efeitoAtual->y1][efeitoAtual->x1] = '#';
            }
            if (efeitoAtual->x2 < colunas && efeitoAtual->y2 < linhas)
            {
                matriz[efeitoAtual->y2][efeitoAtual->x2] = '#';
            }
            efeitoAtual = efeitoAtual->next;
        }
    }

    // Imprime a matriz
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            printf("%c ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Função para salvar os dados em um arquivo binário
void salvarEmBinario(Antena *lista, Nefasto *efeitos, int largura, int altura, const char *nomeArquivo)
{
    FILE *f = fopen(nomeArquivo, "wb");
    if (!f)
    {
        printf("Erro ao abrir o ficheiro '%s'\n", nomeArquivo);
        return;
    }

    // Salvar número de linhas e colunas
    fwrite(&altura, sizeof(int), 1, f);
    fwrite(&largura, sizeof(int), 1, f);

    // Salvar antenas
    Antena *currentAntena = lista;
    while (currentAntena)
    {
        fwrite(&currentAntena->frequencia, sizeof(char), 1, f);
        fwrite(&currentAntena->x, sizeof(int), 1, f);
        fwrite(&currentAntena->y, sizeof(int), 1, f);
        printf("Salvando Antena: Frequencia: %c, Coordenadas: (%d, %d)\n", currentAntena->frequencia, currentAntena->x, currentAntena->y);
        currentAntena = currentAntena->next;
    }

    // Escrever um marcador de fim de lista de antenas
    char endMarker = '\0';
    fwrite(&endMarker, sizeof(char), 1, f);

    // Salvar efeitos nefastos
    Nefasto *currentNefasto = efeitos;
    while (currentNefasto)
    {
        fwrite(&currentNefasto->x1, sizeof(int), 1, f);
        fwrite(&currentNefasto->y1, sizeof(int), 1, f);
        fwrite(&currentNefasto->x2, sizeof(int), 1, f);
        fwrite(&currentNefasto->y2, sizeof(int), 1, f);
        currentNefasto = currentNefasto->next;
    }

    // Escrever um marcador de fim de lista de efeitos nefastos
    int endIntMarker = -1;
    fwrite(&endIntMarker, sizeof(int), 1, f);

    fclose(f);
}

// Função para ler os dados de um arquivo binário
void lerDeBinario(Antena **lista, Nefasto **efeitos, int *largura, int *altura, const char *nomeArquivo)
{
    FILE *f = fopen(nomeArquivo, "rb");
    if (!f)
    {
        printf("Erro ao abrir o ficheiro '%s'\n", nomeArquivo);
        return;
    }

    // Ler número de linhas e colunas
    fread(altura, sizeof(int), 1, f);
    fread(largura, sizeof(int), 1, f);
    printf("Lendo dimensões: Altura: %d, Largura: %d\n", *altura, *largura);

    // Ler antenas
    while (1)
    {
        char freq;
        int x, y;
        if (fread(&freq, sizeof(char), 1, f) != 1)
            break;
        if (freq == '\0')
            break; // Verifica o marcador de fim de lista de antenas
        if (fread(&x, sizeof(int), 1, f) != 1)
            break;
        if (fread(&y, sizeof(int), 1, f) != 1)
            break;
        inserirAntena(lista, freq, x, y);
        printf("Lendo Antena: Frequencia: %c, Coordenadas: (%d, %d)\n", freq, x, y);
    }

    // Ler efeitos nefastos
    while (1)
    {
        int x1, y1, x2, y2;
        if (fread(&x1, sizeof(int), 1, f) != 1) break;
        if (x1 == -1) break; // Verifica o marcador de fim de lista de efeitos nefastos
        if (fread(&y1, sizeof(int), 1, f) != 1) break;
        if (fread(&x2, sizeof(int), 1, f) != 1) break;
        if (fread(&y2, sizeof(int), 1, f) != 1) break;
        inserirNefasto(efeitos, NULL, NULL, x1, y1, x2, y2);
    }

    fclose(f);
}

bool inserirNefasto(Nefasto **head, Antena *antena, Antena *antena2, int x1, int y1, int x2, int y2)
{
    Nefasto *novo = criarEfeitoNefasto(antena, antena2, x1, y1, x2, y2);
    if (!novo)
        return false;
    novo->next = *head;
    *head = novo;
    return true;
}

Nefasto *criarEfeitoNefasto(Antena *a1, Antena *a2, int x1, int y1, int x2, int y2)
{
    if (!a1 || !a2)
        return NULL;

    Nefasto *novoEfeito = (Nefasto *)malloc(sizeof(Nefasto));
    if (!novoEfeito)
        return NULL;
    novoEfeito->x1 = x1;
    novoEfeito->y1 = y1;
    novoEfeito->x2 = x2;
    novoEfeito->y2 = y2;
    novoEfeito->next = NULL;
    return novoEfeito;
}

Nefasto *calcularEfeitoNefasto(Antena *lista, int linha, int coluna)
{
    Nefasto *listaNefastos = NULL;

    // Coletar todas as frequências presentes na lista de antenas
    char frequencias[256] = {0};
    Antena *a1 = lista;
    while (a1)
    {
        frequencias[(unsigned char)a1->frequencia] = 1;
        a1 = a1->next;
    }

    // Calcular efeitos nefastos para cada frequência
    for (int i = 0; i < 256; i++)
    {
        if (frequencias[i])
        {
            a1 = lista;
            while (a1)
            {
                if (a1->frequencia == i)
                {
                    Antena *a2 = a1->next;
                    while (a2)
                    {
                        if (a1->frequencia == a2->frequencia)
                        {
                            int tipoEfeito = 0;
                            int dx = a2->x - a1->x;
                            int dy = a2->y - a1->y;
                            int distancia = abs(a1->x - a2->x);
                            if (abs(a1->y - a2->y) > distancia)
                                distancia = abs(a1->y - a2->y);
                            if (distancia <= 3)
                            {
                                if (a1->x == a2->x)
                                {
                                    tipoEfeito = 1;
                                }
                                else if (a1->y == a2->y)
                                {
                                    tipoEfeito = 2;
                                }
                                else if (abs(dx) + abs(dy) <= 4)
                                {
                                    tipoEfeito = 3;
                                }
                            }

                            switch (tipoEfeito)
                            {
                            case 1:
                                if (a1->y < a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x, a1->y - distancia, a2->x, a2->y + distancia);
                                }
                                else
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a2->x, a2->y - distancia, a1->x, a1->y + distancia);
                                }
                                break;
                            case 2:
                                if (a1->x < a2->x)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x - distancia, a1->y, a2->x + distancia, a2->y);
                                }
                                else
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a2->x - distancia, a2->y, a1->x + distancia, a1->y);
                                }
                                break;
                            case 3:
                                // Cima esquerda → Baixo direita
                                if (a1->x < a2->x && a1->y < a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x - distancia, a1->y - distancia, a2->x + distancia, a2->y + distancia);
                                }
                                // Baixo esquerda → Cima direita
                                else if (a1->x < a2->x && a1->y > a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x - distancia, a1->y + distancia, a2->x + distancia, a2->y - distancia);
                                }
                                // Cima direita → Baixo esquerda
                                else if (a1->x > a2->x && a1->y < a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x + distancia, a1->y - distancia, a2->x - distancia, a2->y + distancia);
                                }
                                // Baixo direita → Cima esquerda
                                else if (a1->x > a2->x && a1->y > a2->y)
                                {
                                    inserirNefasto(&listaNefastos, a1, a2, a1->x + distancia, a1->y + distancia, a2->x - distancia, a2->y - distancia);
                                }

                                printf("⚠️ Efeito nefasto entre '%c' em (%d,%d) e (%d,%d)\n",
                                       a1->frequencia, a1->x, a1->y, a2->x, a2->y);

                                break;
                            default:
                                break;
                            }
                        }
                        a2 = a2->next;
                    }
                }
                a1 = a1->next;
            }
        }
    }

    return listaNefastos;
}