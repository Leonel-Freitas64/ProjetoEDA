/**
 * @file Antenas.c
 * @author Leonel Freitas (a31520@alunos.ipca.pt)
 * @brief Implementação das funções relacionadas à manipulação de antenas.
 * @version 1.1
 * @date 29-03-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

/**
 * @brief Cria uma nova antena.
 * 
 * @param freq Frequência da antena (A-Z, a-z, 0-9).
 * @param x Coordenada X da antena.
 * @param y Coordenada Y da antena.
 * @return Antena* Ponteiro para a nova antena ou NULL em caso de erro.
 */
Antena *criarAntena(char freq, int x, int y)
{
    // Aloca memória para a nova antena
    Antena *nova = malloc(sizeof(Antena));
    if (!nova)
        return NULL;

    // Inicializa os campos da antena
    memset(nova, 0, sizeof(Antena));
    nova->frequencia = freq;
    nova->x = x;
    nova->y = y;
    nova->next = NULL;

    return nova;
}

/**
 * @brief Insere uma antena na lista.
 * 
 * @param head Ponteiro para o ponteiro da lista de antenas.
 * @param freq Frequência da antena.
 * @param x Coordenada X da antena.
 * @param y Coordenada Y da antena.
 * @return true Se a antena foi inserida com sucesso.
 * @return false Se já existe uma antena na mesma posição ou ocorreu um erro.
 */
bool inserirAntena(Antena **head, char freq, int x, int y)
{
    // Verifica se já existe uma antena na mesma posição
    Antena *current = *head;
    while (current)
    {
        if (current->x == x && current->y == y)
        {
            printf("Erro: Já existe uma antena nas coordenadas (%d, %d)\n", x, y);
            return false;
        }
        current = current->next;
    }

    // Cria e insere a nova antena no início da lista
    Antena *nova = criarAntena(freq, x, y);
    if (!nova)
        return false;
    nova->next = *head;
    *head = nova;

    return true;
}

/**
 * @brief Remove uma antena da lista.
 * 
 * @param head Ponteiro para o ponteiro da lista de antenas.
 * @param x Coordenada X da antena a ser removida.
 * @param y Coordenada Y da antena a ser removida.
 * @return true Se a antena foi removida com sucesso.
 * @return false Se a antena não foi encontrada ou ocorreu um erro.
 */
bool eliminarAntena(Antena **head, int x, int y)
{
    if (!head || !*head)
    {
        return false; // Lista vazia
    }

    Antena *atual = *head;
    Antena *anterior = NULL;

    // Percorre a lista para encontrar a antena com as coordenadas (x, y)
    while (atual)
    {
        if (atual->x == x && atual->y == y)
        {
            // Caso a antena seja encontrada
            if (anterior)
            {
                // Ajusta o ponteiro do nó anterior
                anterior->next = atual->next;
            }
            else
            {
                // Atualiza o head se for a primeira antena
                *head = atual->next;
            }
            free(atual); // Libera a memória da antena removida
            return true; // Antena removida com sucesso
        }
        anterior = atual;
        atual = atual->next;
    }

    return false; // Antena não encontrada
}

/**
 * @brief Solicita ao usuário os dados de uma antena.
 * 
 * @param freq Ponteiro para armazenar a frequência da antena.
 * @param x Ponteiro para armazenar a coordenada X.
 * @param y Ponteiro para armazenar a coordenada Y.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 */
void recolherDadosAntena(char *freq, int *x, int *y, int linha, int coluna)
{
    // Solicita a frequência da antena ao usuário
    do
    {
        printf("Digite a frequência da antena (A-Z, a-z, 0-9): ");
        scanf(" %c", freq);
        if (!((*freq >= 'A' && *freq <= 'Z') || (*freq >= 'a' && *freq <= 'z') || (*freq >= '0' && *freq <= '9')))
        {
            printf("Frequência inválida! Deve ser uma letra (A-Z, a-z) ou um número (0-9).\n");
        }
    } while (!((*freq >= 'A' && *freq <= 'Z') || (*freq >= 'a' && *freq <= 'z') || (*freq >= '0' && *freq <= '9')));

    // Solicita a coordenada X da antena ao usuário
    do
    {
        printf("Digite a posição X da antena (0 a %d): ", coluna - 1);
        scanf("%d", x);
        if (*x < 0 || *x >= coluna)
        {
            printf("Posição X inválida! Deve estar entre 0 e %d.\n", coluna - 1);
        }
    } while (*x < 0 || *x >= coluna);

    // Solicita a coordenada Y da antena ao usuário
    do
    {
        printf("Digite a posição Y da antena (0 a %d): ", linha - 1);
        scanf("%d", y);
        if (*y < 0 || *y >= linha)
        {
            printf("Posição Y inválida! Deve estar entre 0 e %d.\n", linha - 1);
        }
    } while (*y < 0 || *y >= linha);
}

/**
 * @brief Lê antenas de um arquivo e as insere na lista.
 * 
 * @param nomeFicheiro Nome do arquivo a ser lido.
 * @param lista Ponteiro para a lista de antenas.
 * @param linha Ponteiro para armazenar o número de linhas da matriz.
 * @param coluna Ponteiro para armazenar o número de colunas da matriz.
 * @return Antena* Ponteiro para a lista de antenas.
 */
Antena *lerAntenasDoFicheiro(const char *nomeFicheiro, Antena **lista, int *linha, int *coluna)
{
    FILE *f = fopen(nomeFicheiro, "r");
    if (!f)
    {
        printf("Erro ao abrir o ficheiro '%s'\n", nomeFicheiro);
        return NULL;
    }

    // Libera a memória da lista de antenas existente, se necessário
    if (*lista)
    {
        liberarAntenas(*lista);
        *lista = NULL;
    }

    char buffer[256];
    int y = 0;
    int maxColuna = 0;

    // Lê o arquivo linha por linha
    while (fgets(buffer, sizeof(buffer), f))
    {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Remove espaços em branco à direita
        int len = strlen(buffer);
        while (len > 0 && isspace(buffer[len - 1]))
        {
            buffer[--len] = '\0';
        }

        // Verifica se a linha está vazia
        int vazia = 1;
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (!isspace(buffer[i]))
            {
                vazia = 0;
                break;
            }
        }
        if (vazia)
            continue;

        // Atualiza o comprimento máximo da linha
        int linhaComprimento = len;
        if (linhaComprimento > maxColuna)
        {
            maxColuna = linhaComprimento;
        }

        // Insere as antenas encontradas na linha
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            char c = buffer[i];
            if (isalpha(c) || isdigit(c))
            {
                if (inserirAntena(lista, c, i, y) == true)
                {
                    // Inserção bem-sucedida
                }
                else
                {
                    printf("Erro ao inserir antena na posição (%d, %d)\n", i, y);
                }
            }
        }
        y++;
    }

    *linha = y;
    *coluna = maxColuna;

    fclose(f);
    return *lista;
}

/**
 * @brief Imprime a lista de antenas.
 * 
 * @param lista Ponteiro para a lista de antenas.
 */
void listarAntenas(Antena *lista)
{
    Antena *current = lista;
    while (current)
    {
        printf("Frequencia: %c, Coordenadas: (%d, %d)\n", current->frequencia, current->x, current->y);
        current = current->next;
    }
}

/**
 * @brief Libera a memória da lista de antenas.
 * 
 * @param lista Ponteiro para a lista de antenas.
 */
void liberarAntenas(Antena *lista)
{
    Antena *atual = lista;
    while (atual)
    {
        Antena *temp = atual;
        atual = atual->next;
        free(temp);
    }
}