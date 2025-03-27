#include <stdio.h>
#include <stdlib.h>
#include "Funções.h"

void mostrarMenuPrincipal() {
    printf("Menu Principal:\n");
    printf("1. Ler arquivo binário\n");
    printf("2. Ler matriz\n");
    printf("3. Sair\n");
}

void mostrarMenuSecundario() {
    printf("Menu Secundário:\n");
    printf("1. Adicionar antena\n");
    printf("2. Eliminar antena\n");
    printf("3. Voltar ao menu principal\n");
}

void processarOpcaoSecundaria(int opcao, Antena** lista, Nefasto** efeitos, int linha, int coluna) {
    char freq;
    int x, y;
    switch (opcao) {
        case 1:
            //recolherDadosAntena(&freq, &x, &y, linha, coluna);
            if (inserirAntena(lista, freq, x, y)) {
                printf("Antena adicionada com sucesso!\n");
            } else {
                printf("Erro ao adicionar antena!\n");
            }
            *efeitos = calcularEfeitoNefasto(*lista, linha, coluna);
            imprimirMatrizAntenas(*lista, *efeitos, coluna, linha, 3);
            salvarEmBinario(*lista, *efeitos, coluna, linha, "dados.bin");
            break;
       /*  case 2:
            recolherDadosAntena(&freq, &x, &y, linha, coluna);
            if (eliminarAntena(lista, x, y)) {
                printf("Antena eliminada com sucesso.\n");
            } else {
                printf("Antena não encontrada.\n");
            }
            *efeitos = calcularEfeitoNefasto(*lista, linha, coluna);
            imprimirMatrizAntenas(*lista, *efeitos, coluna, linha, 3);
            salvarEmBinario(*lista, *efeitos, coluna, linha, "dados.bin");
            break; */
        case 3:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opção inválida.\n");
            break;
    }
}

void processarOpcao(int opcao, Antena** lista, Nefasto** efeitos, int* linha, int* coluna) {
    switch (opcao) {
        case 1:
            lerDeBinario(lista, efeitos, coluna, linha, "dados.bin");
            if (!*lista) {
                printf("Lista vazia ou erro ao ler!\n");
            } else {
                *efeitos = calcularEfeitoNefasto(*lista, *linha, *coluna);
                imprimirMatrizAntenas(*lista, *efeitos, *coluna, *linha, 3); // Modo 3: Antenas e Efeitos
                int opcaoSecundaria;
                do {
                    mostrarMenuSecundario();
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcaoSecundaria);
                    processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna);
                } while (opcaoSecundaria != 3);
            }
            break;
        case 2:
            *lista = lerAntenasDoFicheiro("matriz.txt", linha, coluna);
            if (!*lista) {
                printf("Lista vazia ou erro ao ler!\n");
            } else {
                listarAntenas(*lista);
                *efeitos = calcularEfeitoNefasto(*lista, *linha, *coluna);
                imprimirMatrizAntenas(*lista, *efeitos, *coluna, *linha, 3); // Modo 3: Antenas e Efeitos
                salvarEmBinario(*lista, *efeitos, *coluna, *linha, "dados.bin");
                int opcaoSecundaria;
                do {
                    mostrarMenuSecundario();
                    printf("Escolha uma opção: ");
                    scanf("%d", &opcaoSecundaria);
                    processarOpcaoSecundaria(opcaoSecundaria, lista, efeitos, *linha, *coluna);
                } while (opcaoSecundaria != 3);
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

void liberarListaAntenas(Antena *lista) {
    while (lista) {
        Antena *temp = lista;
        lista = lista->next;
        free(temp); // Removed reference to temp->efeitos
    }
}

int main() {
    int opcao, linha, coluna;
    Antena *lista = NULL;
    Nefasto *efeitos = NULL;

    while (1) {
        mostrarMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        processarOpcao(opcao, &lista, &efeitos, &linha, &coluna);
    }

    liberarListaAntenas(lista);
    liberarEfeitosNefastos(efeitos);
    return 0;
}