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
    Antena *nova = (Antena *)malloc(sizeof(Antena));
    if (!nova)
    {
        return NULL;
    }

    nova->frequencia = freq;
    nova->x = x;
    nova->y = y;
    nova->next = NULL;

    return nova;
}

Antena *encontrarAntenaPorCoordenadas(Antena *lista, int x, int y)
{
    if (!lista)
    {
        return NULL;
    }

    Antena *atual = lista;
    while (atual)
    {
        if (atual->x == x && atual->y == y)
        {
            return atual; // Retorna a antena encontrada
        }
        atual = atual->next;
    }

    return NULL; // Retorna NULL se a antena não for encontrada
}

Antena *selecionarAntena(Antena *lista, int linha, int coluna, ListaMensagem *listaMensagens, int idioma)
{
    int x, y;
    char f; // ignorado
    if (recolherDadosAntena(listaMensagens, idioma, &f, &x, &y, linha, coluna) == false)
    {
        return NULL;
    }

    Antena *antenaEncontrada = encontrarAntenaPorCoordenadas(lista, x, y);
    if (!antenaEncontrada)
    {
        mostrarMensagem(listaMensagens, idioma, 806); // "Antena com coordenadas (%d, %d) não encontrada."
        return NULL;
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
Antena *inserirAntena(Antena *lista, char freq, int x, int y)
{
    // Verifica se já existe uma antena na mesma posição
    Antena *atual = lista;
    while (atual)
    {
        if (atual->x == x && atual->y == y)
        {
            return NULL; // Retorna NULL indicando falha
        }
        atual = atual->next;
    }

    // Cria uma nova antena
    Antena *nova = criarAntena(freq, x, y);
    if (!nova)
    {
        return NULL; // Retorna NULL em caso de erro
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
Antena *eliminarAntena(Antena *lista, int x, int y, Nefasto *listaNefastos, ListaMensagem *listaMensagens, int idioma)
{
    if (!lista)
    {
        mostrarMensagem(listaMensagens, idioma, 803); // "Erro: A lista de antenas está vazia."
        return NULL;
    }

    Antena *atual = lista;
    Antena *anterior = NULL;

    while (atual)
    {
        if (atual->x == x && atual->y == y)
        {
            // Antena encontrada, remove da lista
            if (anterior)
            {
                anterior->next = atual->next;
            }
            else
            {
                lista = atual->next; // Atualiza o início da lista
            }

            // Remove referências à antena na lista de efeitos nefastos
            listaNefastos = removerEfeitosPorAntena(listaNefastos, atual);

            Mensagem *mensagem = obterMensagem(listaMensagens, idioma, 805); // "Antena removida: Frequência '%c', Coordenadas (%d, %d)"
            if (mensagem)
            {
                printf(mensagem->texto, atual->frequencia, atual->x, atual->y);
            }

            // Libera a memória da antena
            free(atual);

            return lista;
        }

        anterior = atual;
        atual = atual->next;
    }

    mostrarMensagem(listaMensagens, idioma, 804); // "Antena com coordenadas (%d, %d) não encontrada."
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
bool recolherDadosAntena(ListaMensagem *listaMensagens, int idioma, char *freq, int *x, int *y, int linha, int coluna)
{
    // Valida os ponteiros de entrada
    if (!freq || !x || !y)
        return false;
    char buffer[256]; // Buffer para capturar entradas do usuário

    // Solicita a frequência da antena ao usuário
    do
    {
        mostrarMensagem(listaMensagens, idioma, 700); // Código 700: "Digite a frequência da antena (A-Z, a-z): \n 0 - Voltar para menu: "
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, " %c", freq) == 1)
            {
                if (*freq == '0')
                {
                    return false; // Retorna falso se o usuário escolher voltar
                }
                else if ((*freq >= 'A' && *freq <= 'Z') || (*freq >= 'a' && *freq <= 'z'))
                {
                    break; // Frequência válida
                }
            }
        }
        mostrarMensagem(listaMensagens, idioma, 701); // Código 701: "Frequência inválida! Deve ser uma letra (A-Z, a-z)."
    } while (1);

    // Solicita a coordenada X da antena ao usuário
    do
    {
        Mensagem *mensagem = obterMensagem(listaMensagens, idioma, 702); // Código 702: "Digite a coordenada X da antena (0 a coluna-1): "
        if (mensagem)
        {
            printf(mensagem->texto, coluna - 1);
        }
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", x) == 1 && *x >= 0 && *x < coluna)
            {
                break; // Coordenada X válida
            }
        }
        mensagem = obterMensagem(listaMensagens, idioma, 703); // Código 703: "Coordenada X inválida! Deve estar entre 0 e coluna-1."
        if (mensagem)
        {
            printf(mensagem->texto, coluna - 1);
        }
    } while (1);

    // Solicita a coordenada Y da antena ao usuário
    do
    {
        Mensagem *mensagem = obterMensagem(listaMensagens, idioma, 704); // Código 704: "Digite a coordenada Y da antena (0 a linha-1): "
        if (mensagem)
        {
            printf(mensagem->texto, linha - 1);
        }
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", y) == 1 && *y >= 0 && *y < linha)
            {
                break; // Coordenada Y válida
            }
        }
        mensagem = obterMensagem(listaMensagens, idioma, 705); // Código 705: "Coordenada Y inválida! Deve estar entre 0 e linha-1."
        if (mensagem)
        {
            printf(mensagem->texto, linha - 1);
        }
    } while (1);

    return true; // Retorna true se os dados forem recolhidos com sucesso
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
        return NULL;
    }
    Antena *lista = NULL;
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
            if (isalpha(c))
            {
                if (inserirAntena(lista, c, i, y) != NULL)
                {
                    lista = inserirAntena(lista, c, i, y);
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
bool listarAntenas(Antena *lista, ListaMensagem *listaMensagens, int idioma)
{
    if (!lista)
    {
        mostrarMensagem(listaMensagens, idioma, 800); // "Lista vazia!"
        return false;
    }

    Antena *atual = lista;
    while (atual)
    {
        Mensagem *mensagem = obterMensagem(listaMensagens, idioma, 801); // "Frequência: %c, Coordenadas: (%d, %d)"
        if (mensagem)
        {
            printf(mensagem->texto, atual->frequencia, atual->x, atual->y);
        }
        atual = atual->next;
    }
    return true;
}

/**
 * @brief Libera a memória da lista de antenas.
 *
 * @param lista Ponteiro para a lista de antenas.
 */
Antena *liberarAntenas(Antena *lista)
{
    Antena *atual = lista;
    while (atual)
    {
        Antena *temp = atual;
        atual = atual->next;
        free(temp);
    }
    return NULL;
}