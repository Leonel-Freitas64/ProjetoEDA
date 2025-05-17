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

void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/**
 * @brief Processa a entrada do usuário para o menu principal.
 *
 * @return int A opção escolhida pelo usuário.
 */
int mostrarMenuPrincipal(ListaMensagem *listaMensagens, int idioma)
{
    limpartela();
    mostrarMensagem(listaMensagens, idioma, 100);
    int opcao;
    char buffer[256]; // Buffer para capturar entradas inválidas

    while (1)
    {
        mostrarMensagem(listaMensagens, idioma, 1);
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
        limparBufferEntrada();
        mostrarMensagem(listaMensagens, idioma, 404);
    }
}

int mostrarMenuSecundario(ListaMensagem *listaMensagem, int idioma)
{
    mostrarMensagem(listaMensagem, idioma, 200);
    int opcao;
    char buffer[256]; // Buffer para capturar entradas inválidas

    while (true)
    {
        mostrarMensagem(listaMensagem, idioma, 1);
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
        mostrarMensagem(listaMensagem, idioma, 404);
    }
}

int executarMenuSecundario(Antena *lista, Nefasto *efeitos, int *linha, int *coluna, ListaMensagem *listaMensagens, int idioma)
{
    efeitos = calcularEfeitoNefasto(lista, *linha, *coluna);
    imprimirMatrizAntenas(lista, efeitos, *coluna, *linha, 3, listaMensagens, idioma);
    int opcaoSecundaria;
    do
    {
        opcaoSecundaria = mostrarMenuSecundario(listaMensagens, idioma);
        lista = processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna, listaMensagens, idioma);
    } while (opcaoSecundaria != 0);
    lista = liberarAntenas(lista);
    efeitos = liberarEfeitosNefastos(efeitos);
    limpartela();
}
/**
 * @brief Aguarda o usuário pressionar Enter para continuar.
 */
