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
    Antena* criarAntena(char freq, int x, int y) {
        Antena* nova = (Antena*)malloc(sizeof(Antena));
        if (!nova) {
            printf("Erro: Falha ao alocar memória para a nova antena.\n");
            return NULL;
        }

        nova->frequencia = freq;
        nova->x = x;
        nova->y = y;
        nova->next = NULL;

        return nova;
    }

Antena* encontrarAntenaPorCoordenadas(Antena* lista, int x, int y) {
    if (!lista) {
        printf("Erro: A lista de antenas está vazia.\n");
        return NULL;
    }

    Antena* atual = lista;
    while (atual) {
        if (atual->x == x && atual->y == y) {
            return atual; // Retorna a antena encontrada
        }
        atual = atual->next;
    }

    printf("Antena com coordenadas (%d, %d) não encontrada.\n", x, y);
    return NULL; // Retorna NULL se a antena não for encontrada
}

Antena* selecionarAntena(Antena* lista, int linha, int coluna) {
    int x, y;
    char f; // ignorado
    if (recolherDadosAntena(&f, &x, &y, linha, coluna) == false) {
        printf("Erro ao recolher os dados da antena.\n");
        return NULL;
    }

    Antena* antenaEncontrada = encontrarAntenaPorCoordenadas(lista, x, y);
    if (!antenaEncontrada) {
        printf("Antena com frequência '%c' e coordenadas (%d, %d) não encontrada.\n", f, x, y);
    }
    return antenaEncontrada;
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
Antena* inserirAntena(Antena* lista, char freq, int x, int y) {
    // Verifica se já existe uma antena na mesma posição
    Antena* atual = lista;
    while (atual) {
        if (atual->x == x && atual->y == y) {
            printf("Erro: Já existe uma antena na posição (%d, %d).\n", x, y);
            return lista; // Retorna a lista sem alterações
        }
        atual = atual->next;
    }

    // Cria uma nova antena
    Antena* nova = criarAntena(freq, x, y);
    if (!nova) {
        return lista; // Retorna a lista sem alterações em caso de erro
    }

    // Insere a nova antena no início da lista
    nova->next = lista;
    return nova; // Retorna a nova lista com a antena adicionada
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
Antena* eliminarAntena(Antena* lista, int x, int y) {
    if (!lista) {
        printf("Erro: A lista de antenas está vazia.\n");
        return NULL;
    }

    Antena* atual = lista;
    Antena* anterior = NULL;

    while (atual) {
        if (atual->x == x && atual->y == y) {
            // Antena encontrada, remove da lista
            if (anterior) {
                anterior->next = atual->next;
            } else {
                lista = atual->next; // Atualiza o início da lista
            }

            // Imprime os dados da antena antes de liberar a memória
            printf("Antena removida: Frequência '%c', Coordenadas (%d, %d)\n", atual->frequencia, atual->x, atual->y);

            // Libera a memória da antena
            free(atual);
            return lista;
        }

        anterior = atual;
        atual = atual->next;
    }

    printf("Erro: Antena com coordenadas (%d, %d) não encontrada.\n", x, y);
    return lista;
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
bool recolherDadosAntena(char *freq, int *x, int *y, int linha, int coluna)
{
    // Solicita a frequência da antena ao usuário
    if(!freq || !x || !y) return false;
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
    return true;
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
Antena *lerAntenasDoFicheiro(const char *nomeFicheiro, int *linha, int *coluna)
{
    FILE *f = fopen(nomeFicheiro, "r");
    if (!f)
    {
        printf("Erro ao abrir o ficheiro '%s'\n", nomeFicheiro);
        return NULL;
    }
    Antena* lista = NULL;
    char buffer[256];
    int y = 0;
    int maxColuna = 0;

    // Lê o arquivo linha por linha
    while (fgets(buffer, sizeof(buffer), f))
    {
        buffer[strcspn(buffer, "\r\n")] = '\0'; // Remove caracteres de nova linha

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
                if (inserirAntena(lista, c, i, y) != NULL)
                {
                    lista = inserirAntena(lista, c, i, y);
                }
                else
                {
                    printf("Erro ao inserir antena na posição (%d, %d)\n", i, y);
                }
            }
        }
        y++;
    }

    fclose(f);

    // Atualiza as dimensões da matriz
    *linha = y;
    *coluna = maxColuna;

    return lista;
}

/**
 * @brief Imprime a lista de antenas.
 * 
 * @param lista Ponteiro para a lista de antenas.
 */
bool listarAntenas(Antena* lista) {
    if (!lista) {
        printf("Lista vazia!\n");
        return false;
    }

    Antena* atual = lista;
    while (atual) {
        printf("Frequência: %c, Coordenadas: (%d, %d)\n", atual->frequencia, atual->x, atual->y);
        atual = atual->next;
    }
    return true;
}

/**
 * @brief Libera a memória da lista de antenas.
 * 
 * @param lista Ponteiro para a lista de antenas.
 */
bool liberarAntenas(Antena *lista)
{
    if (!lista)
    {
        return false;
    }
    Antena *atual = lista;
    while (atual)
    {
        Antena *temp = atual;
        atual = atual->next;
        free(temp);
    }
    return true;
}