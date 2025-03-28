#include "../resources/Funções.h"
#include "../resources/Estruturas.h"

void limpartela()
{
    system("cls||clear");
}
void mostrarMenuPrincipal()
{
    printf("Menu Principal:\n");
    printf("1. Ler arquivo binário\n");
    printf("2. Ler matriz\n");
    printf("3. Sair\n");
}

void mostrarMenuSecundario()
{
    printf("Menu Secundário:\n");
    printf("1. Adicionar antena\n");
    printf("2. Eliminar antena\n");
    printf("3. Verificar Efeitos Nefasto\n");
    printf("4. Imprimir Matriz\n\n");
    printf("5. Voltar ao menu principal\n");
}

void processarOpcaoSecundaria(int opcao, Antena **lista, Nefasto **efeitos, int linha, int coluna)
{
    char freq;
    int x, y, opcaomatriz;
    switch (opcao)
    {
    case 1:
        recolherDadosAntena(&freq, &x, &y, linha, coluna);
        if (inserirAntena(lista, freq, x, y))
        {
            printf("Antena adicionada com sucesso!\n");
        }
        else
        {
            printf("Erro ao adicionar antena!\n");
        }
        *efeitos = calcularEfeitoNefasto(*lista, linha, coluna);
        imprimirMatrizAntenas(*lista, *efeitos, coluna, linha, 3);
        salvarEmBinario(*lista, *efeitos, coluna, linha, "dados.bin");
        break;
    case 2:
        recolherDadosAntena(&freq, &x, &y, linha, coluna);
        if (eliminarAntena(lista, x, y))
        {
            printf("Antena eliminada com sucesso.\n");
        }
        else
        {
            printf("Antena não encontrada.\n");
        }
        *efeitos = calcularEfeitoNefasto(*lista, linha, coluna);
        imprimirMatrizAntenas(*lista, *efeitos, coluna, linha, 3);
        salvarEmBinario(*lista, *efeitos, coluna, linha, "dados.bin");
        break;
    case 3:
        listarEfeitosNefastos(*efeitos);
        break;
    case 4:
        printf("Escolha o modo de impressão:\n");
        printf("1. Apenas antenas\n");
        printf("2. Apenas efeitos nefastos\n");
        printf("3. Antenas e efeitos nefastos\n");
        scanf("%d", &opcaomatriz);
        imprimirMatrizAntenas(*lista, *efeitos, coluna, linha, opcaomatriz);
        break;
    case 5:
        return; // Voltar ao menu principal
    default:
        printf("Opção inválida.\n");
        break;
    }
}

void processarOpcao(int opcao, Antena **lista, Nefasto **efeitos, int *linha, int *coluna)
{
    switch (opcao)
    {
    case 1:
        lerDeBinario(lista, efeitos, coluna, linha, "dados.bin");
        if (!*lista)
        {
            printf("Lista vazia ou erro ao ler!\n");
        }
        else
        {
            *efeitos = calcularEfeitoNefasto(*lista, *linha, *coluna);
            imprimirMatrizAntenas(*lista, *efeitos, *coluna, *linha, 3);
            int opcaoSecundaria;
            do
            {
                mostrarMenuSecundario();
                printf("Escolha uma opção: ");
                scanf("%d", &opcaoSecundaria);
                processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna);
            } while (opcaoSecundaria != 5); // Sair do menu secundário
        }
        break;
    case 2:
        *lista = lerAntenasDoFicheiro("matriz.txt", linha, coluna);
        if (!*lista)
        {
            printf("Lista vazia ou erro ao ler!\n");
        }
        else
        {
            listarAntenas(*lista);
            *efeitos = calcularEfeitoNefasto(*lista, *linha, *coluna);
            imprimirMatrizAntenas(*lista, *efeitos, *coluna, *linha, 3); // Modo 3: Antenas e Efeitos
            salvarEmBinario(*lista, *efeitos, *coluna, *linha, "dados.bin");
            int opcaoSecundaria;
            do
            {
                mostrarMenuSecundario();
                printf("Escolha uma opção: ");
                scanf("%d", &opcaoSecundaria);
                processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna);
            } while (opcaoSecundaria != 5); // Sair do menu secundário
        }
        break;
    case 3:
        printf("Saindo...\n");
        exit(0);
    default:
        printf("Opção inválida\n");
        break;
    }
}

void executarMenu(Antena **lista, Nefasto **efeitos, int *linha, int *coluna)
{
    int opcao;
    do
    {
        mostrarMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        processarOpcao(opcao, lista, efeitos, linha, coluna);
    } while (opcao != 3); // Sair do programa
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

void redimensionarMatriz(Antena **lista, Nefasto **efeitos, int *linhas, int *colunas, int novasLinhas, int novasColunas) {
    // Verifica se o novo tamanho é válido
    if (novasLinhas <= 0 || novasColunas <= 0) {
        printf("Erro: Dimensões inválidas para redimensionamento.\n");
        return;
    }

    // Atualiza as posições das antenas para se adequarem ao novo tamanho
    Antena *atualAntena = *lista;
    while (atualAntena) {
        if (atualAntena->x >= novasColunas || atualAntena->y >= novasLinhas) {
            printf("Removendo antena fora dos limites: (%d, %d)\n", atualAntena->x, atualAntena->y);
            eliminarAntena(lista, atualAntena->x, atualAntena->y);
        }
        atualAntena = atualAntena->next;
    }

    // Atualiza os efeitos nefastos para se adequarem ao novo tamanho
    Nefasto *atualNefasto = *efeitos;
    Nefasto *anteriorNefasto = NULL;
    while (atualNefasto) {
        if (atualNefasto->x1 >= novasColunas || atualNefasto->y1 >= novasLinhas ||
            atualNefasto->x2 >= novasColunas || atualNefasto->y2 >= novasLinhas) {
            printf("Removendo efeito nefasto fora dos limites: ((%d, %d), (%d, %d))\n",
                   atualNefasto->x1, atualNefasto->y1, atualNefasto->x2, atualNefasto->y2);

            // Remove o efeito nefasto da lista
            if (anteriorNefasto) {
                anteriorNefasto->next = atualNefasto->next;
            } else {
                *efeitos = atualNefasto->next;
            }
            Nefasto *temp = atualNefasto;
            atualNefasto = atualNefasto->next;
            free(temp);
        } else {
            anteriorNefasto = atualNefasto;
            atualNefasto = atualNefasto->next;
        }
    }
}
