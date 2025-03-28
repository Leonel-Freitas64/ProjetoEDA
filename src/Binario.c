#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

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
        //printf("Salvando Antena: Frequencia: %c, Coordenadas: (%d, %d)\n", currentAntena->frequencia, currentAntena->x, currentAntena->y);
        currentAntena = currentAntena->next;
    }

    // Escrever um marcador de fim de lista de antenas
    char endMarker = '\0';
    fwrite(&endMarker, sizeof(char), 1, f);

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
    //printf("Lendo dimensões: Altura: %d, Largura: %d\n", *altura, *largura);

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

}
