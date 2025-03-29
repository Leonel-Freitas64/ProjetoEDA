/**
 * @file Funções.c
 * @author Leonel Freitas (a31520@alunos.ipca.pt)
 * @brief Implementação das funções principais do programa, incluindo menus e manipulação de dados.
 * @version 1.1
 * @date 29-03-2025
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

/**
 * @brief Limpa a tela do terminal.
 */
void limpartela()
{
    // Comando para limpar a tela, compatível com Windows e Unix
    system("cls||clear");
}

/**
 * @brief Mostra o menu principal do programa.
 */
void mostrarMenuPrincipal()
{
    printf("Menu Principal:\n");
    printf("1. Ler arquivo binário\n");
    printf("2. Ler matriz\n");
    printf("3. Sair\n");
}

/**
 * @brief Mostra o menu secundário do programa.
 */
void mostrarMenuSecundario()
{
    printf("Menu Secundário:\n");
    printf("1. Adicionar antena\n");
    printf("2. Eliminar antena\n");
    printf("3. Verificar Efeitos Nefasto\n");
    printf("4. Imprimir Matriz\n");
    printf("5. Redimensionar a matriz\n\n");
    printf("6. Voltar ao menu principal\n");
}

/**
 * @brief Processa as opções do menu secundário.
 * 
 * @param opcao Opção escolhida pelo usuário.
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @param primeiraVez Ponteiro para o indicador de primeira execução.
 */
void processarOpcaoSecundaria(int opcao, Antena **lista, Nefasto **efeitos, int linha, int coluna, bool *primeiraVez)
{
    char freq;
    int x, y, opcaomatriz;

    switch (opcao)
    {
    case 1:
        // Adiciona uma nova antena
        recolherDadosAntena(&freq, &x, &y, linha, coluna);
        if (inserirAntena(lista, freq, x, y))
        {
            printf("Antena adicionada com sucesso!\n");
        }
        else
        {
            printf("Erro ao adicionar antena!\n");
        }
        break;
    case 2:
        // Remove uma antena existente
        recolherDadosAntena(&freq, &x, &y, linha, coluna);
        if (eliminarAntena(lista, x, y))
        {
            printf("Antena eliminada com sucesso.\n");
        }
        else
        {
            printf("Antena não encontrada.\n");
        }
        break;
    case 3:
        // Lista os efeitos nefastos
        listarEfeitosNefastos(*efeitos);
        return;
    case 4:
        // Imprime a matriz com antenas e/ou efeitos
        printf("Escolha o modo de impressão:\n");
        printf("1. Apenas antenas\n");
        printf("2. Apenas efeitos nefastos\n");
        printf("3. Antenas e efeitos nefastos\n");
        scanf("%d", &opcaomatriz);
        imprimirMatrizAntenas(*lista, *efeitos, coluna, linha, opcaomatriz);
        return;
    case 5:
        //aumentarMatriz(&linha, &coluna, *lista);
        printf("Função temporariamente desativada.\n");
        break;
    case 6:
        // Volta ao menu principal
        limpartela();
        return;
    default:
        printf("Opção inválida.\n");
        return;
    }

    // Recalcula ou verifica os efeitos nefastos após alterações
    if (*primeiraVez)
    {
        *efeitos = calcularEfeitoNefasto(*lista, linha, coluna);
        *primeiraVez = false;
    }
    else
    {
        *efeitos = verificaEfeitoNefasto(*lista, efeitos, linha, coluna);
    }

    // Atualiza a matriz e salva os dados
    imprimirMatrizAntenas(*lista, *efeitos, coluna, linha, 3);
    salvarEmBinario(*lista, *efeitos, coluna, linha, "../bin/dados.bin");
}

/**
 * @brief Processa as opções do menu principal.
 * 
 * @param opcao Opção escolhida pelo usuário.
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param linha Ponteiro para o número de linhas da matriz.
 * @param coluna Ponteiro para o número de colunas da matriz.
 * @param primeiraVez Ponteiro para o indicador de primeira execução.
 */