void esperarEnter(ListaMensagem *listaMensagem, int idioma)
{
    mostrarMensagem(listaMensagem, idioma, 500);
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
Antena *processarOpcaoSecundaria(int opcao, Antena *lista, Nefasto *efeitos, int linha, int coluna, ListaMensagem *listaMensagens, int idioma)
{
    char freq;       // Variável para armazenar a frequência da antena.
    int x, y;        // Variáveis para armazenar as coordenadas da antena.
    int opcaomatriz; // Variável para armazenar o modo de impressão da matriz.

    switch (opcao)
    {
    case 1:
        // Adiciona uma nova antena à lista.
        limparBufferEntrada();
        if (recolherDadosAntena(listaMensagens, idioma, &freq, &x, &y, linha, coluna))
        {
            lista = inserirAntena(lista, freq, x, y);
            efeitos = calcularEfeitoNefasto(lista, linha, coluna);
        }
        esperarEnter(listaMensagens, idioma);
        break;

    case 2:
        listarAntenas(lista, listaMensagens, idioma);
        limparBufferEntrada();
        if (recolherDadosAntena(listaMensagens, idioma, &freq, &x, &y, linha, coluna))
        {
            lista = eliminarAntena(lista, x, y, efeitos, listaMensagens, idioma);

            // Libera a lista de efeitos nefastos antes de recalcular
            liberarEfeitosNefastos(efeitos);

            // Recalcula os efeitos nefastos com a lista atualizada
            efeitos = calcularEfeitoNefasto(lista, linha, coluna);
        }
        else
        {
            mostrarMensagem(listaMensagens, idioma, 405);
        }
        esperarEnter(listaMensagens, idioma);
        break;

    case 3:
        // Verifica os efeitos nefastos.
        listarEfeitosNefastos(efeitos, listaMensagens, idioma);
        esperarEnter(listaMensagens, idioma);
        break;

    case 4:
        // Imprime a matriz de antenas e efeitos nefastos.
        limpartela();
        mostrarMensagem(listaMensagens, idioma, 2);
        scanf("%d", &opcaomatriz);

        if (opcaomatriz == 1 || opcaomatriz == 2)
        {
            imprimirMatrizAntenas(lista, efeitos, coluna, linha, opcaomatriz, listaMensagens, idioma);
        }
        else
        {
            mostrarMensagem(listaMensagens, idioma, 404);
        }
        esperarEnter(listaMensagens, idioma);

        break;

    case 5:
        limpartela();
        Grafo *grafo = NULL;
        executarMenuGrafo(grafo, lista, linha, coluna, listaMensagens, idioma);
        break;

    default:
        mostrarMensagem(listaMensagens, idioma, 404);
        break;
    }

    // Atualiza a matriz e salva os dados em um arquivo binário.
    limpartela();
    imprimirMatrizAntenas(lista, efeitos, coluna, linha, 3, listaMensagens, idioma);
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
void processarOpcao(int opcao, Antena *lista, Nefasto *efeitos, int *linha, int *coluna, ListaMensagem *listaMensagens, int idioma)
{
    switch (opcao)
    {
    case 1:
        // Carrega os dados de um arquivo binário
        lista = lerDeBinario(coluna, linha, "../bin/dados.bin");
        if (!lista)
        {
            mostrarMensagem(listaMensagens, idioma, 406);
        }
        else
        {
            limpartela();
            executarMenuSecundario(lista, efeitos, linha, coluna, listaMensagens, idioma);
            limparBufferEntrada();
        }
        break;
    case 2:
        // Carrega os dados de um arquivo de texto
        lista = lerAntenasDoFicheiro("matriz.txt", linha, coluna);
        if (!lista)
        {
            mostrarMensagem(listaMensagens, idioma, 406);
        }
        else
        {
            limpartela();
            executarMenuSecundario(lista, efeitos, linha, coluna, listaMensagens, idioma);
            limparBufferEntrada();
        }

        break;
    case 0:
        // Encerra o programa.
        mostrarMensagem(listaMensagens, idioma, 400);
        exit(0);
    default:
        mostrarMensagem(listaMensagens, idioma, 404);
        break;
    }
}

Grafo *executarMenuGrafo(Grafo *grafo, Antena *lista, int linha, int coluna, ListaMensagem *listaMensagens, int idioma)
{
    Antena *antena, *antena2;
    grafo = construirGrafo(lista, listaMensagens, idioma);
    int opcaografo;
    do
    {
        switch (opcaografo)
        {
        case 1:
            // Mostra o grafo
            imprimirGrafo(grafo, lista, linha, coluna, listaMensagens, idioma);
            esperarEnter(listaMensagens, idioma);
            break;
        case 2:
            // Executa BFS
            limparBufferEntrada();
            antena = selecionarAntena(lista, linha, coluna, listaMensagens, idioma);
            if (antena)
            {
                bfs(grafo, antena, listaMensagens, idioma);
            }
            else
            {
                break;
            }
            esperarEnter(listaMensagens, idioma);
            break;
        case 3:
            // Executa DFS
            limparBufferEntrada();
            antena = selecionarAntena(lista, linha, coluna, listaMensagens, idioma);
            if (antena)
            {

                dfs(grafo, antena, listaMensagens, idioma);
                esperarEnter(listaMensagens, idioma);
            }
            else
            {
                esperarEnter(listaMensagens, idioma);
                break;
            }
            break;
        case 4:
            // Mostra caminhos entre duas antenas
            limparBufferEntrada();
            antena = selecionarAntena(lista, linha, coluna, listaMensagens, idioma);
            antena2 = selecionarAntena(lista, linha, coluna, listaMensagens, idioma);
            if (antena && antena2)
            {
                mostrarCaminhos(grafo, antena, antena2);
                esperarEnter(listaMensagens, idioma);
            }
            else
            {
                esperarEnter(listaMensagens, idioma);

                break;
            }
            break;
        case 5:
            // Verifica interseções de frequências
            detetarIntersecoes(grafo, listaMensagens, idioma);
            esperarEnter(listaMensagens, idioma);
            break;
        default:
            break;
        }
        limpartela();
        imprimirMatrizAntenas(lista, grafo, coluna, linha, 1, listaMensagens, idioma);
        mostrarMensagem(listaMensagens, idioma, 300);
        mostrarMensagem(listaMensagens, idioma, 1);
        scanf("%d", &opcaografo);
    } while (opcaografo != 0);
    return grafo;
}
