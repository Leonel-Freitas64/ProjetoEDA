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
    printf("0. Sair\n");                // Opção para encerrar o programa.
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
    printf("4. Imprimir Matriz\n");
    // printf("3. Redimensionar a matriz. POR IMPLEMENTAR\n\n");
    printf("5. Construir grafo\n\n");
    printf("0. Voltar ao menu principal\n");
}

/**
 * @brief Processa a entrada do usuário para o menu principal.
 *
 * @return int A opção escolhida pelo usuário.
 */
int obterOpcaoMenu()
{
    int opcao;
    char buffer[256]; // Buffer para capturar entradas inválidas

    while (true)
    {
        printf("Escolha uma opção: ");
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            // Tenta converter a entrada para um número inteiro
            if (sscanf(buffer, "%d", &opcao) == 1)
            {
                if (opcao >= 0 && opcao <= 3)
                {
                    return opcao; // Retorna a opção válida
                }
            }
        }
        printf("Opção inválida! Por favor, insira um número entre 1 e 3.\n");
    }
}

int obterOpcaoMenuSecundario()
{
    int opcao;
    char buffer[256]; // Buffer para capturar entradas inválidas

    while (true)
    {
        printf("Escolha uma opção: ");
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            // Tenta converter a entrada para um número inteiro
            if (sscanf(buffer, "%d", &opcao) == 1)
            {
                if (opcao >= 0 && opcao <= 5)
                {
                    return opcao; // Retorna a opção válida
                }
            }
        }
        printf("Opção inválida! Por favor, insira um número entre 0 e 5.\n");
    }
}

int executarMenuSecundario(Antena *lista, Nefasto *efeitos, int *linha, int *coluna)
{
    efeitos = calcularEfeitoNefasto(lista, *linha, *coluna);
    imprimirMatrizAntenas(lista, efeitos, *coluna, *linha, 3);
    int opcaoSecundaria;
    do
    {
        mostrarMenuSecundario();
        opcaoSecundaria = obterOpcaoMenuSecundario();
        lista = processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna);
    } while (opcaoSecundaria != 0);
    lista = liberarAntenas(lista);
    efeitos = liberarEfeitosNefastos(efeitos);
    limpartela();
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
 */
Antena *processarOpcaoSecundaria(int opcao, Antena *lista, Nefasto *efeitos, int linha, int coluna)
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
        }
        else
        {
            // printf("Erro: Dados inválidos para a antena.\n");
        }
        esperarEnter();
        break;

    case 2:
        listarAntenas(lista);
        if (recolherDadosAntena(&freq, &x, &y, linha, coluna))
        {
            lista = eliminarAntena(lista, x, y, efeitos);

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
        listarEfeitosNefastos(efeitos);
        esperarEnter();
        break;

    case 4:
        // Imprime a matriz de antenas e efeitos nefastos.
        limpartela();
        printf("Escolha o modo de impressão da matriz (1 apenas antenas ou 2 apenas nefastos): ");
        scanf("%d", &opcaomatriz);

        if (opcaomatriz == 1 || opcaomatriz == 2)
        {
            imprimirMatrizAntenas(lista, efeitos, coluna, linha, opcaomatriz);
        }
        else
        {
            printf("Modo de impressão inválido. Tente novamente.\n");
        }
        esperarEnter();

        break;

    case 5:
        limpartela();
        Grafo *grafo = NULL;
        executarMenuGrafo(grafo, lista, linha, coluna);
        break;

    default:
        printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        break;
    }

    // Atualiza a matriz e salva os dados em um arquivo binário.
    limpartela();
    imprimirMatrizAntenas(lista, efeitos, coluna, linha, 3);
    salvarEmBinario(lista, coluna, linha, "../bin/dados.bin");
    return lista;
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
 */
void processarOpcao(int opcao, Antena *lista, Nefasto *efeitos, int *linha, int *coluna)
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
            executarMenuSecundario(lista, efeitos, linha, coluna);
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
            executarMenuSecundario(lista, efeitos, linha, coluna);
        }

        break;
    case 0:
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
 */
Antena *executarMenu(Antena *lista, Nefasto *efeitos, int *linha, int *coluna)
{
    int opcao; // Variável para armazenar a opção escolhida pelo usuário.
    do
    {
        // Exibe o menu principal e solicita a escolha do usuário.
        mostrarMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Processa a opção escolhida e atualiza a lista de antenas.
        processarOpcao(opcao, lista, efeitos, linha, coluna);
    } while (opcao != 3); // Continua até que o usuário escolha sair.

    return lista; // Retorna a lista atualizada.
}

Grafo *executarMenuGrafo(Grafo *grafo, Antena *lista, int linha, int coluna)
{
    Antena *antena, *antena2;
    grafo = construirGrafo(lista);
    imprimirMatrizAntenas(lista, grafo, coluna, linha, 1);
    imprimirGrafo(grafo, lista, linha, coluna);
    int opcaografo;
    do
    {
        switch (opcaografo)
        {
        case 1:
            // Mostra o grafo
            imprimirGrafo(grafo, lista, linha, coluna);
            esperarEnter();
            break;
        case 2:
            // Executa BFS
            antena = selecionarAntena(lista, linha, coluna);
            if (antena)
            {
                bfs(grafo, antena);
            }
            else
            {
                break;
            }
            esperarEnter();
            break;
        case 3:
            // Executa DFS
            antena = selecionarAntena(lista, linha, coluna);
            if (antena)
            {
                dfs(grafo, antena);
            }
            else
            {
                break;
            }
            esperarEnter();
            break;
        case 4:
            // Mostra caminhos entre duas antenas
            antena = selecionarAntena(lista, linha, coluna);
            antena2 = selecionarAntena(lista, linha, coluna);
            if (antena && antena2)
            {
                mostrarCaminhos(grafo, antena, antena2);
            }
            else
            {
                break;
            }
            esperarEnter();
            break;
        default:
            break;
        }
        mostrarMenuGrafo();
        printf("Escolha uma opção: ");
        scanf("%d", &opcaografo);
    } while (opcaografo != 0); // Continue until the user chooses to exit.
    return grafo;
}

int mostrarMenuGrafo()
{
    printf("Menu Grafo:\n");
    printf("1. Mostrar grafo\n");
    printf("2. Executar BFS\n");
    printf("3. Executar DFS\n");
    printf("4. Mostrar caminhos entre duas antenas\n");
    printf("0. Voltar ao menu\n\n");
    printf("Escolha uma opção: ");
    return 0;
}