void processarOpcao(int opcao, Antena **lista, Nefasto **efeitos, int *linha, int *coluna, bool *primeiraVez)
{
    switch (opcao)
    {
    case 1:
        // Lê os dados de um arquivo binário
        lerDeBinario(lista, efeitos, coluna, linha, "../bin/dados.bin");
        if (!*lista)
        {
            printf("Lista vazia ou erro ao ler!\n");
        }
        else
        {
            // Calcula ou verifica os efeitos nefastos
            if (*primeiraVez)
            {
                *efeitos = calcularEfeitoNefasto(*lista, *linha, *coluna);
                *primeiraVez = false;
            }
            else
            {
                *efeitos = verificaEfeitoNefasto(*lista, efeitos, *linha, *coluna);
            }
            imprimirMatrizAntenas(*lista, *efeitos, *coluna, *linha, 3);

            // Mostra o menu secundário
            int opcaoSecundaria;
            do
            {
                mostrarMenuSecundario();
                printf("Escolha uma opção: ");
                scanf("%d", &opcaoSecundaria);
                processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna, primeiraVez);
            } while (opcaoSecundaria != 6);
        }
        break;
    case 2:
        // Lê os dados de um arquivo de texto
        *lista = lerAntenasDoFicheiro("matriz.txt", lista, linha, coluna);
        if (!*lista)
        {
            printf("Lista vazia ou erro ao ler!\n");
        }
        else
        {
            // Calcula ou verifica os efeitos nefastos
            if (*primeiraVez)
            {
                *efeitos = calcularEfeitoNefasto(*lista, *linha, *coluna);
                *primeiraVez = false;
            }
            else
            {
                *efeitos = verificaEfeitoNefasto(*lista, efeitos, *linha, *coluna);
            }
            imprimirMatrizAntenas(*lista, *efeitos, *coluna, *linha, 3);
            salvarEmBinario(*lista, *efeitos, *coluna, *linha, "../bin/dados.bin");

            // Mostra o menu secundário
            int opcaoSecundaria;
            do
            {
                mostrarMenuSecundario();
                printf("Escolha uma opção: ");
                scanf("%d", &opcaoSecundaria);
                processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna, primeiraVez);
            } while (opcaoSecundaria != 6);
        }
        break;
    case 3:
        // Sai do programa
        printf("Saindo...\n");
        exit(0);
    default:
        printf("Opção inválida\n");
        break;
    }
}

/**
 * @brief Executa o menu principal do programa.
 * 
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param linha Ponteiro para o número de linhas da matriz.
 * @param coluna Ponteiro para o número de colunas da matriz.
 * @param primeraVez Ponteiro para o indicador de primeira execução.
 */
void executarMenu(Antena **lista, Nefasto **efeitos, int *linha, int *coluna, bool *primeraVez)
{
    int opcao;
    do
    {
        // Mostra o menu principal e processa a opção escolhida
        mostrarMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        processarOpcao(opcao, lista, efeitos, linha, coluna, primeraVez);
    } while (opcao != 3);
}

/**
 * @brief Imprime a matriz com as antenas e/ou efeitos nefastos.
 * 
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param colunas Número de colunas da matriz.
 * @param linhas Número de linhas da matriz.
 * @param modo Modo de impressão (1: apenas antenas, 2: apenas efeitos, 3: ambos).
 */
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
            if (efeitoAtual->x1 < colunas && efeitoAtual->x1 >= 0 && efeitoAtual->y1 < linhas && efeitoAtual->y1 >= 0)
            {
                if (matriz[efeitoAtual->y1][efeitoAtual->x1] != '.')
                {
                    printf("⚠️ Sobreposição detectada em (%d, %d) com '%c'\n",
                           efeitoAtual->x1, efeitoAtual->y1, matriz[efeitoAtual->y1][efeitoAtual->x1]);
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
                    printf("⚠️ Sobreposição detectada em (%d, %d) com '%c'\n",
                           efeitoAtual->x2, efeitoAtual->y2, matriz[efeitoAtual->y2][efeitoAtual->x2]);
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

/**
 * @brief Redimensiona a matriz, permitindo aumentar ou diminuir suas dimensões.
 * 
 * @param linhas Ponteiro para o número de linhas da matriz.
 * @param colunas Ponteiro para o número de colunas da matriz.
 * @param lista Ponteiro para a lista de antenas.
 */
void aumentarMatriz(int *linhas, int *colunas, Antena *lista)
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