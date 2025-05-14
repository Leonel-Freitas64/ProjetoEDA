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
#include "../resources/Grafos.h"

/**
 * @brief Limpa a tela do terminal.
 *
 * Esta função utiliza comandos do sistema operacional para limpar a tela do terminal.
 * No Windows, o comando "cls" é usado, enquanto no Unix/Linux/MacOS, o comando "clear" é utilizado.
 * O operador "||" garante que o comando correto seja executado dependendo do sistema.
 */
void limpartela()
{
    system("cls||clear");
}

/**
 * @brief Mostra o menu principal do programa.
 *
 * Este menu apresenta as opções principais disponíveis para o usuário.
 * Ele é exibido no início do programa e após o retorno do menu secundário.
 */
void mostrarMenuPrincipal()
{
    printf("Menu Principal:\n");
    printf("1. Ler arquivo binário\n"); // Opção para carregar dados de um arquivo binário.
    printf("2. Ler matriz\n");          // Opção para carregar dados de um arquivo de texto.
    printf("3. Sair\n");                // Opção para encerrar o programa.
}

/**
 * @brief Mostra o menu secundário do programa.
 *
 * Este menu é exibido após o carregamento dos dados e permite ao usuário
 * realizar operações específicas, como adicionar ou remover antenas, verificar
 * efeitos nefastos, e redimensionar a matriz.
 */
void mostrarMenuSecundario()
{
    printf("Menu Secundário:\n");
    printf("1. Adicionar antena\n");
    printf("2. Eliminar antena\n");
    printf("3. Verificar Efeitos Nefasto\n");
    printf("4. Imprimir Matriz\n\n");
    // printf("3. Redimensionar a matriz. POR IMPLEMENTAR\n\n");
    printf("5. Construir grafo\n");
    printf("6. Voltar ao menu principal\n");
}

/**
 * @brief Aguarda o usuário pressionar Enter para continuar.
 */
void esperarEnter()
{
    printf("Pressione Enter para continuar...");
    while (getchar() != '\n')
        ;      // Limpa o buffer de entrada
    getchar(); // Aguarda o Enter
}
/**
 * @brief Processa as opções do menu secundário.
 *
 * Esta função é responsável por executar a ação correspondente à opção escolhida pelo usuário no menu secundário.
 * Ela manipula a lista de antenas e efeitos nefastos, além de atualizar a matriz e salvar os dados em um arquivo binário.
 *
 * @param opcao Opção escolhida pelo usuário.
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @param primeiraVez Ponteiro para o indicador de primeira execução.
 */
void processarOpcaoSecundaria(int opcao, Antena *lista, Nefasto *efeitos, int linha, int coluna, _Bool primeiraVez)
{
    char freq;       // Variável para armazenar a frequência da antena.
    int x, y;        // Variáveis para armazenar as coordenadas da antena.
    int opcaomatriz; // Variável para armazenar o modo de impressão da matriz.

    switch (opcao)
    {
    case 1:
        // Adiciona uma nova antena à lista.
        if (recolherDadosAntena(&freq, &x, &y, linha, coluna))
        {
            lista = inserirAntena(lista, freq, x, y);
            efeitos = calcularEfeitoNefasto(lista, linha, coluna);
            esperarEnter();
        }
        else
        {
            printf("Erro: Dados inválidos para a antena.\n");
        }
        break;

    case 2:
        listarAntenas(lista);
        if (recolherDadosAntena(&freq, &x, &y, linha, coluna))
        {
            lista = eliminarAntena(lista, x, y);

            // Libera a lista de efeitos nefastos antes de recalcular
            liberarEfeitosNefastos(efeitos);

            // Recalcula os efeitos nefastos com a lista atualizada
            efeitos = calcularEfeitoNefasto(lista, linha, coluna);
        }
        else
        {
            printf("Erro: Dados inválidos para a antena.\n");
        }
        esperarEnter();
        break;

    case 3:
        // Verifica os efeitos nefastos.
        efeitos = calcularEfeitoNefasto(lista, linha, coluna);
        listarEfeitosNefastos(efeitos);
        esperarEnter();
        break;

    case 4:
        // Imprime a matriz de antenas e efeitos nefastos.
        do
        {
            printf("Escolha o modo de impressão da matriz (1 ou 2): ");
            scanf("%d", &opcaomatriz);

            if (opcaomatriz == 1 || opcaomatriz == 2)
            {
                imprimirMatrizAntenas(lista, efeitos, coluna, linha, opcaomatriz);
            }
            else
            {
                printf("Modo de impressão inválido. Tente novamente.\n");
            }

            // Pergunta ao usuário se deseja imprimir novamente
            printf("Deseja imprimir a matriz novamente? (1 - Sim, 0 - Não): ");
            scanf("%d", &opcao);

        } while (opcao == 1); // Repete enquanto o usuário escolher "Sim"
        break;

    case 5:
        Antena *antena;
        Grafo *grafo = construirGrafo(lista);
        listarAntenas(lista);
        antena = selecionarAntena(lista, linha, coluna);
        if (antena)
        {
            printf("Antena selecionada: %c (%d, %d)\n", antena->frequencia, antena->x, antena->y);

            dfs(grafo, antena);
            mostrarCaminhos(grafo, antena);
        }
        else
        {
            printf("Antena não encontrada.\n");
        }
        esperarEnter();
        break;

    default:
        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        break;
    }

    // Atualiza a matriz e salva os dados em um arquivo binário.
    limpartela();
    imprimirMatrizAntenas(lista, efeitos, coluna, linha, 3);
    salvarEmBinario(lista, coluna, linha, "../bin/dados.bin");
}

