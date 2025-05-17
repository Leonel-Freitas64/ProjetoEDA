#include "../resources/Funções.h"
#include "../resources/Estruturas.h"
/**
 * @brief Imprime a matriz com as antenas e/ou efeitos nefastos.
 *
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param colunas Número de colunas da matriz.
 * @param linhas Número de linhas da matriz.
 * @param modo Modo de impressão (1: apenas antenas, 2: apenas efeitos, 3: ambos).
 */
void imprimirMatrizAntenas(Antena *lista, Nefasto *efeitos, int colunas, int linhas, int modo,ListaMensagem *listaMensagens, int idioma)
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
            if (efeitoAtual->x1 < colunas && efeitoAtual->x1 >= 0 && efeitoAtual->y1 < linhas && efeitoAtual->y1 >= 0)
            {
                if (matriz[efeitoAtual->y1][efeitoAtual->x1] != '.')
                {
                    mostrarSobreposicao(listaMensagens, idioma, efeitoAtual->x1, efeitoAtual->y1, matriz[efeitoAtual->y1][efeitoAtual->x1]);
                    matriz[efeitoAtual->y1][efeitoAtual->x1] = '@';
                }
                else
                {
                    matriz[efeitoAtual->y1][efeitoAtual->x1] = '#';
                }
            }
            if (efeitoAtual->x2 < colunas && efeitoAtual->x2 >= 0 && efeitoAtual->y2 < linhas && efeitoAtual->y2 >= 0)
            {
                if (matriz[efeitoAtual->y2][efeitoAtual->x2] != '.')
                {
                    mostrarSobreposicao(listaMensagens, idioma, efeitoAtual->x2, efeitoAtual->y2, matriz[efeitoAtual->y2][efeitoAtual->x2]);
                    matriz[efeitoAtual->y2][efeitoAtual->x2] = '@';
                }
                else
                {
                    matriz[efeitoAtual->y2][efeitoAtual->x2] = '#';
                }
            }
            efeitoAtual = efeitoAtual->next;
        }
    }

    // Imprime os números das colunas
    printf("   ");
    for (int j = 0; j < colunas; j++)
    {
        printf("%2d ", j);
    }
    printf("\n");

    // Imprime a matriz com os números das linhas
    for (int i = 0; i < linhas; i++)
    {
        printf("%2d ", i); // Número da linha
        for (int j = 0; j < colunas; j++)
        {
            printf(" %c ", matriz[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Redimensiona a matriz, permitindo aumentar ou diminuir suas dimensões.
 *
 * @param linhas Ponteiro para o número de linhas da matriz.
 * @param colunas Ponteiro para o número de colunas da matriz.
 * @param lista Ponteiro para a lista de antenas.
 */
/* void aumentarMatriz(int *linhas, int *colunas, Antena *lista)
{
    int maisLinhas, maisColunas;

    // Solicita ao usuário o número de linhas e colunas adicionais
    printf("Quantas linhas adicionais queres adicionar? ");
    scanf("%d", &maisLinhas);
    printf("Quantas colunas adicionais queres adicionar? ");
    scanf("%d", &maisColunas);

    if (maisLinhas < 0 || maisColunas < 0)
    {
        printf("⚠️ Os valores têm de ser positivos!\n");
        return;
    }

    // Atualiza as dimensões da matriz
    *linhas += maisLinhas;
    *colunas += maisColunas;

    printf("✅ Nova dimensão da matriz: %d linhas x %d colunas\n", *linhas, *colunas);

    // Pergunta se o usuário deseja deslocar as antenas
    char opcao;
    printf("Queres deslocar todas as antenas para baixo e/ou para a direita? (s/n): ");
    scanf(" %c", &opcao);

    if (opcao == 's' || opcao == 'S')
    {
        int deslocX, deslocY;
        printf("Quantas colunas queres deslocar para a direita? ");
        scanf("%d", &deslocX);
        printf("Quantas linhas queres deslocar para baixo? ");
        scanf("%d", &deslocY);

        // Desloca as antenas
        Antena *atual = lista;
        while (atual)
        {
            atual->x += deslocX;
            atual->y += deslocY;
            atual = atual->next;
        }
        printf("✅ Antenas deslocadas %d colunas e %d linhas.\n", deslocX, deslocY);
    }
}
 */