/**
 * @brief Processa as opções do menu principal.
 *
 * Esta função é responsável por executar a ação correspondente à opção escolhida pelo usuário no menu principal.
 * Ela coordena o carregamento de dados, a exibição do menu secundário e a saída do programa.
 *
 * @param opcao Opção escolhida pelo usuário.
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param linha Ponteiro para o número de linhas da matriz.
 * @param coluna Ponteiro para o número de colunas da matriz.
 * @param primeiraVez Ponteiro para o indicador de primeira execução.
 */
void processarOpcao(int opcao, Antena *lista, Nefasto *efeitos, int *linha, int *coluna, bool *primeiraVez)
{
    switch (opcao)
    {
    case 1:
        // Carrega os dados de um arquivo binário
        lista = lerDeBinario(coluna, linha, "../bin/dados.bin");
        if (!lista)
        {
            printf("Lista vazia ou erro ao ler!\n");
        }
        else
        {
            if (*primeiraVez)
            {
                efeitos = calcularEfeitoNefasto(lista, *linha, *coluna);
                *primeiraVez = false;
            }
            else
            {
                efeitos = calcularEfeitoNefasto(lista, *linha, *coluna);
            }
            imprimirMatrizAntenas(lista, efeitos, *coluna, *linha, 3);
            int opcaoSecundaria;
            do
            {
                mostrarMenuSecundario();
                printf("Escolha uma opção: ");
                scanf("%d", &opcaoSecundaria);
                processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna, primeiraVez);
            } while (opcaoSecundaria != 6);
            limpartela();
        }
        break;
    case 2:
        // Carrega os dados de um arquivo de texto
        lista = lerAntenasDoFicheiro("matriz.txt", linha, coluna);
        if (!lista)
        {
            printf("Lista vazia ou erro ao ler!\n");
        }
        else
        {
            if (*primeiraVez)
            {
                efeitos = calcularEfeitoNefasto(lista, *linha, *coluna);
                *primeiraVez = false;
            }
            else
            {
                liberarEfeitosNefastos(efeitos);
                efeitos = calcularEfeitoNefasto(lista, *linha, *coluna);
            }
            imprimirMatrizAntenas(lista, efeitos, *coluna, *linha, 3);
            int opcaoSecundaria;
            do
            {
                mostrarMenuSecundario();
                printf("Escolha uma opção: ");
                scanf("%d", &opcaoSecundaria);
                processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna, primeiraVez);
            } while (opcaoSecundaria != 6);
            limpartela();
        }
        break;
    case 3:
        // Encerra o programa.
        printf("Saindo...\n");
        exit(0);
    default:
        printf("Opção inválida!\n");
        break;
    }
}
/**
 * @brief Executa o menu principal do programa.
 *
 * Esta função exibe o menu principal em um loop até que o usuário escolha sair do programa.
 * Ela chama a função `processarOpcao` para executar as ações correspondentes às opções escolhidas.
 *
 * @param lista Ponteiro para a lista de antenas.
 * @param efeitos Ponteiro para a lista de efeitos nefastos.
 * @param linha Ponteiro para o número de linhas da matriz.
 * @param coluna Ponteiro para o número de colunas da matriz.
 * @param primeraVez Ponteiro para o indicador de primeira execução.
 */
Antena *executarMenu(Antena *lista, Nefasto *efeitos, int *linha, int *coluna, bool *primeiraVez)
{
    int opcao; // Variável para armazenar a opção escolhida pelo usuário.
    do
    {
        // Exibe o menu principal e solicita a escolha do usuário.
        mostrarMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Processa a opção escolhida e atualiza a lista de antenas.
        processarOpcao(opcao, lista, efeitos, linha, coluna, primeiraVez);
    } while (opcao != 3); // Continua até que o usuário escolha sair.

    return lista; // Retorna a lista atualizada.
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